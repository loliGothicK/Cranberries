#ifndef CRANBERRIES_RADIX_SORT_HPP
#define CRANBERRIES_RADIX_SORT_HPP

#include <iterator>
#include <vector>
#include <cstdint>
#include <cstring>
#include <bitset>
#include "..\utility.hpp"
#include "../integers.hpp"

namespace cranberries
{
namespace detail {
  template < size_t N >
  struct key_t_traits{
    using type = std::bitset<N>;
  };

  template < >
  struct key_t_traits<8> {
    using type = std::int8_t;
  };
  template < >
  struct key_t_traits<16> {
    using type = std::int16_t;
  };
  template < >
  struct key_t_traits<32> {
    using type = std::int32_t;
  };
  template < >
  struct key_t_traits<64> {
    using type = std::int64_t;
  };

  template < size_t N >
  using key_t = typename key_t_traits<N>::type;
}

  template < size_t N >
  std::bitset<N>& operator++( std::bitset<N>& bs ) {
    for (size_t i{}; i < N; ++i) {
      if (bs[i]) {
        bs.flip( i );
        continue;
      }
      else {
        bs.flip( i );
        break;
      }
    }
    return bs;
  }

  template < size_t N >
  std::bitset<N>& operator--( std::bitset<N>& bs ) {
    for (size_t i{}; i < N; ++i) {
      if (bs[i]) {
        bs.flip( i );
        break;
      }
      else {
        bs.flip( i );
        continue;
      }
    }
    return bs;
  }

  template < size_t N >
  decltype(auto) operator+=( std::bitset<N>& a, std::bitset<N> const& b )
  {
    bool c = a[0] & b[0];
    a[0] = a[0] ^ b[0];
    for (size_t i{ 1 }; i < N; ++i) {
      bool tmp = (a[i] & b[i]) || ((a[i] ^ b[i]) & c);
      a[i] = (a[i] ^ b[i]) ^ c;
      c = tmp;
    }
    return a;
  }

  template < size_t N >
  decltype(auto) operator+( std::bitset<N> const& a, std::bitset<N> const& b ) {
    return std::bitset<N>{a} += b;
  }

  class default_get_key
  {
  public:

    template <
      typename T,
      std::enable_if_t<
        !std::is_floating_point<std::decay_t<T>>::value,
        std::nullptr_t
      > = nullptr
    >
    auto
    operator()
    (
      T&& a
    )
      const noexcept
    {
      return a;
    }

    template <
      typename T,
      std::enable_if_t<
        std::is_floating_point<std::decay_t<T>>::value,
        std::nullptr_t
      > = nullptr
    >
    auto
    operator()
    (
      T&& a
    )
      const noexcept
    {
      detail::key_t<sizeof( std::decay_t<T> ) * 8> key;
      std::memcpy(&key,&a,sizeof(key));
      return key;
    }
  };

  template < typename F >
  class get_key_wrapper {
    F f_;
  public:
    get_key_wrapper( F f ) :f_{ std::forward<F>( f ) } {}

    template < typename T >
    auto operator()( T&& a ) {
      detail::key_t<sizeof( std::decay_t<T> ) * 8> key;
      std::memcpy( &key, &a, sizeof( key ) );
      return key;
    }

  };

  template < typename F >
  get_key_wrapper<F> make_get_key_wrapper( F&& f ) {
    return{ std::forward<F>( f ) };
  }

  template <
    class RAI,
    typename value_type = typename std::iterator_traits<remove_cv_ref_t<RAI>>::value_type,
    typename difference_type = typename std::iterator_traits<remove_cv_ref_t<RAI>>::difference_type,
    class G = default_get_key,
    typename key_type = std::result_of_t<std::decay_t<G>( value_type )>,
    int BITS = sizeof( key_type ) * 8,
    int UNIT = (BITS > 8 ? 8 : BITS >> 2),
    std::enable_if_t<
      std::is_integral<key_type>::value, std::nullptr_t
    > = nullptr
  >
  void
  ascending_radix_sort_impl
  (
    RAI a0,
    RAI aN,
    G&& get_key = default_get_key{}
  )
    noexcept
  {
    constexpr int KEYS = 1 << UNIT;
    constexpr int MASK = KEYS - 1;
    const difference_type N = aN - a0;
    const auto& a = a0;

    if (N < 2) return;
    std::vector<difference_type> h( KEYS );
    std::vector<value_type> b( N );
    const auto b0 = b.begin();
    const auto bN = b.end();
    for (int shift = 0; shift < BITS; shift += UNIT) {
      for (int k = 0; k < KEYS; k++) h[k] = 0;
      auto bi = b0;
      bool done = true;
      for (auto ai = a0; ai < aN; ++ai, ++bi) {
        const value_type& x = *ai;
        const key_type y = get_key( x ) >> shift;
        if (y > 0) done = false;
        ++h[static_cast<int>(y & MASK)];
        *bi = x;
      }
      if (done) return;
      for (int k = 1; k < KEYS; k++) h[k] += h[k - 1];
      for (bi = bN; bi > b0;) {
        const value_type& x = *(--bi);
        const int y = static_cast<int>((get_key( x ) >> shift) & MASK);
        const difference_type j = --h[y];
        a[j] = x;
      }
    }
  }

  template <
    class RAI,
    typename value_type = typename std::iterator_traits<std::decay_t<RAI>>::value_type,
    typename difference_type = typename std::iterator_traits<std::decay_t<RAI>>::difference_type,
    class G,
    typename key_type = std::result_of_t<G( value_type )>,
    int BITS = sizeof( key_type ) * 8,
    int UNIT = (BITS > 8 ? 8 : BITS >> 2),
    std::enable_if_t<
      !std::is_integral<key_type>::value, std::nullptr_t
    > = nullptr
  >
  void
  ascending_radix_sort_impl
  (
    RAI a0,
    RAI aN,
    G&& get_key
  )
    noexcept
  {
    std::bitset<BITS> KEYS{};
    KEYS.set( UNIT );
    std::bitset<BITS> MASK{};
    for (std::size_t i{}; i < UNIT; ++i) MASK.set( i );
    const difference_type N = aN - a0;
    const auto& a = a0;

    if (N < 2) return;
    std::vector<std::bitset<BITS>> h( KEYS.to_ullong() );
    std::vector<value_type> b( N );
    const auto b0 = b.begin();
    const auto bN = b.end();
    for (std::size_t shift = 0; shift < BITS; shift += UNIT) {
      for (std::size_t k = 0; k < KEYS.to_ullong(); k++) h[k] = std::bitset<BITS>{};
      auto bi = b0;
      bool done = true;
      for (auto ai = a0; ai < aN; ++ai, ++bi) {
        const value_type& x = *ai;
        const key_type y = get_key( x ) >> shift;
        if (y.any()) done = false;
        (++h[(y & MASK).to_ullong()]);
        *bi = x;
      }
      if (done) return;
      for (size_t k = 1; k < KEYS.to_ullong(); k++) h[k] += h[k - 1];
      for (bi = bN; bi > b0;) {
        const value_type& x = *(--bi);
        auto y = ((get_key( x ) >> shift) & MASK).to_ullong();
        const difference_type j = (--h[y]).to_ullong();
        a[j] = x;
      }
    }
  }


  template <
    class RAI,
    typename value_type = typename std::iterator_traits<std::decay_t<RAI>>::value_type,
    typename difference_type = typename std::iterator_traits<std::decay_t<RAI>>::difference_type,
    class G,
    typename key_type = std::result_of_t<G( value_type )>,
    int BITS = sizeof( key_type ) * 8,
    int UNIT = (BITS > 8 ? 8 : BITS >> 2),
    std::enable_if_t<
      !std::is_integral<key_type>::value, std::nullptr_t
    > = nullptr
  >
  void
  descending_radix_sort_impl
  (
    RAI a0,
    RAI aN,
    G&& get_key
  )
    noexcept
  {
    std::bitset<BITS> KEYS{};
    KEYS.set( UNIT );
    std::bitset<BITS> MASK{};
    for (std::size_t i{}; i < UNIT; ++i) MASK.set( i );
    const difference_type N = aN - a0;
    const auto& a = a0;

    if (N < 2) return;
    std::vector<std::bitset<BITS>> h( KEYS.to_ullong() );
    std::vector<value_type> b( N );
    const auto b0 = b.begin();
    const auto bN = b.end();
    for (std::size_t shift = 0; shift < BITS; shift += UNIT) {
      for (std::size_t k = 0; k < KEYS.to_ullong(); k++) h[k] = std::bitset<BITS>{};
      auto bi = b0;
      bool done = true;
      for (auto ai = a0; ai < aN; ++ai, ++bi) {
        const value_type& x = *ai;
        const key_type y = get_key( x ) >> shift;
        if (y.any()) done = false;
        (++h[(y & MASK).to_ullong()]);
        *bi = x;
      }
      if (done) return;
      for (auto k = MASK.to_ullong(); k > 0; k--) h[k - 1] += h[k];
      for (bi = bN; bi > b0;) {
        const value_type& x = *(--bi);
        auto y = ((get_key( x ) >> shift) & MASK).to_ullong();
        const difference_type j = (--h[y]).to_ullong();
        a[j] = x;
      }
    }
  }

  template <
    class RAI,
    typename value_type = typename std::iterator_traits<remove_cv_ref_t<RAI>>::value_type,
    typename difference_type = typename std::iterator_traits<remove_cv_ref_t<RAI>>::difference_type,
    class G = default_get_key,
    typename key_type = std::result_of_t<std::decay_t<G>( value_type )>,
    int BITS = sizeof( key_type ) * 8,
    int UNIT = ( BITS > 8 ? 8 : BITS >> 2),
    std::enable_if_t<
      std::is_integral<key_type>::value, std::nullptr_t
    > = nullptr
  >
  void
  descending_radix_sort_impl
  (
    RAI a0,
    RAI aN,
    G&& get_key = default_get_key{}
  )
    noexcept
  {
    constexpr int KEYS = 1 << UNIT;
    constexpr int MASK = KEYS - 1;
    const difference_type N = aN - a0;
    const auto& a = a0;

    if (N < 2) return;
    std::vector<difference_type> h( KEYS );
    std::vector<value_type> b( N );
    const auto b0 = b.begin();
    const auto bN = b.end();
    for (int shift = 0; shift < BITS; shift += UNIT) {
      for (int k = 0; k < KEYS; k++) h[k] = 0;
      auto bi = b0;
      bool done = true;
      for (auto ai = a0; ai < aN; ++ai, ++bi) {
        const value_type& x = *ai;
        const key_type y = get_key( x ) >> shift;
        if (y > 0) done = false;
        ++h[static_cast<int>(y & MASK)];
        *bi = x;
      }
      if (done) return;
      for (int k = MASK; k > 0; k--) h[k - 1] += h[k];
      for (bi = bN; bi > b0;) {
        const value_type& x = *(--bi);
        const int y = static_cast<int>((get_key( x ) >> shift) & MASK);
        const difference_type j = --h[y];
        a[j] = x;
      }
    }
  }


  template <
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G(T)>,
    std::enable_if_t<
      conjunctional<
        std::is_unsigned,
        std::is_integral
      >::type<K>::value,
      std::nullptr_t
    > = nullptr
  >
  void
  ascending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    ascending_radix_sort_impl(
      range.begin(), range.end(),
      std::forward<G>( key_getter )
    );
  }

  template <
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G(T)>,
    std::enable_if_t<
      conjunctional<
        std::is_signed,
        std::is_integral
      >::type<K>::value,
      std::nullptr_t
    > = nullptr
  >
  void
  ascending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    auto middle = std::stable_partition(
      range.begin(), range.end(),
      []( const T& a ) { return a < 0; }
    );
    descending_radix_sort_impl(
      range.begin(), middle,
      std::forward<G>( key_getter )
    );
    ascending_radix_sort_impl(
      middle, range.end(),
      std::forward<G>( key_getter )
    );
  }
  

  template <
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      std::is_floating_point<K>::value,
    std::nullptr_t
    > = nullptr
  >
  void
  ascending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    auto middle = std::stable_partition(
      range.begin(), range.end(),
      []( const T& a ) { return a < 0; }
    );
    descending_radix_sort_impl(
      range.begin(), middle,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
    ascending_radix_sort_impl(
      middle, range.end(),
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }

  template <
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G(T)>,
    std::enable_if_t<
      conjunctional<
        std::is_unsigned,
        std::is_integral
      >::type<K>::value,
      std::nullptr_t
    > = nullptr
  >
  void
  descending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    descending_radix_sort_impl(
      range.begin(), range.end(),
      std::forward<G>( key_getter )
    );
  }

  template <
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G(T)>,
    std::enable_if_t<
      conjunctional<
        std::is_signed,
        std::is_integral
      >::type<K>::value,
      std::nullptr_t
    > = nullptr
  >
  void
  descending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    auto middle = std::stable_partition(
      range.begin(), range.end(),
      []( const T& a ) { return a > -1; }
    );
    descending_radix_sort_impl(
      range.begin(), middle,
      std::forward<G>( key_getter )
    );
    ascending_radix_sort_impl(
      middle, range.end(),
      std::forward<G>( key_getter )
    );
  }

  template <
    int UNIT = 8,
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      std::is_floating_point<K>::value,
    std::nullptr_t
    > = nullptr
  >
  void
  descending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    auto middle = std::stable_partition(
      range.begin(), range.end(),
      []( const T& a ) { return a > -1; }
    );
    descending_radix_sort_impl(
      range.begin(), middle,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
    ascending_radix_sort_impl(
      middle, range.end(),
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }


  template <
    int UNIT = 8,
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !std::is_arithmetic<K>::value,
    std::nullptr_t
    > = nullptr
  >
  void
  ascending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    auto middle = std::stable_partition(
      range.begin(), range.end(),
      []( const T& a ) { return a < 0; }
    );
    descending_radix_sort_impl(
      range.begin(), middle,
      std::forward<G>( key_getter )
    );
    ascending_radix_sort_impl(
      middle, range.end(),
      std::forward<G>( key_getter )
    );
  }

  template <
    int UNIT = 8,
    typename Range,
    typename T = typename std::decay_t<Range>::value_type,
    typename G = default_get_key,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !std::is_arithmetic<K>::value,
    std::nullptr_t
    > = nullptr
  >
  void
  descending_radix_sort
  (
    Range&& range,
    G&& key_getter = default_get_key{}
  )
    noexcept
  {
    auto middle = std::stable_partition(
      range.begin(), range.end(),
      []( const T& a ) { return a > -1; }
    );
    descending_radix_sort_impl(
      range.begin(), middle,
      std::forward<G>( key_getter )
    );
    ascending_radix_sort_impl(
      middle, range.end(),
      std::forward<G>( key_getter )
    );
  }
}

#endif
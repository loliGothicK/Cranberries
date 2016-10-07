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

struct default_get_key {
  template < typename T >
  std::bitset<sizeof(T)*8> operator()( T&& a ) {
    std::bitset<sizeof( T )*8> key;
    std::memcpy( &key, &a, sizeof(key) );
    return key;
  }
};

template <
  typename F
>
struct get_key_wrapper
{
public:
  get_key_wrapper( F f ) : f_{ std::forward<F>( f ) } {}

  template <
    typename T,
    size_t BITS = sizeof( std::result_of_t<F( T )> )*8
  >
  auto operator()( T&& a ) {
    std::bitset<BITS> key;
    auto tmp = f_( a );
    std::memcpy( &key, &tmp, BITS );
    return key;
  }

private:
  F f_;
};

template < typename F >
get_key_wrapper<F> make_get_key_wrapper( F&& f ) {
  return{ std::forward<F>(f) };
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

template <
  class RAI,
  typename value_type = typename std::iterator_traits<std::decay_t<RAI>>::value_type,
  typename difference_type = typename std::iterator_traits<std::decay_t<RAI>>::difference_type,
  class G,
  typename key_type = std::result_of_t<G( value_type )>,
  int BITS = sizeof( key_type ) * 8,
  int UNIT = (BITS > 32 ? 8 : BITS >> 2),
  std::enable_if_t<
  !std::is_arithmetic<key_type>::value, std::nullptr_t
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
  int UNIT = ( BITS > 32 ? 8 : BITS >> 2 ),
  std::enable_if_t<
    !std::is_arithmetic<key_type>::value, std::nullptr_t
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
      ++h[(y & MASK).to_ullong()];
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
  typename Range,
  typename T = typename std::decay_t<Range>::value_type,
  typename G = default_get_key,
  typename K = std::result_of_t<G( T )>,
  std::enable_if_t<
    is_bitset_v<K>,
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
#ifdef _MSC_VER
  if (conjunctional<std::is_signed, std::is_integral>::type<T>::value)
  {
    ascending_radix_sort_impl(
      range.begin(), middle,
      std::forward<G>( key_getter )
    );
  } else {
    descending_radix_sort_impl(
      range.begin(), middle,
      std::forward<G>( key_getter )
    );
  }
#elif
  descending_radix_sort_impl(
    range.begin(), middle,
    std::forward<G>( key_getter )
  );
#endif
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
    is_bitset_v<K>,
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
#ifdef _MSC_VER
  if (conjunctional<std::is_signed, std::is_integral>::type<T>::value)
  {
    descending_radix_sort_impl(
      middle, range.end(),
      std::forward<G>( key_getter )
    );
  } else {
    ascending_radix_sort_impl(
      middle, range.end(),
      std::forward<G>( key_getter )
    );
  }
#elif
  aescending_radix_sort_impl(
    middle, range.end(),
    std::forward<G>( key_getter )
  );
#endif
}


template <
  typename Range,
  typename T = typename std::decay_t<Range>::value_type,
  typename G = default_get_key,
  typename K = std::result_of_t<G( T )>,
  std::enable_if_t<
    !is_bitset_v<K>,
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
#ifdef _MSC_VER
  if (conjunctional<std::is_signed, std::is_integral>::type<T>::value)
  {
    ascending_radix_sort_impl(
      range.begin(), middle,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }
  else {
    descending_radix_sort_impl(
      range.begin(), middle,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }
#elif
  descending_radix_sort_impl(
    range.begin(), middle,
    make_get_key_wrapper( std::forward<G>( key_getter ) )
  );
#endif
  ascending_radix_sort_impl(
    middle, range.end(),
    make_get_key_wrapper( std::forward<G>( key_getter ) )
  );
}

template <
  typename Range,
  typename T = typename std::decay_t<Range>::value_type,
  typename G = default_get_key,
  typename K = std::result_of_t<G( T )>,
  std::enable_if_t<
    !is_bitset_v<K>,
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
#ifdef _MSC_VER
  if (conjunctional<std::is_signed, std::is_integral>::type<T>::value)
  {
    descending_radix_sort_impl(
      middle, range.end(),
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }
  else {
    ascending_radix_sort_impl(
      middle, range.end(),
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }
#elif
  aescending_radix_sort_impl(
    middle, range.end(),
    make_get_key_wrapper( std::forward<G>( key_getter ) )
  );
#endif
}


} // ! namespace cranberries

#endif
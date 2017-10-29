#ifndef CRANBERRIES_RADIX_SORT_HPP
#define CRANBERRIES_RADIX_SORT_HPP

#include <iterator>
#include <vector>
#include <cstdint>
#include <cstring>
#include <bitset>
#include "../utility.hpp"
#include "../integers.hpp"

namespace cranberries
{

  // defaulted key getter function obj
  // for radix sort
  struct default_get_key
  {
    template <
      typename T
    >
    std::bitset<sizeof(T)*8>
    operator()
    (
      T&& a
    )
      noexcept
    {
      std::bitset<sizeof( T )*8> key;
      std::memcpy( &key, &a, sizeof(key) );
      return key;
    }
  };


  // non bitset key getter to bitset key getter
  template <
    typename F // key getter
  >
  struct get_key_wrapper
  {
  public:
    get_key_wrapper( F f ) : f_{ std::forward<F>( f ) } {}

    template <
      typename T,
      size_t BITS = sizeof( std::result_of_t<F( T )> )*8
    >
    auto
    operator()
    (
      T&& a
    )
      noexcept
    {
      std::bitset<BITS> key;
      auto tmp = f_( std::forward<T>( a ) ); // perfect forwarding
      std::memcpy( &key, &tmp, BITS );
      return key;
    }

  private:
    F f_; // key getter
  };

  // make function for get_key_wrapper
  template < typename F >
  inline get_key_wrapper<F> make_get_key_wrapper( F&& f ) {
    return{ std::forward<F>(f) };
  }

  // increment for bitset
  // ! invisible to ADL
  template <
    size_t N
  >
  std::bitset<N>&
  operator++
  (
    std::bitset<N>& bs
  )
    noexcept
  {
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

  // decrement for bitset
  // ! invisible to ADL
  template < size_t N >
  inline
  std::bitset<N>&
  operator--
  (
    std::bitset<N>& bs
  )
    noexcept
  {
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

  // full adder assignment for bitset
  // ! invisible to ADL
  template < size_t N >
  inline
  decltype(auto)
  operator+=
  (
    std::bitset<N>& a,
    std::bitset<N> const& b
  )
    noexcept
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


  /***********************************************************

  Implementation of ascending/descengding radix sort

  Arguments:
  | RAI(Randmo Access Iterator) : first
  | RAI(Random Access Iterator) : last
  | G&&(key Getter) : get_key
 
  Note:
  | Sorting iterator range [ first, last ).
  | get_key is function obj that take value_type in argument
  | and return bitset as keys.
  
  ***********************************************************/
  template <
    class RAI,
    typename value_type = typename std::iterator_traits<std::decay_t<RAI>>::value_type,
    typename difference_type = typename std::iterator_traits<std::decay_t<RAI>>::difference_type,
    class G,
    typename key_type = std::result_of_t<G( value_type )>,
    int BITS = sizeof( key_type ) * 
#ifdef _MSC_VER
    8,
#else // for GCC/Clang
    4,
#endif
  int UNIT = (BITS > 32 ? 8 : BITS >> 2),
    std::enable_if_t<
      is_bitset_v<key_type>, std::nullptr_t // key_type must be bitset
    > = nullptr
  >
  inline
  void
  ascending_radix_sort_impl
  (
    RAI first,
    RAI last,
    G&& get_key
  )
    noexcept
  {
    std::bitset<BITS> KEYS{};
    KEYS.set( UNIT );
    std::bitset<BITS> MASK{};
    for (std::size_t i{}; i < UNIT; ++i) MASK.set( i );
    const difference_type N = last - first;
    const auto& a = first;

    if (N < 2) return;
    std::vector<std::bitset<BITS>> h( KEYS.to_ullong() );
    std::vector<value_type> b( N );
    const auto b0 = b.begin();
    const auto bN = b.end();
    for (std::size_t shift = 0; shift < BITS; shift += UNIT) {
      for (std::size_t k = 0; k < KEYS.to_ullong(); k++) h[k] = std::bitset<BITS>{};
      auto bi = b0;
      bool done = true;
      for (auto ai = first; ai < last; ++ai, ++bi) {
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
    int BITS = sizeof( key_type ) *
#ifdef _MSC_VER
    8,
#else // for GCC/Clang
    4,
#endif
    int UNIT = ( BITS > 32 ? 8 : BITS >> 2 ),
    std::enable_if_t<
      is_bitset_v<key_type>, std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort_impl
  (
    RAI first,
    RAI last,
    G&& get_key
  )
    noexcept
  {
    std::bitset<BITS> KEYS{};
    KEYS.set( UNIT );
    std::bitset<BITS> MASK{};
    for (std::size_t i{}; i < UNIT; ++i) MASK.set( i );
    const difference_type N = last - first;
    const auto& a = first;

    if (N < 2) return;
    std::vector<std::bitset<BITS>> h( KEYS.to_ullong() );
    std::vector<value_type> b( N );
    const auto b0 = b.begin();
    const auto bN = b.end();
    for (std::size_t shift = 0; shift < BITS; shift += UNIT) {
      for (std::size_t k = 0; k < KEYS.to_ullong(); k++) h[k] = std::bitset<BITS>{};
      auto bi = b0;
      bool done = true;
      for (auto ai = first; ai < last; ++ai, ++bi) {
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


  /*************************

   | No get_key.
   | signed type range.

   | Make key automatically from value
   | copying to bitset<sizeof(value)>
   | using std::memcpy.

   Note:
   |  In the first, partitioning range to
   | negative and positive numbers
   | using std::stable_partition.
   | Next, sorting each partitioned range.

  **************************/


  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      std::is_signed<T>::value,
    std::nullptr_t
    > = nullptr
  >
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    auto middle = std::stable_partition(
      first, last,
      []( const T& a ) { return a < 0; }
    );
  #ifdef _MSC_VER
    if (std::is_integral<T>::value)
    {
      ascending_radix_sort_impl(
        first, middle,
        default_get_key{}
      );
    } else {
      descending_radix_sort_impl(
        first, middle,
        default_get_key{}
      );
    }
  #else // for GCC/Clang
    descending_radix_sort_impl(
      first, middle,
      default_get_key{}
    );
  #endif
    ascending_radix_sort_impl(
      middle, last,
      default_get_key{}
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      std::is_signed<T>::value,
    std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    auto middle = std::stable_partition(
      first, last,
      []( const T& a ) { return a > -1; }
    );
    descending_radix_sort_impl(
      first, middle,
      default_get_key{}
    );
  #ifdef _MSC_VER
    if (std::is_integral<T>::value)
    {
      descending_radix_sort_impl(
        middle, last,
        default_get_key{}
      );
    } else {
      ascending_radix_sort_impl(
        middle, last,
        default_get_key{}
      );
    }
  #else // for GCC/Clang
    ascending_radix_sort_impl(
      middle, last,
      default_get_key{}
    );
  #endif
  }


  /*************************

  | No get_key.
  | unsigned type range.

  | Make key automatically from value
  | to copy to bitset<sizeof(value)>
  | using std::memcpy.

  **************************/


  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      std::is_unsigned<T>::value,
    std::nullptr_t
    > = nullptr
  >
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    ascending_radix_sort_impl(
        first, last,
        default_get_key{}
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      std::is_unsigned<T>::value,
    std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    descending_radix_sort_impl(
      first, last,
      default_get_key{}
    );
  }

  /*************************

  | No get_key.
  | bitset type range.

  Note:
  | Value(bitset) directly using as key.

  **************************/


  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      is_bitset_v<T>,
      std::nullptr_t
    > = nullptr
  >
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    ascending_radix_sort_impl(
      first, last,
      []( auto&& a ) { return a; }
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      is_bitset_v<T>,
      std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    descending_radix_sort_impl(
      first, last,
      []( auto&& a ) { return a; }
    );
  }


  /*************************

  | No get_key.
  | non bitset, non arithmetic type range.

  | These two are deprecated.
  | Can sort successfully if and only if 
  | value_type's upper bits is in upper key.

  Note:
  | Make key automatically from value
  | to copy to bitset<sizeof(value)>
  | using std::memcpy.


  **************************/


  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      !is_bitset_v<T> && !std::is_arithmetic<T>::value,
    std::nullptr_t
    > = nullptr
  >
  [[deprecated]]
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    ascending_radix_sort_impl(
      first, last,
      default_get_key{}
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    std::enable_if_t<
      !is_bitset_v<T> && !std::is_arithmetic<T>::value,
    std::nullptr_t
    > = nullptr
  >
  [[deprecated]]
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last
  )
    noexcept
  {
    descending_radix_sort_impl(
      first, last,
      default_get_key{}
    );
  }


  /*************************

  | get_key specified.
  | signed type key.

  Note:
  |  Key ( return value of get_key )
  | being copied to bitset<sizeof(key)>
  | using std::memcpy.
  |  In the first, partitioning range to
  | negative and positive numbers
  | using std::stable_partition.
  | Next, sorting each partitioned range.

  **************************/



  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !is_bitset_v<K> && std::is_signed<K>::value,
    std::nullptr_t
    > = nullptr
  >
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    auto middle = std::stable_partition(
      first, last,
      []( const T& a ) { return a < 0; }
    );
  #ifdef _MSC_VER
    if (std::is_integral<T>::value)
    {
      ascending_radix_sort_impl(
        first, middle,
        make_get_key_wrapper( std::forward<G>( key_getter ) )
      );
    }
    else {
      descending_radix_sort_impl(
        first, middle,
        make_get_key_wrapper( std::forward<G>( key_getter ) )
      );
    }
  #else // for GCC/Clang
    descending_radix_sort_impl(
      first, middle,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  #endif
    ascending_radix_sort_impl(
      middle, last,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      is_bitset_v<K> && std::is_signed<K>::value,
    std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    auto middle = std::stable_partition(
      first, last,
      []( const T& a ) { return a > -1; }
    );
    descending_radix_sort_impl(
      first, middle,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  #ifdef _MSC_VER
    if (std::is_integral<T>::value)
    {
      descending_radix_sort_impl(
        middle, last,
        make_get_key_wrapper( std::forward<G>( key_getter ) )
      );
    }
    else {
      ascending_radix_sort_impl(
        middle, last,
        make_get_key_wrapper( std::forward<G>( key_getter ) )
      );
    }
  #else // for GCC/Clang
    ascending_radix_sort_impl(
      middle, last,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  #endif
  }

  /*************************

  | get_key specified.
  | unsigned type key.

  Note:
  |  Key ( return value of get_key )
  | be copy to bitset<sizeof(key)>
  | using std::memcpy.

  **************************/


  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !is_bitset_v<K> && std::is_unsigned<K>::value,
      std::nullptr_t
    > = nullptr
  >
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    ascending_radix_sort_impl(
      std::forward<RAI>( first ), std::forward<RAI>( last ),
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !is_bitset_v<K> && std::is_unsigned<K>::value,
      std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    descending_radix_sort_impl(
      std::forward<RAI>( first ), std::forward<RAI>( last ),
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }


  /*************************

  | get_key specified.
  | bitset key.

  Note:
  | key_get directly using.

  **************************/


  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      is_bitset_v<K>,
      std::nullptr_t
    > = nullptr
  >
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    ascending_radix_sort_impl(
      std::forward<RAI>( first ), std::forward<RAI>( last ),
      std::forward<G>( key_getter )
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      is_bitset_v<K>,
      std::nullptr_t
    > = nullptr
  >
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    descending_radix_sort_impl(
      first, last,
      std::forward<G>( key_getter )
    );
  }


  /*************************

  | get_key specified.
  | Non bitset, non arithmetic key.
  |
  | These two are deprecated.
  | Can sort successfully if and only if
  | upper bits is in upper keys.

  Note:
  | Key ( return value of get_key )
  | is copy to bitset<sizeof<(key)>
  | using std::memcpy.

  **************************/

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !is_bitset_v<K> && !std::is_arithmetic<K>::value,
    std::nullptr_t
    > = nullptr
  >
  [[deprecated]]
  inline
  void
  ascending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    ascending_radix_sort_impl(
      first, last,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }

  template <
    typename RAI,
    typename T = typename std::iterator_traits<RAI>::value_type,
    typename G,
    typename K = std::result_of_t<G( T )>,
    std::enable_if_t<
      !is_bitset_v<K> && !std::is_arithmetic<K>::value,
    std::nullptr_t
    > = nullptr
  >
  [[deprecated]]
  inline
  void
  descending_radix_sort
  (
    RAI first,
    RAI last,
    G&& key_getter
  )
    noexcept
  {
    descending_radix_sort_impl(
      first, last,
      make_get_key_wrapper( std::forward<G>( key_getter ) )
    );
  }

} // ! namespace cranberries

#endif
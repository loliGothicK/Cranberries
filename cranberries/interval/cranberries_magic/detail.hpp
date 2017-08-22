/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_DETAIL_HPP
#define CRANBERRIES_INTERVAL_LIB_DETAIL_HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <type_traits>



namespace cranberries {
  namespace cranberries_magic {

    // For isinterval_v.
    // This class is tag for identify that class T is interval.
    // Class inheritancing interval_tag is interval.
    // expr_tag similar to.
    // Empty Base Optimization just works.
    class interval_base {};

    template < typename L, typename R, class = void>
    struct is_available_total_order : std::false_type {};

    template < typename L, typename R >
    struct is_available_total_order<L, R,
      std::enable_if_t<std::is_same<decltype(total_less( std::declval<const L&>(), std::declval<const R&>() )), bool>::value>
    > : std::true_type {};

    template < typename L, typename R >
    constexpr bool is_available_total_order_v = is_available_total_order<L, R>::value;

  } // ! cranberries_magic

    // is_ineterval_v definition
  template < typename T >
  struct is_interval {
    static constexpr bool value = std::is_base_of<cranberries_magic::interval_base, T>::value;
  };

  template < typename T >
  constexpr bool is_interval_v = is_interval<T>::value;

} // ! cranberries

namespace cranberries {



  namespace cranberries_magic {
    template < typename T >
#ifdef _MSC_VER
    __forceinline
#elif __GNUC__
    __attribute__( (always_inline) )
#elif __clang__
    __attribute__( (always_inline) )
#else
    inline
#endif
      constexpr bool is_overflow( T&& a ) noexcept
    {
      return std::abs( a ) == std::numeric_limits<std::decay_t<T>>::infinity()
        || std::abs( a ) == std::numeric_limits<std::decay_t<T>>::max();
    }

    template < typename T >
#ifdef _MSC_VER
    __forceinline
#elif __GNUC__
    __attribute__( (always_inline) )
#elif __clang__
    __attribute__( (always_inline) )
#else
    inline
#endif
      constexpr bool is_overflow( T&& a, T&& b ) noexcept
    {
      return std::abs( a ) == std::numeric_limits<std::decay_t<T>>::infinity()
        || std::abs( b ) == std::numeric_limits<std::decay_t<T>>::infinity()
        || std::abs( a ) == std::numeric_limits<std::decay_t<T>>::max()
        || std::abs( b ) == std::numeric_limits<std::decay_t<T>>::max();
    }



  } // ! cranberries_magic 
} // ! cranberries

#endif

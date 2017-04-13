#ifndef CRANBRIIES_INTERVAL_LIB_COMPARE_EXPLICIT_HPP
#define CRANBRIIES_INTERVAL_LIB_COMPARE_EXPLICIT_HPP

#include <type_traits>
#include "../cranberries_magic/detail.hpp"
#include "../interval.hpp"

namespace cranberries {

  //-----------------------------------//
  /*   interval relational functions   */
  //-----------------------------------//

  /*  Interval Ordering  */

  // interval less
  template < typename L, typename R >
  inline constexpr bool interval_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.upper() < y.lower() );
  }

  template < typename R >
  inline constexpr bool interval_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.lower() );
  }

  template < typename L >
  inline constexpr bool interval_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() < y );
  }

  template < typename L, typename R >
  inline constexpr bool interval_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.upper() <= y.lower() );
  }
  template < typename R >
  inline constexpr bool interval_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.lower() );
  }

  template < typename L >
  inline constexpr bool interval_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() <= y );
  }

  // interval greater
  template < typename L, typename R >
  inline constexpr bool interval_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() > y.upper() );
  }

  template < typename R >
  inline constexpr bool interval_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.upper() );
  }

  template < typename L >
  inline constexpr bool interval_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool interval_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() >= y.upper() );
  }

  template < typename R >
  inline constexpr bool interval_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x >= y.upper() );
  }

  template < typename L >
  inline constexpr bool interval_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= y );
  }

  // interval unordered
  template < typename L, typename R >
  inline constexpr bool interval_unordered( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( interval_less( x, y ) == false && interval_greater( x, y ) == false );
  }
  template < typename R >
  inline constexpr bool interval_unordered( typename interval<R>::value_type const&, interval<R> const& ) noexcept
  {
    return false;
  }
  template < typename L >
  inline constexpr bool interval_unordered( interval<L> const&, typename interval<L>::value_type const& ) noexcept
  {
    return false;
  }

  /*  Partial Ordering  */

  // partial less
  template < typename L, typename R >
  inline constexpr bool partial_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() < y.lower() && x.upper() < y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.lower() );
  }

  template < typename L >
  inline constexpr bool partial_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() < y );
  }

  template < typename L, typename R >
  inline constexpr bool partial_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() <= y.lower() && x.upper() <= y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.lower() );
  }

  template < typename L >
  inline constexpr bool partial_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() <= y );
  }

  // partial greater
  template < typename L, typename R >
  inline constexpr bool partial_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() > y.lower() && x.upper() > y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.upper() );
  }

  template < typename L >
  inline constexpr bool partial_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool partial_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() >= y.lower() && x.upper() >= y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( std::forward<R>( x ) >= y.upper() );
  }

  template < typename L >
  inline constexpr bool partial_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= y );
  }

  // partial unordered
  template < typename L, typename R >
  inline constexpr bool partial_unordered( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( partial_less( x, y ) == false && partial_greater( x, y ) == false );
  }
  template < typename R >
  inline constexpr bool partial_unordered( typename interval<R>::value_type const&, interval<R> const& ) noexcept
  {
    return false;
  }
  template < typename L >
  inline constexpr bool partial_unordered( interval<L> const&, typename interval<L>::value_type const& ) noexcept
  {
    return false;
  }

  /*  Weak Ordering  */

  // weak less
  template < typename L, typename R >
  inline constexpr bool weak_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() < y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool weak_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() <= y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= std::forward<L>( y ) );
  }

  // weak greater
  template < typename L, typename R >
  inline constexpr bool weak_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() > y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool weak_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() >= y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x >= y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= y );
  }

  // weak equal
  template < typename L, typename R >
  inline constexpr bool weak_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( weak_less( x, y ) == false && weak_less( y, x ) == false );
  }

  template < typename R >
  inline constexpr bool weak_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x == y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() == std::forward<L>( y ) );
  }

  /*  Total Ordering  */

  // total less
  template < typename L, typename R >
  inline constexpr bool total_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() < y.upper() : x.lower() < y.lower();
  }

  template < typename R >
  inline constexpr bool total_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.upper() );
  }

  template < typename L >
  inline constexpr bool total_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() < y );
  }

  template < typename L, typename R >
  inline constexpr bool total_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() <= y.upper() : x.lower() <= y.lower();
  }

  template < typename R >
  inline constexpr bool total_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.upper() );
  }

  template < typename L >
  inline constexpr bool total_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() <= y );
  }

  // total greater
  template < typename L, typename R >
  inline constexpr bool total_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() > y.upper() : x.lower() > y.lower();
  }

  template < typename R >
  inline constexpr bool total_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.lower() );
  }

  template < typename L >
  inline constexpr bool total_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() > y );
  }

  template < typename L, typename R >
  inline constexpr bool total_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() >= y.upper() : x.lower() >= y.lower();
  }

  template < typename R >
  inline constexpr bool total_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x >= y.lower() );
  }

  template < typename L >
  inline constexpr bool total_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() >= y );
  }

  // total equal
  template < typename L, typename R >
  inline constexpr bool total_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() == y.lower() && x.upper() == y.upper() );
  }

  template < typename L, typename R, std::enable_if_t<!is_interval_v<R>, std::nullptr_t> = nullptr >
  inline constexpr bool total_equal( R const& x, interval<L> const& y ) noexcept
  {
    return ( x == y.lower() && x == y.upper() );
  }

  template < typename L, typename R, std::enable_if_t<!is_interval_v<R>, std::nullptr_t> = nullptr >
  inline constexpr bool total_equal( interval<L> const& x, R const& y ) noexcept
  {
    return ( x.lower() == std::forward<L>( y ) && x.upper() == std::forward<L>( y ) );
  }


} // ! cranberries
#endif
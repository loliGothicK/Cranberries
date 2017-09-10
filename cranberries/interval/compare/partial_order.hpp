#ifndef CRANBRIIES_PARTIAL_LIB_COMPARE_PARTIAL_ORDER_HPP
#define CRANBRIIES_PARTIAL_LIB_COMPARE_PARTIAL_ORDER_HPP

#include "../interval.hpp"
#include "explicit.hpp"

namespace cranberries {
namespace compare {
namespace partial_ordering_policy
{
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>& x, interval<R>& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>& x, interval<R> const& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>& x, interval<R>&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>& x, interval<R> const&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const& x, interval<R>& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const& x, interval<R> const& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const& x, interval<R>&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const& x, interval<R> const&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>&& x, interval<R>& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>&& x, interval<R> const& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>&& x, interval<R>&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>&& x, interval<R> const&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const&& x, interval<R>& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const&& x, interval<R> const& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const&& x, interval<R>&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const&& x, interval<R> const&& y )
  {
    return partial_less( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>& x, R&& y )
  {
    return partial_less( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const& x, R&& y )
  {
    return partial_less( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L>&& x, R&& y )
  {
    return partial_less( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( interval<L> const&& x, R&& y )
  {
    return partial_less( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( L&& x, interval<R>& y )
  {
    return partial_less( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( L&& x, interval<R> const& y )
  {
    return partial_less( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( L&& x, interval<R>&& y )
  {
    return partial_less( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<( L&& x, interval<R> const&& y )
  {
    return partial_less( std::forward<L>( x ), y );
  }

  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>& x, interval<R>& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>& x, interval<R> const& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>& x, interval<R>&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>& x, interval<R> const&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const& x, interval<R>& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const& x, interval<R> const& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const& x, interval<R>&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const& x, interval<R> const&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>&& x, interval<R>& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>&& x, interval<R> const& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>&& x, interval<R>&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>&& x, interval<R> const&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const&& x, interval<R>& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const&& x, interval<R> const& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const&& x, interval<R>&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const&& x, interval<R> const&& y )
  {
    return partial_less_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>& x, R&& y )
  {
    return partial_less_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const& x, R&& y )
  {
    return partial_less_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L>&& x, R&& y )
  {
    return partial_less_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( interval<L> const&& x, R&& y )
  {
    return partial_less_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( L&& x, interval<R>& y )
  {
    return partial_less_or_equal( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( L&& x, interval<R> const& y )
  {
    return partial_less_or_equal( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( L&& x, interval<R>&& y )
  {
    return partial_less_or_equal( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator<=( L&& x, interval<R> const&& y )
  {
    return partial_less_or_equal( std::forward<L>( x ), y );
  }


  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>& x, interval<R>& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>& x, interval<R> const& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>& x, interval<R>&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>& x, interval<R> const&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const& x, interval<R>& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const& x, interval<R> const& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const& x, interval<R>&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const& x, interval<R> const&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>&& x, interval<R>& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>&& x, interval<R> const& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>&& x, interval<R>&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>&& x, interval<R> const&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const&& x, interval<R>& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const&& x, interval<R> const& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const&& x, interval<R>&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const&& x, interval<R> const&& y )
  {
    return partial_greater( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>& x, R&& y )
  {
    return partial_greater( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const& x, R&& y )
  {
    return partial_greater( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L>&& x, R&& y )
  {
    return partial_greater( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( interval<L> const&& x, R&& y )
  {
    return partial_greater( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( L&& x, interval<R>& y )
  {
    return partial_greater( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( L&& x, interval<R> const& y )
  {
    return partial_greater( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( L&& x, interval<R>&& y )
  {
    return partial_greater( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>( L&& x, interval<R> const&& y )
  {
    return partial_greater( std::forward<L>( x ), y );
  }


  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>& x, interval<R>& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>& x, interval<R> const& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>& x, interval<R>&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>& x, interval<R> const&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const& x, interval<R>& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const& x, interval<R> const& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const& x, interval<R>&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const& x, interval<R> const&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>&& x, interval<R>& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>&& x, interval<R> const& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>&& x, interval<R>&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>&& x, interval<R> const&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const&& x, interval<R>& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const&& x, interval<R> const& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const&& x, interval<R>&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const&& x, interval<R> const&& y )
  {
    return partial_greater_or_equal( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>& x, R&& y )
  {
    return partial_greater_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const& x, R&& y )
  {
    return partial_greater_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L>&& x, R&& y )
  {
    return partial_greater_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( interval<L> const&& x, R&& y )
  {
    return partial_greater_or_equal( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( L&& x, interval<R>& y )
  {
    return partial_greater_or_equal( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( L&& x, interval<R> const& y )
  {
    return partial_greater_or_equal( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( L&& x, interval<R>&& y )
  {
    return partial_greater_or_equal( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator>=( L&& x, interval<R> const&& y )
  {
    return partial_greater_or_equal( std::forward<L>( x ), y );
  }


  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>& x, interval<R>& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>& x, interval<R> const& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>& x, interval<R>&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>& x, interval<R> const&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const& x, interval<R>& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const& x, interval<R> const& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const& x, interval<R>&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const& x, interval<R> const&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>&& x, interval<R>& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>&& x, interval<R> const& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>&& x, interval<R>&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>&& x, interval<R> const&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const&& x, interval<R>& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const&& x, interval<R> const& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const&& x, interval<R>&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const&& x, interval<R> const&& y )
  {
    return partial_unordered( x, y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>& x, R&& y )
  {
    return partial_unordered( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const& x, R&& y )
  {
    return partial_unordered( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L>&& x, R&& y )
  {
    return partial_unordered( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( interval<L> const&& x, R&& y )
  {
    return partial_unordered( x, std::forward<R>( y ) );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( L&& x, interval<R>& y )
  {
    return partial_unordered( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( L&& x, interval<R> const& y )
  {
    return partial_unordered( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( L&& x, interval<R>&& y )
  {
    return partial_unordered( std::forward<L>( x ), y );
  }
  template < typename L, typename R >
  inline constexpr bool operator!=( L&& x, interval<R> const&& y )
  {
    return partial_unordered( std::forward<L>( x ), y );
  }

} // ! partial_order
} // ! compare
} // ! cranberries
#endif
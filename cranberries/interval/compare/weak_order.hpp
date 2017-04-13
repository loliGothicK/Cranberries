#ifndef CRANBRIIES_INTERVAL_LIB_COMPARE_WEAK_ORDER_HPP
#define CRANBRIIES_INTERVAL_LIB_COMPARE_WEAK_ORDER_HPP

#include <utility>
#include <type_traits>
#include "../interval.hpp"
#include "explicit.hpp"

namespace cranberries {
namespace compare {
namespace weak_ordering_policy {

    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( interval<L>& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( interval<L> const& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( interval<L>&& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( interval<L> const&& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( L&& x, interval<R>& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( L&& x, interval<R> const& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( L&& x, interval<R>&& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<( L&& x, interval<R> const&& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( interval<L>& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( interval<L> const& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( interval<L>&& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<R>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( interval<L> const&& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R>& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R> const& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R>&& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<is_interval_v<L>, std::nullptr_t> = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R> const&& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>&& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const&& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>&& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R>& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R> const& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R>&& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R> const&& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>&& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const&& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
} // ! weak_order
} // ! compare
} // ! cranberries

#endif
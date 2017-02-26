#ifndef CRANBRIIES_INTERVAL_LIB_FOUR_ARITHMETIC_HPP
#define CRANBRIIES_INTERVAL_LIB_FOUR_ARITHMETIC_HPP

#include "interval.hpp"
#include "four_arithmetic_impl.hpp"
#include "cranberries_magic/detail.hpp"

namespace cranberries {
namespace normal_accurate_policy {

    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R>& y )
    {
      return add( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R> const& y )
    {
      return add( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R>&& y )
    {
      return add( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R> const&& y )
    {
      return add( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, R&& y )
    {
      return subtract( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, R&& y )
    {
      return subtract( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, R&& y )
    {
      return subtract( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, R&& y )
    {
      return subtract( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R>& y )
    {
      return subtract( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R> const& y )
    {
      return subtract( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R>&& y )
    {
      return subtract( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R> const&& y )
    {
      return subtract( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R>& y )
    {
      return multiply( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R> const& y )
    {
      return multiply( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R>&& y )
    {
      return multiply( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R> const&& y )
    {
      return multiply( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, R&& y )
    {
      return multiply( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, R&& y )
    {
      return multiply( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, R&& y )
    {
      return multiply( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, R&& y )
    {
      return multiply( x, std::forward<R>( y ) );
    }

    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::divide( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R>& y )
    {
      return divide( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R> const& y )
    {
      return divide( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R>&& y )
    {
      return divide( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R> const&& y )
    {
      return divide( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, R&& y )
    {
      return divide( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, R&& y )
    {
      return divide( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, R&& y )
    {
      return divide( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, R&& y )
    {
      return divide( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::add_assign( x, y );
    }

    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::subtract_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::subtract_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::subtract_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::subtract_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::multiply_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::multiply_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::multiply_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::multiply_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::divide_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::divide_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::divide_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::divide_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return add_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return add_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return add_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return add_assign( x, y );
    }

    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return subtract_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return subtract_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return subtract_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return subtract_assign( x, std::forward<L>( y ) );
    }

    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return multiply_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return multiply_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return multiply_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return multiply_assign( x, std::forward<L>( y ) );
    }

    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return divide_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return divide_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return divide_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return divide_assign( x, std::forward<L>( y ) );
    }
  }

  inline namespace high_accurate_policy
  {
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, R&& y )->decltype( add( x, y ) )
    {
      CRANBERRIES_ASSERT( !( cranberries_magic::concept::is_expr_v<L> || cranberries_magic::concept::is_expr_v<R> ) );
      return add( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, R&& y )->decltype( subtract( x, y ) )
    {
      CRANBERRIES_ASSERT( !( cranberries_magic::concept::is_expr_v<L> || cranberries_magic::concept::is_expr_v<R> ) );
      return subtract( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, R&& y )->decltype( multiply( x, y ) )
    {
      CRANBERRIES_ASSERT( !( cranberries_magic::concept::is_expr_v<L> || cranberries_magic::concept::is_expr_v<R> ) );
      return multiply( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, R&& y )->decltype( divide( x, y ) )
    {
      CRANBERRIES_ASSERT( !( cranberries_magic::concept::is_expr_v<L> || cranberries_magic::concept::is_expr_v<R> ) );
      return divide( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+=( interval<L>& x, R&& y )->decltype( add_assign( x, y ) )
    {
      return add_assign( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-=( interval<L>& x, R&& y )->decltype( subtract_assign( x, y ) )
    {
      return subtract_assign( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*=( interval<L>& x, R&& y )->decltype( multiply_assign( x, y ) )
    {
      return multiply_assign( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/=( interval<L>& x, R&& y )->decltype( divide_assign( x, y ) )
    {
      return divide_assign( x, std::forward<R>( y ) );
    }

  }

} // ! cranberries
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP
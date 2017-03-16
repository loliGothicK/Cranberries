#ifndef CRANBRIIES_INTERVAL_LIB_FOUR_ARITHMETIC_IMPL_HPP
#define CRANBRIIES_INTERVAL_LIB_FOUR_ARITHMETIC_IMPL_HPP

#include "interval.hpp"
#include "rounding_control.hpp"
#include "cranberries_magic/detail.hpp"
#include "constants.hpp"
#include "rounding_control.hpp"

namespace cranberries {
  /*  Interval Addition Op  */

  namespace normal_accuracy
  {
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> add( interval<L> const& x, interval<R> const& y )
    {
      auto x_low = x.lower();
      auto x_up = x.upper();
      auto y_low = y.lower();
      auto y_up = y.upper();

      return cranberries_magic::make_interval_with_accuracy_assurance( x_low + y_low, x_up + y_up );
    }

    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> subtract( interval<L> const& x, interval<R> const& y )
    {
      auto x_low = x.lower();
      auto x_up = x.upper();
      auto y_low = y.lower();
      auto y_up = y.upper();

      return cranberries_magic::make_interval_with_accuracy_assurance( x_low - y_up, x_up - y_low );
    }

    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> multiply( interval<L> const& x, interval<R> const& y )
    {
      auto x_lower = x.lower();
      auto x_upper = x.upper();
      auto y_lower = y.lower();
      auto y_upper = y.upper();

      return ( x_lower >= interval_constants::zero<T> && y_lower >= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_lower, x_upper * y_upper )
        : ( x_lower >= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_lower, x_upper * y_upper )
        : ( x_lower >= interval_constants::zero<T> && y_upper <= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_lower, x_lower * y_upper )
        : ( x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_lower >= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_upper, x_upper * y_upper )
        : ( x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_upper <= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_lower, x_lower * y_lower )
        : ( x_upper <= interval_constants::zero<T> && y_lower >= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_upper, x_upper * y_lower )
        : ( x_upper <= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_upper, x_lower * y_lower )
        : ( x_upper <= interval_constants::zero<T> && y_upper <= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_upper, x_lower * y_lower )
        : cranberries_magic::make_interval_with_accuracy_assurance(
        ( x_lower * y_upper < x_upper * y_lower ) ? cranberries_magic::downward_multiply( x_lower, y_upper ) : cranberries_magic::downward_multiply( x_upper, y_lower )
          , ( x_lower*y_lower < x_upper*y_upper ) ? cranberries_magic::upward_multiply( x_upper, y_upper ) : cranberries_magic::upward_multiply( x_lower, y_lower )
        );
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> divide( interval<L> const& x, interval<R> const& y )
    {
      return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( y.lower() <= interval_constants::zero<R> && y.upper() >= interval_constants::zero<R> )
        : normal_accuracy::multiply( x, cranberries_magic::make_interval_with_accuracy_assurance( interval_constants::one<R> / y.upper(), interval_constants::one<R> / y.lower() ) );
    }
    template < typename T, typename R >
    inline constexpr interval<T>& add_assign( interval<T>& x, interval<R> const& y )
    {
      return x = cranberries_magic::make_interval_with_accuracy_assurance( x.lower() + y.lower(), x.upper() + y.upper() );
    }

    template < typename T, typename R >
    inline constexpr interval<T>& subtract_assign( interval<T>& x, interval<R> const& y )
    {
      return x = cranberries_magic::make_interval_with_accuracy_assurance( x.lower() - y.upper(), x.upper() - y.lower() );
    }

    template < typename T, typename R >
    inline constexpr interval<T>& multiply_assign( interval<T>& x, interval<R> const& y )
    {
      auto x_lower = x.lower();
      auto x_upper = x.upper();
      auto y_lower = y.lower();
      auto y_upper = y.upper();

      return x = ( x_lower >= interval_constants::zero<T> && y_lower >= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_lower, x_upper * y_upper )
        : ( x_lower >= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_lower, x_upper * y_upper )
        : ( x_lower >= interval_constants::zero<T> && y_upper <= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_lower, x_lower * y_upper )
        : ( x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_lower >= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_upper, x_upper * y_upper )
        : ( x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_upper <= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_lower, x_lower * y_lower )
        : ( x_upper <= interval_constants::zero<T> && y_lower >= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_upper, x_upper * y_lower )
        : ( x_upper <= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_lower * y_upper, x_lower * y_lower )
        : ( x_upper <= interval_constants::zero<T> && y_upper <= interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x_upper * y_upper, x_lower * y_lower )
        : cranberries_magic::make_interval_with_accuracy_assurance(
        ( x_lower * y_upper < x_upper * y_lower ) ? cranberries_magic::downward_multiply( x_lower, y_upper ) : cranberries_magic::downward_multiply( x_upper, y_lower )
          , ( x_lower*y_lower < x_upper*y_upper ) ? cranberries_magic::upward_multiply( x_upper, y_upper ) : cranberries_magic::upward_multiply( x_lower, y_lower )
        );
    }
    template < typename T, typename R >
    inline constexpr interval<T>& divide_assign( interval<T>& x, interval<R> const& y )
    {
      CRANBERRIES_RANGE_ERROR_THROW_IF( y.lower() <= interval_constants::zero<R> && interval_constants::zero<R> <= y.upper() );
      return normal_accuracy::multiply_assign( x, cranberries_magic::make_interval_with_accuracy_assurance( interval_constants::one<R> / y.upper(), interval_constants::one<R> / y.lower() ) );
    }
  }

  inline namespace high_accuracy
  {
    template < typename T >
    inline constexpr interval<T> add( interval<T> const& x, interval<T> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> add( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename T >
    inline constexpr interval<T> subtract( interval<T> const& x, interval<T> const& y )
    {
      return ( &x == &y )
        ? interval<T>{}
      : normal_accuracy::subtract( x, y );
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    interval<T> subtract( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::subtract( x, y );
    }
    template < typename T >
    inline constexpr interval<T> multiply( interval<T> const& x, interval<T> const& y )
    {
      return ( &x == &y )
        ? pow( x, 2 )
        : normal_accuracy::multiply( x, y );
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> multiply( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::multiply( x, y );
    }
    template < typename T >
    inline constexpr interval<T> divide( interval<T> const& x, interval<T> const& y )
    {
      return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( y.lower() <= interval_constants::zero<T> && interval_constants::zero<T> <= y.upper() )
        : ( &x == &y )
        ? interval<T>{ interval_constants::one<T>, interval_constants::one<T> }
      : normal_accuracy::divide( x, y );
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline constexpr interval<T> divide( interval<L> const& x, interval<R> const& y )
    {
      return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( y.lower() <= interval_constants::zero<R> && interval_constants::zero<R> <= y.upper() )
        : normal_accuracy::multiply( x, cranberries_magic::make_interval_with_accuracy_assurance( interval_constants::one<R> / y.upper(), interval_constants::one<R> / y.lower() ) );
    }

    template < typename L, typename R >
    inline constexpr interval<L>& add_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::add_assign( x, y );
    }

    template < typename L, typename R >
    inline constexpr interval<L>& subtract_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::subtract_assign( x, y );
    }

    template < typename T >
    inline constexpr interval<T>& subtract_assign( interval<T>& x, interval<T> const& y )
    {
      return ( &x == &y )
        ? ( x = {} )
        : normal_accuracy::subtract_assign( x, y );
    }

    template < typename T >
    inline constexpr interval<T>& multiply_assign( interval<T>& x, interval<T> const& y )
    {
      using math_impl::pow_down; using math_impl::pow_up;
      if ( &x == &y )
        return x = ( x.lower() > interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( pow_down( x.lower(), 2 ), pow_up( x.upper(), 2 ) )
        : ( x.upper() < interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( pow_down( x.upper(), 2 ), pow_up( x.lower(), 2 ) )
        : cranberries_magic::make_interval_with_accuracy_assurance(interval_constants::zero<T>, std::max({ pow_up(x.lower(), 2), pow_up(x.upper(), 2) }));
      else
        return normal_accuracy::multiply_assign( x, y );
    }
    template < typename L, typename R >
    inline constexpr interval<L>& multiply_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::multiply_assign( x, y );
    }

    template < typename L, typename R >
    inline constexpr interval<L>& divide_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::divide_assign( x, y );
    }
    template < typename T >
    inline constexpr interval<T>& divide_assign( interval<T>& x, interval<T> const& y )
    {
      CRANBERRIES_OVERFLOW_ERROR_THROW_IF( y.lower() <= interval_constants::zero<T> && interval_constants::zero<T> <= y.upper() );
      return ( &x == &y ) ? x = { interval_constants::one<T> } : normal_accuracy::divide_assign( x, y );
    }
  }

  inline namespace
  {
    template < typename T >
    inline constexpr interval<T> subtract( typename interval<T>::value_type const& x, interval<T> const& y )
    {
      return cranberries_magic::make_interval_with_accuracy_assurance( x - y.upper(), x - y.lower() );
    }

    template < typename T >
    inline constexpr interval<T> subtract( interval<T> const& x, typename interval<T>::value_type const& y )
    {
      return cranberries_magic::make_interval_with_accuracy_assurance( x.lower() - y, x.upper() - y );
    }

    template < typename T >
    inline constexpr interval<T> add( typename interval<T>::value_type const& x, interval<T> const& y )
    {
      return cranberries_magic::make_interval_with_accuracy_assurance( x + y.lower(), x + y.upper() );
    }

    template < typename T >
    inline constexpr interval<T> add( interval<T> const& x, typename interval<T>::value_type const& y )
    {
      return cranberries_magic::make_interval_with_accuracy_assurance( x.lower() + y, x.upper() + y );
    }

    template < typename T >
    inline constexpr interval<T> multiply( typename interval<T>::value_type const& x, interval<T> const& y )
    {
      return ( x < interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x * y.upper(), x * y.lower() )
        : cranberries_magic::make_interval_with_accuracy_assurance( x * y.lower(), x * y.upper() );
    }

    template < typename T >
    inline constexpr interval<T> multiply( interval<T> const& x, typename interval<T>::value_type const& y )
    {
      return  ( y < interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x.upper() * y, x.lower() * y )
        : cranberries_magic::make_interval_with_accuracy_assurance( x.lower() * y, x.upper() * y );
    }

    template < typename T >
    inline constexpr interval<T> divide( interval<T> const& x, typename interval<T>::value_type const& y )
    {
      return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( y == interval_constants::zero<T> )
        : ( y < interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x.upper() / y, x.lower() / y )
        : cranberries_magic::make_interval_with_accuracy_assurance( x.lower() / y, x.upper() / y );
    }

    template < typename T >
    inline constexpr interval<T> divide( typename interval<T>::value_type const& x, interval<T> const& y )
    {
      return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( y.lower() < interval_constants::zero<T> && interval_constants::zero<T> <= y.upper() )
        : ( y.lower() > interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x / y.upper(), x / y.lower() )
        : cranberries_magic::make_interval_with_accuracy_assurance( x / y.lower(), x / y.upper() );
    }

    template < typename T >
    inline constexpr interval<T>& add_assign( interval<T>& x, typename interval<T>::value_type const& y )
    {
      return x = cranberries_magic::make_interval_with_accuracy_assurance( x.lower() + y, x.upper() + y );
    }

    template < typename T >
    inline constexpr interval<T>& subtract_assign( interval<T>& x, typename interval<T>::value_type const& y )
    {
      return x = cranberries_magic::make_interval_with_accuracy_assurance( x.lower() - y, x.upper() - y );
    }

    template < typename T >
    inline constexpr interval<T>& multiply_assign( interval<T>& x, typename interval<T>::value_type const& y )
    {
      return x = ( y < interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x.upper() * y, x.lower() * y )
        : cranberries_magic::make_interval_with_accuracy_assurance( x.lower() * y, x.upper() * y );
    }
    template < typename T >
    inline constexpr interval<T>& divide_assign( interval<T>& x, typename interval<T>::value_type const& y )
    {
      return x = CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( y == interval_constants::zero<T> )
        : ( y < interval_constants::zero<T> )
        ? cranberries_magic::make_interval_with_accuracy_assurance( x.upper() / y, x.lower() / y )
        : cranberries_magic::make_interval_with_accuracy_assurance( x.lower() / y, x.upper() / y );
    }
  }


} // ! cranberries
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP

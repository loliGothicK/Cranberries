/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBRIIES_INTERVAL_LIB_ARITHMETIC2_HPP
#define CRANBRIIES_INTERVAL_LIB_ARITHMETIC2_HPP

#include <cmath>
#include "interval.hpp"
#include "constants.hpp"
#include "rounding_control.hpp"

namespace cranberries {
  /*  interval cosine  */

  template < typename T >
  inline constexpr interval<T> cos( interval<T> const& x ) noexcept
  {
    using math_impl::cos_down;
    using math_impl::cos_up;
    auto a = x.lower();
    auto b = x.upper();

    if ( b - a >= static_cast<T>( 2.0L ) * interval_constants::pi<T> )
      return make_interval([] {return-1.0; }, [] {return 1.0; });

    /*  base point reset  */
    auto pi = interval_constants::pi<T>;
    DOWNWARD_POLICY;
    auto base1 = std::ceil( a / ( 2. * pi ) );
    auto base2 = std::ceil( a / ( 2. * pi ) - 0.5 );
    auto x1 = a / ( 2. * pi );
    auto y1 = a / ( 2. * pi ) - 0.5;
    DOWNWARD_POLICY;
    auto x2 = b / ( 2. * pi );
    auto y2 = b / ( 2. * pi ) - 0.5;
    /*  checking phase  */
    return ( x1 < base1 && base1 < x2 )
      ? (
      (y1 < base2 && base2 < y2) ? make_interval([] { return -1.0; }, [] { return 1.0; })
    : [&] {
      DOWNWARD_POLICY;
      auto l = static_cast<T>( cos_down( a ) );
      auto r = static_cast<T>( cos_down( b ) );
      return make_interval([&] { return std::fmin(l, r); }, [] { return 1.0; });
    }( ) )
      : ( y1 < base2 && base2 < y2 ) ? [&] {
      UPWARD_POLICY;
      auto l = static_cast<T>( cos_up( a ) );
      auto r = static_cast<T>( cos_up( b ) );
      return make_interval([] { return -1.0; }, [&] {return std::fmax(l, r); });
    }()
      : (cos_up(a) < cos_up(b))
      ? make_interval([&] {return static_cast<T>(cos_down(a)); }, [&] {return static_cast<T>(cos_up(b)); })
      : make_interval([&] {return static_cast<T>(cos_down(b)); }, [&] {return static_cast<T>(cos_up(a)); });
  }

  /*  interval sine  */

  template < typename T >
  inline constexpr interval<T> sin( interval<T> const& x ) noexcept
  {
    using math_impl::sin_up;
    using math_impl::sin_down;
    auto a = x.lower();
    auto b = x.upper();

    if ( b - a >= static_cast<T>( 2.0L ) * interval_constants::pi<T> )
      return make_interval([] {return -1.0; }, [] {return 1.0; });

    /*  base point reset  */
    auto pi = interval_constants::pi<T>;
    DOWNWARD_POLICY;
    auto base1 = std::ceil( a / ( 2. * pi ) - 0.25 );
    auto base2 = std::ceil( a / ( 2. * pi ) - 0.75 );
    auto x1 = a / ( 2. * pi ) - 0.25;
    auto y1 = a / ( 2. * pi ) - 0.75;
    DOWNWARD_POLICY;
    auto x2 = b / ( 2. * pi ) - 0.25;
    auto y2 = b / ( 2. * pi ) - 0.75;
    /*  checking phase  */
    return ( x1 < base1 && base1 < x2 )
      ? (
      (y1 < base2 && base2 < y2) ? make_interval([] {return -1.0; }, [] {return 1.0; })
    : [&] {
      DOWNWARD_POLICY;
      auto l = static_cast<T>( sin_down( a ) );
      auto r = static_cast<T>( sin_down( b ) );
      return make_interval([&] {return std::fmin(l, r); }, [] { return 1.0; });
    }( ) )
      : ( y1 < base2 && base2 < y2 ) ? [&] {
      UPWARD_POLICY;
      auto l = static_cast<T>( sin_up( a ) );
      auto r = static_cast<T>( sin_up( b ) );
      return make_interval([] { return -1.0; }, [&] {return std::fmax(l, r); });
    }( )
      : ( sin_up( a ) < sin_up( b ) )
      ? make_interval([&] {return static_cast<T>(sin_down(a)); }, [&] {return static_cast<T>(sin_up(b)); })
      : make_interval([&] {return static_cast<T>(sin_down(b)); }, [&] {return static_cast<T>(sin_up(a)); });
  }

  /*  interval tangent  */

  template < typename T >
  inline constexpr interval<T> tan( interval<T> const& x )
  {
    using math_impl::sin_up; using math_impl::sin_down;
    using math_impl::cos_up; using math_impl::cos_down;
    auto a = x.lower();
    auto b = x.upper();

    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( b - a >= interval_constants::pi<T> )
      : CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( static_cast<int>( b * 2 / interval_constants::pi<T> ) - static_cast<int>( a * 2 / interval_constants::pi<T> ) != 0 )
      : CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( cranberries_magic::is_overflow( sin_down( a ) / cos_up( a ), sin_up( b ) / cos_down( b ) ) )
      : make_interval([&] {return sin_down(a) / cos_up(a); }, [&] {return sin_up(b) / cos_down(b); });
  }

  template < typename T >
  interval<T> sec( interval<T> const& x )
  {
    return cos(x).inverse();
  }

  template < typename T >
  interval<T> csc( interval<T> const& x )
  {
    return sin(x).inverse();
  }

  template < typename T >
  interval<T> cot( interval<T> const& x )
  {
    return tan(x).inverse();
  }


  /*  interval arc cosine  */

  template < typename T >
  inline constexpr interval<T> acos( interval<T> const& x )
  {
    using std::acos;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( x.lower() < -1.0 || 1.0 < x.upper() )
      : make_interval([&] {return acos(x.lower()); }, [&] {return acos(x.upper()); });
  }

  /*  interval arc sine  */

  template < typename T >
  inline constexpr interval<T> asin( interval<T> const& x )
  {
    using std::asin;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( x.lower() < -1.0 || 1.0 < x.upper() )
      : make_interval([&] {return asin(x.lower()); }, [&] {return asin(x.upper()); });
  }

  /*  interval arc tangent  */

  template < typename T >
  inline constexpr interval<T> atan( interval<T> const& x )
  {
    return make_interval([&] {return std::atan(x.lower()); }, [&] {return std::atan(x.upper()); });
  }

  template < typename T >
  interval<T> asec( interval<T> const& x )
  {
    using std::acos;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( !( x.upper() < -1.0 || 1.0 < x.lower() ) )
      : make_interval([&] { return acos(1.0 / x.lower()); }, [&] {return acos(1.0 / x.upper()); });
  }

  template < typename T >
  interval<T> acsc( interval<T> const& x )
  {
    using std::asin;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( !( x.upper() < -1.0 || 1.0 < x.lower() ) )
      : make_interval([&] {return asin(1.0 / x.upper()); }, [&] {return asin(1.0 / x.lower()); });
  }

  template < typename T >
  interval<T> acot( interval<T> const& x )
  {
    using std::atan;
    using std::abs;
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( x.lower() == interval_constants::zero<T> || x.upper() == interval_constants::zero<T> )
      : ( abs( x.lower() ) < abs( x.upper() ) )
      ? make_interval([&] {return atan(1.0 / x.upper()); }, [&] {return atan(1.0 / x.lower()); })
      : make_interval([&] {return atan(1.0 / x.lower()); }, [&] {return atan(1.0 / x.upper()); });
  }

  /*  interval hyperbolic cosine  */

  template < typename T >
  inline constexpr interval<T> cosh( interval<T> const& x )
  {
    using std::cosh;
    auto a = x.lower();
    auto b = x.upper();
    return (b < interval_constants::zero<T>) ? make_interval([&] {return cosh(b); }, [&] {return cosh(a); })
      : (interval_constants::zero<T> <= a) ? make_interval([&] {return cosh(a); }, [&] {return cosh(b); })
      : [&] {
      UPWARD_POLICY;
      auto l = cosh( a );
      auto r = cosh( b );
      return make_interval([] { return 1.0; }, [&] {return std::fmax(l, r); });
    }( );
  }

  /*  interval hyperbolic sine  */

  template < typename T >
  inline constexpr interval<T> sinh( interval<T> const& x )
  {
    using std::sinh;
    return make_interval([&] {return sinh(x.lower()); }, [&] {return sinh(x.upper()); });
  }

  /*  interval hyperbolic tangent  */

  template < typename T >
  inline constexpr interval<T> tanh( interval<T> const& x ) noexcept
  {
    using std::tanh;
    return make_interval([&] {return tanh(x.lower()); }, [&] {return tanh(x.upper()); });
  }

  template < typename T >
  inline constexpr interval<T> sech( interval<T> const& x ) noexcept
  {
    return cosh(x).inverse();
  }

  template < typename T >
  inline constexpr interval<T> csch( interval<T> const& x )
  {
    return sinh(x).inverse();
  }

  template < typename T >
  inline constexpr interval<T> coth( interval<T> const& x )
  {
    return tanh(x).inverse();
  }

  /*  interval arc hyperbolic cosine  */

  template < typename T >
  inline constexpr interval<T> acosh( interval<T> const& x )
  {
    using std::acosh;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( x.lower() < 1.0 )
      : make_interval([&] {return acosh(x.lower()); }, [&] {return acosh(x.upper()); });
  }

  /*  interval arc hyperbolic sine  */

  template < typename T >
  inline constexpr interval<T> asinh( interval<T> const& x ) noexcept
  {
    using std::asinh;
    return make_interval([&] {return asinh(x.lower()); }, [&] {return asinh(x.upper()); });
  }

  /*  interval arc hyperbolic tangent  */

  template < typename T >
  inline interval<T> atanh( interval<T> const& x )
  {
    using std::atanh;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( x.lower() < -1.0 || 1.0 < x.upper() )
      : make_interval([&] {return atanh(x.lower()); }, [&] {return atanh(x.upper()); });
  }

  template < typename T >
  inline interval<T> asech( interval<T> const& x )
  {
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( !( 0.0 < x.lower() && x.upper() <= 1.0 ) )
      : acosh( x.inverse() );
  }

  template < typename T >
  inline constexpr interval<T> acsch( interval<T> const& x )
  {
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( !( x.upper() < -1.0 || 1.0 < x.lower() ) )
      : asinh( x.inverse() );
  }

  template < typename T >
  inline constexpr interval<T> acoth( interval<T> const& x )
  {
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( !( x.upper() < -1.0 || 1.0 < x.lower() ) )
      : atanh( x.inverse() );
  }

  /*  interval power function  */

  template < typename T >
  inline constexpr interval<T> pow( interval<T> const& x, long double n )
  {
    using math_impl::pow_up;
    using math_impl::pow_down;
    using std::pow;
    auto a = x.lower();
    auto b = x.upper();
    return ( n < 0 ) ? pow( x.inverse(), -n )
      : ( fmodl( n, 1 ) == 0 )
      ? (
      ( n == 0.0 )
        ? make_interval([] { return 1.0; }, [] { return 1.0; })
    : ( a <= 0.0 && 0.0 <= b )
      ? [&] {
      T l = pow_up( a, static_cast< std::size_t >( n ) );
      T r = pow_up( b, static_cast< std::size_t >( n ) );
      return make_interval([] { return 0.0; }, [&] {return std::fmax(l, r); });
    }( )
      : [&] {
      T l_min = pow_down( a, static_cast< std::size_t >( n ) );
      T r_min = pow_down( b, static_cast< std::size_t >( n ) );
      T l_max = pow_up( a, static_cast< std::size_t >( n ) );
      T r_max = pow_up( b, static_cast< std::size_t >( n ) );
      return make_interval([&] {return std::fmin(l_min, r_min); }, [&] {return std::fmax(l_max, r_max); });
    }( )
      )
      : CRANBERRIES_RANGE_ERROR_THROW_CONDITIONAL( a < 0.0 )
      : ( a <= 0.0 && 0.0 <= b )
      ? [&] {
      UPWARD_POLICY;
      T l = pow( a, n );
      T r = pow( b, n );
      return make_interval([] {return 0; }, [&] {return std::fmax(l, r); });
    }( )
      : [&] {
      DOWNWARD_POLICY;
      T l_min = pow( a, n );
      T r_min = pow( b, n );
      UPWARD_POLICY;
      T l_max = pow( a, n );
      T r_max = pow( b, n );
      return make_interval([&] {return std::fmin(l_min, r_min); }, [&] {return std::fmax(l_max, r_max); });
    }( );
  }

  /*  interval square root function  */

  template < typename T >
  inline constexpr interval<T> sqrt( interval<T> const& x )
  {
    using std::sqrt;
    return CRANBERRIES_DOMAIN_ERROR_THROW_CONDITIONAL( x.lower() < interval_constants::zero<T> )
      : make_interval([&] {return sqrt(x.lower()); }, [&] {return sqrt(x.upper()); });
  }

  template < typename T >
  inline constexpr interval<T> cbrt( interval<T> const& x ) noexcept
  {
    using std::cbrt;
    return make_interval([&] {return cbrt(x.lower()); }, [&] {return cbrt(x.upper()); });
  }

  /*  interval expconstants::onential function ( base = e )  */

  template < typename T >
  inline constexpr interval<T> exp( interval<T> const& x )
  {
    using std::exp;
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( cranberries_magic::is_overflow( exp( x.lower() ), exp( x.upper() ) ) )
      : make_interval([&] {return exp(x.lower()); }, [&] {return exp(x.upper()); });
  }

  /*  interval expconstants::onential function ( base = 2 )  */

  template < typename T >
  inline constexpr interval<T> exp2( interval<T> const& x )
  {
    using std::exp2;
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL( cranberries_magic::is_overflow( exp2( x.lower() ), exp2( x.upper() ) ) )
      : make_interval([&] {return exp2(x.lower()); }, [&] {return exp2(x.upper()); });
  }

  /*  interval exp( x ) - 1  */

  template < typename T >
  inline constexpr interval<T> expm1( interval<T> const& x )
  {
    using std::expm1;
    return cranberries_magic::is_overflow( expm1( x.lower() ), expm1( x.upper() ) )
      ? CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG( "Overflow has occurred." )
      : make_interval([&] {return expm1(x.lower()); }, [&] {return expm1(x.upper()); });
  }

  /*  interval logarithmic function ( base = e ) */

  template < typename T >
  inline constexpr interval<T> log( interval<T> const& x )
  {
    using std::log;
    return interval_constants::zero<T> < x.lower()
      ? make_interval([&] {return log(x.lower()); }, [&] {return log(x.upper()); })
      : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval log( x ) + 1  */
  template < typename T >
  inline constexpr interval<T> log1p( interval<T> const& x )
  {
    using std::log1p;
    return -1.0 < x.lower()
      ? make_interval([&] {return log1p(x.lower()); }, [&] {return log1p(x.upper()); })
      : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval logarithmic function ( base = 10 )  */

  template < typename T >
  inline constexpr interval<T> log10( interval<T> const& x )
  {
    using std::log10;
    return interval_constants::zero<T> < x.lower()
      ? make_interval([&] {return log10(x.lower()); }, [&] {return log10(x.upper()); })
      : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval logarithmic function ( base = 2 )  */
  template < typename T >
  inline constexpr interval<T> log2( interval<T> const& x )
  {
    using std::log2;
    return interval_constants::zero<T> < x.lower()
      ? make_interval([&] {return log2(x.lower()); }, [&] {return log2(x.upper()); })
      : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval absolute funtion  */

  template < typename T >
  inline constexpr interval<T> abs( interval<T> const& x )
  {
    using std::abs;
    auto low = x.lower();
    auto up = x.upper();
    return low < -interval_constants::zero<T> && interval_constants::zero<T> <= up 
      ? make_interval([] {return interval_constants::zero<T>; }, [&] {return std::fmax(abs(low), abs(up)); })
      : up < interval_constants::zero<T>
      ? make_interval([&] {return abs(up); }, [&] {return abs(low); })
        : interval<T>{ x };
  }


  /*  interval fused multiply-add function fma( interval<T>, interval<T>, interval<T>)  */
  template < typename T1, typename T2, typename T3, typename T = std::common_type_t<T1, std::common_type_t<T2, T3>> >
  inline constexpr interval<T> fma( interval<T1> const& x, interval<T2> const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    auto x_low = x.lower();
    auto y_low = y.lower();
    auto z_low = z.lower();
    auto x_up = x.upper();
    auto y_up = y.upper();
    auto z_up = z.upper();

    return x_low >= interval_constants::zero<T1> && y_low >= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_low, y_low, z_low); }, [&] {return fma(x_up, y_up, z_up); })
      : x_low >= interval_constants::zero<T1> && y_low < interval_constants::zero<T2> && y_up > interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_low, z_low); }, [&] {return fma(x_low, y_up, z_up); })
      : x_low >= interval_constants::zero<T1> && y_up <= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_low, z_low); }, [&] {return fma(x_low, y_up, z_up); })
      : x_low < interval_constants::zero<T1> && x_up > interval_constants::zero<T1> && y_low >= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_low, y_up, z_low); }, [&] {return fma(x_up, y_up, z_up); })
      : x_low < interval_constants::zero<T1> && x_up > interval_constants::zero<T1> && y_low <= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_low, z_low); }, [&] {return fma(x_low, y_low, z_up); })
      : x_up <= interval_constants::zero<T1> && y_low >= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_low, y_up, z_low); }, [&] {return fma(x_up, y_low, z_up); })
      : x_up <= interval_constants::zero<T> && y_low < interval_constants::zero<T> && y_up > interval_constants::zero<T>
        ? make_interval([&] {return fma(x_low, y_up, z_low); }, [&] {return fma(x_low, y_low, z_up); })
      : x_up <= interval_constants::zero<T> && y_up <= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_up, z_low); }, [&] {return fma(x_low, y_low, z_up); })
      : x_up*y_low < x_low*y_up
        ? make_interval([&] {return fma(x_up, y_low, z_low); }, [&] {return fma(x_low, y_up, z_up); })
        : make_interval([&] {return fma(x_low, y_up, z_low); }, [&] {return fma(x_up, y_low, z_up); });
  }

  /*  interval fused multiplyply-add function fma( interval<T>, interval<T>, T)  */

  template < typename T1, typename T2, typename T = std::common_type_t<T1, T2> >
  inline constexpr interval<T> fma( interval<T1> const& x, interval<T2> const& y, T const& z ) noexcept
  {
    using std::fma;
    auto x_low = x.lower();
    auto y_low = y.lower();
    auto y_up = y.upper();
    auto x_up = x.upper();

    return x_low >= interval_constants::zero<T1> && y_low >= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_low, y_low, z); }, [&] {return fma(x_up, y_up, z); })
      : x_low >= interval_constants::zero<T1> && y_low < interval_constants::zero<T2> && y_up > interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_low, z); }, [&] {return fma(x_low, y_up, z); })
      : x_low >= interval_constants::zero<T1> && y_up <= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_low, z); }, [&] {return fma(x_low, y_up, z); })
      : x_low < interval_constants::zero<T1> && x_up > interval_constants::zero<T1> && y_low >= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_low, y_up, z); }, [&] {return fma(x_up, y_up, z); })
      : x_low < interval_constants::zero<T1> && x_up > interval_constants::zero<T1> && y_low <= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_low, z); }, [&] {return fma(x_low, y_low, z); })
      : x_up <= interval_constants::zero<T1> && y_low >= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_low, y_up, z); }, [&] {return fma(x_up, y_low, z); })
      : x_up <= interval_constants::zero<T> && y_low < interval_constants::zero<T> && y_up > interval_constants::zero<T>
        ? make_interval([&] {return fma(x_low, y_up, z); }, [&] {return fma(x_low, y_low, z); })
      : x_up <= interval_constants::zero<T> && y_up <= interval_constants::zero<T2>
        ? make_interval([&] {return fma(x_up, y_up, z); }, [&] {return fma(x_low, y_low, z); })
      : x_up*y_low < x_low*y_up
        ? make_interval([&] {return fma(x_up, y_low, z); }, [&] {return fma(x_low, y_up, z); })
        : make_interval([&] {return fma(x_low, y_up, z); }, [&] {return fma(x_up, y_low, z); });
  }

  /*  interval fused multiplyply-add function fma( interval<T>, T, interval<T>)  */

  template < typename T1, typename T3, typename T = std::common_type_t<T1, T3> >
  inline constexpr interval<T> fma( interval<T1> const& x, T const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    return y < interval_constants::zero<T>
      ? make_interval([&] {return fma(x.upper(), y, z.lower()); }, [&] {return fma(x.lower(), y, z.upper()); })
      : make_interval([&] {return fma(x.lower(), y, z.lower()); }, [&] {return fma(x.upper(), y, z.upper()); });
  }

  /*  interval fused multiplyply-add function fma( T, interval<T>, interval<T>)  */

  template < typename T2, typename T3, typename T = std::common_type_t<T2, T3> >
  inline constexpr interval<T> fma( T const& x, interval<T2> const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    return x < interval_constants::zero<T>
      ? make_interval([&] {return fma(x, y.upper(), z.lower()); }, [&] {return fma(x, y.lower(), z.upper()); })
      : make_interval([&] {return fma(x, y.lower(), z.lower()); }, [&] {return fma(x, y.upper(), z.upper()); });
  }

  /*  interval fused multiplyply-add function fma( interval<T>, T, T)  */

  template < typename T >
  inline constexpr interval<T> fma( interval<T> const& x, typename interval<T>::value_type const& y, typename interval<T>::value_type const& z ) noexcept
  {
    using std::fma;
    return y < interval_constants::zero<T>
      ? make_interval([&] {return fma(x.upper(), y, z); }, [&] {return fma(x.lower(), y, z); })
      : make_interval([&] {return fma(x.lower(), y, z); }, [&] {return fma(x.upper(), y, z); });
  }

  /*  interval fused multiplyply-add function fma( T, interval<T>, T)  */

  template < typename T >
  inline constexpr interval<T> fma( typename interval<T>::value_type const& x, interval<T> const& y, typename interval<T>::value_type const& z ) noexcept
  {
    using std::fma;
    return x < interval_constants::zero<T>
      ? make_interval([&] {return fma(x, y.upper(), z); }, [&] {return fma(x, y.lower(), z); })
      : make_interval( fma( x, y.lower(), z ), fma( x, y.upper(), z ) );
  }

  /*  interval fused multiplyply-add function fma( T, T, interval<T>)  */

  template < typename T >
  inline constexpr interval<T> fma( typename interval<T>::value_type const& x, typename interval<T>::value_type const& y, interval<T> const& z ) noexcept
  {
    using std::fma;
    return make_interval([&] {return fma(x, y, z.lower()); }, [&] {return fma(x, y, z.upper()); });
  }

  /*  interval error function  */

  template < typename T >
  inline constexpr interval<T> erf( interval<T> const& x ) noexcept
  {
    using std::erf;
    return make_interval([&] {return erf(x.lower()); }, [&] {return erf(x.upper()); });
  }

  /*  interval complementary error function  */

  template < typename T >
  inline constexpr interval<T> erfc( interval<T> const& x ) noexcept
  {
    using std::erfc;
    return make_interval([&] {return erfc(x.upper()); }, [&] {return erfc(x.lower()); });
  }

  /*  is_singleton  */

  template < typename T >
  inline constexpr bool is_singleton( interval<T> const& x ) noexcept
  {
    return ( x.lower() == x.upper() ) ? true : false;
  }

  /*  Interval Accessors  */

  template < typename T >
  inline constexpr T interval<T>::lower() const& noexcept
  {
    return lower_;
  }

  template < typename T >
  inline constexpr T interval<T>::upper() const& noexcept
  {
    return upper_;
  }

  template < typename T >
  inline constexpr T& interval<T>::lower() & noexcept
  {
    return lower_;
  }

  template < typename T >
  inline constexpr T& interval<T>::upper() & noexcept
  {
    return upper_;
  }

  template < typename T >
  inline constexpr T&& interval<T>::lower() && noexcept
  {
    return lower_;
  }

  template < typename T >
  inline constexpr T&& interval<T>::upper() && noexcept
  {
    return upper_;
  }

  template < typename T >
  inline constexpr void interval<T>::swap( interval<T>& x ) noexcept
  {
    auto tmp = std::move(*this);
    *this = std::move(x);
    x = std::move(tmp);
  }

  template < typename T >
  inline constexpr void swap( interval<T>& x, interval<T>& y ) noexcept
  {
    x.swap( y );
  }


} // ! cranberries
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC2_HPP

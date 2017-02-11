#ifndef CRANBRIIES_INTERVAL_LIB_ARITHMETIC2_HPP
#define CRANBRIIES_INTERVAL_LIB_ARITHMETIC2_HPP

#include <cmath>
#include "interval.hpp"
#include "constants.hpp"

namespace cranberries {
namespace interval_lib
{
  /*  interval cosine  */

  template < typename T >
  inline interval<T> cos( interval<T> const& x ) noexcept
  {
    using math_impl::cos_down;
    using math_impl::cos_up;
    auto&& a = x.lower();
    auto&& b = x.upper();

    if ( b - a >= static_cast<T>( 2.0L ) * constants::pi<T> )
      return interval<T>{ -constants::one<T>, constants::one<T> };

    /*  base point reset  */
    auto pi = constants::pi<T>;
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
      ( y1 < base2 && base2 < y2 ) ? interval<T>{ -constants::one<T>, constants::one<T> }
    : [&] {
      DOWNWARD_POLICY;
      auto&& l = static_cast<T>( cos_down( a ) );
      auto&& r = static_cast<T>( cos_down( b ) );
      return interval<T>{ std::fmin( l, r ), constants::one<T> };
    }( ) )
      : ( y1 < base2 && base2 < y2 ) ? [&] {
      UPWARD_POLICY;
      auto&& l = static_cast<T>( cos_up( a ) );
      auto&& r = static_cast<T>( cos_up( b ) );
      return interval<T>{ -constants::one<T>, std::fmax( l, r ) };
    }( )
      : ( cos_up( a ) < cos_up( b ) )
      ? ACCURACY_ASSURANCE( static_cast<T>( cos_down( a ) ), static_cast<T>( cos_up( b ) ) )
      : ACCURACY_ASSURANCE( static_cast<T>( cos_down( b ) ), static_cast<T>( cos_up( a ) ) );
  }

  /*  interval sine  */

  template < typename T >
  inline interval<T> sin( interval<T> const& x ) noexcept
  {
    using math_impl::sin_up;
    using math_impl::sin_down;
    auto a = x.lower();
    auto b = x.upper();

    if ( b - a >= static_cast<T>( 2.0L ) * constants::pi<T> )
      return interval<T>{ -constants::one<T>, constants::one<T> };

    /*  base point reset  */
    auto pi = constants::pi<T>;
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
      ( y1 < base2 && base2 < y2 ) ? interval<T>{ -constants::one<T>, constants::one<T> }
    : [&] {
      DOWNWARD_POLICY;
      auto&& l = static_cast<T>( sin_down( a ) );
      auto&& r = static_cast<T>( sin_down( b ) );
      return interval<T>{ std::fmin( l, r ), constants::one<T> };
    }( ) )
      : ( y1 < base2 && base2 < y2 ) ? [&] {
      UPWARD_POLICY;
      auto&& l = static_cast<T>( sin_up( a ) );
      auto&& r = static_cast<T>( sin_up( b ) );
      return interval<T>{ -constants::one<T>, std::fmax( l, r ) };
    }( )
      : ( sin_up( a ) < sin_up( b ) )
      ? ACCURACY_ASSURANCE( static_cast<T>( sin_down( a ) ), static_cast<T>( sin_up( b ) ) )
      : ACCURACY_ASSURANCE( static_cast<T>( sin_down( b ) ), static_cast<T>( sin_up( a ) ) );
  }

  /*  interval tangent  */

  template < typename T >
  inline interval<T> tan( interval<T> const& x )
  {
    using math_impl::sin_up; using math_impl::sin_down;
    using math_impl::cos_up; using math_impl::cos_down;
    auto&& a = x.lower();
    auto&& b = x.upper();

    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( b - a >= constants::pi<T> );
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( static_cast<int>( b * 2 / constants::pi<T> ) - static_cast<int>( a * 2 / constants::pi<T> ) != 0 );
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( detail::is_overflow( sin_down( a ) / cos_up( a ), sin_up( b ) / cos_down( b ) ) );
    return ACCURACY_ASSURANCE( sin_down( a ) / cos_up( a ), sin_up( b ) / cos_down( b ) );
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
  inline interval<T> acos( interval<T> const& x )
  {
    using std::acos;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.lower() < -constants::one<T> || constants::one<T> < x.upper() );
    return ACCURACY_ASSURANCE( acos( x.lower() ), acos( x.upper() ) );
  }

  /*  interval arc sine  */

  template < typename T >
  inline interval<T> asin( interval<T> const& x )
  {
    using std::asin;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.lower() < -constants::one<T> || constants::one<T> < x.upper() );
    return ACCURACY_ASSURANCE( asin( x.lower() ), asin( x.upper() ) );
  }

  /*  interval arc tangent  */

  template < typename T >
  inline interval<T> atan( interval<T> const& x ) noexcept
  {
    using std::atan;
    return ACCURACY_ASSURANCE( atan( x.lower() ), atan( x.upper() ) );
  }

  template < typename T >
  interval<T> asec( interval<T> const& x )
  {
    using std::acos;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.upper() < -1.0 || 1.0 < x.lower() ) );
    return ACCURACY_ASSURANCE( acos( 1.0 / x.lower() ), acos( 1.0 / x.upper() ) );
  }

  template < typename T >
  interval<T> acsc( interval<T> const& x )
  {
    using std::asin;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.upper() < -1.0 || 1.0 < x.lower() ) );
    return ACCURACY_ASSURANCE( asin( 1.0 / x.upper() ), asin( 1.0 / x.lower() ) );
  }

  template < typename T >
  interval<T> acot( interval<T> const& x )
  {
    using std::atan;
    using std::abs;
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( x.lower() == constants::zero<T> || x.upper() == constants::zero<T> );
    return ( abs( x.lower() ) < abs( x.upper() ) )
      ? ACCURACY_ASSURANCE( atan( 1.0 / x.upper() ), atan( 1.0 / x.lower() ) )
      : ACCURACY_ASSURANCE( atan( 1.0 / x.lower() ), atan( 1.0 / x.upper() ) );
  }

  /*  interval hyperbolic cosine  */

  template < typename T >
  inline interval<T> cosh( interval<T> const& x )
  {
    using std::cosh;
    auto&& a = x.lower();
    auto&& b = x.upper();
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( detail::is_overflow( cosh( a ), cosh( b ) ) );
    return ( b < constants::zero<T> )
      ? ACCURACY_ASSURANCE( cosh( b ), cosh( a ) )
        : ( constants::zero<T> < a )
        ? ACCURACY_ASSURANCE( cosh( a ), cosh( b ) )
      : [&] {
      UPWARD_POLICY;
      auto&& l = cosh( a );
      auto&& r = cosh( b );
      return interval<T>{ constants::one<T>, std::fmax( l, r ) };
    }( );
  }

  /*  interval hyperbolic sine  */

  template < typename T >
  inline interval<T> sinh( interval<T> const& x )
  {
    using std::sinh;
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( detail::is_overflow( sinh( x.lower() ), sinh( x.upper() ) ) );
    return ACCURACY_ASSURANCE( sinh( x.lower() ), sinh( x.upper() ) );
  }

  /*  interval hyperbolic tangent  */

  template < typename T >
  inline interval<T> tanh( interval<T> const& x ) noexcept
  {
    using std::tanh;
    return ACCURACY_ASSURANCE( tanh( x.lower() ), tanh( x.upper() ) );
  }

  template < typename T >
  inline interval<T> sech( interval<T> const& x ) noexcept
  {
    return cosh(x).inverse();
  }

  template < typename T >
  inline interval<T> csch( interval<T> const& x )
  {
    return sinh(x).inverse();
  }

  template < typename T >
  inline interval<T> coth( interval<T> const& x )
  {
    return tanh(x).inverse();
  }

  /*  interval arc hyperbolic cosine  */

  template < typename T >
  inline interval<T> acosh( interval<T> const& x )
  {
    using std::acosh;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.lower() < constants::one<T> );
    return ACCURACY_ASSURANCE( acosh( x.lower() ), acosh( x.upper() ) );
  }

  /*  interval arc hyperbolic sine  */

  template < typename T >
  inline interval<T> asinh( interval<T> const& x ) noexcept
  {
    using std::asinh;
    return ACCURACY_ASSURANCE( asinh( x.lower() ), asinh( x.upper() ) );
  }

  /*  interval arc hyperbolic tangent  */

  template < typename T >
  inline interval<T> atanh( interval<T> const& x )
  {
    using std::atanh;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.lower() < -constants::one<T> || constants::one<T> < x.upper() );
    return ACCURACY_ASSURANCE( atanh( x.lower() ), atanh( x.upper() ) );
  }

  template < typename T >
  inline interval<T> asech( interval<T> const& x )
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( 0.0 < x.lower() && x.upper() <= 1.0 ) );
    return acosh( x.inverse() );
  }

  template < typename T >
  inline interval<T> acsch( interval<T> const& x )
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.upper() < -1.0 || 1.0 < x.lower() ) );
    return asinh( x.inverse() );
  }

  template < typename T >
  inline interval<T> acoth( interval<T> const& x )
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.upper() < -1.0 || 1.0 < x.lower() ) );
    return atanh( x.inverse() );
  }

  /*  interval power function  */

  template < typename T >
  inline interval<T> pow( interval<T> const& x, long double n )
  {
    using math_impl::pow_up;
    using math_impl::pow_down;
    using std::pow;
    auto&& a = x.lower();
    auto&& b = x.upper();
    return ( n < 0 ) ? pow( x.inverse(), -n )
      : ( fmodl( n, 1 ) == 0 )
      ? (
      ( n == 0.0 )
        ? interval<T>{ constants::one<T>, constants::one<T> }
    : ( a <= 0.0 && 0.0 <= b )
      ? [&] {
      T l = pow_up( a, static_cast< std::size_t >( n ) );
      T r = pow_up( b, static_cast< std::size_t >( n ) );
      return interval<T>{0, std::fmax( l, r )};
    }( )
      : [&] {
      T l_min = pow_down( a, static_cast< std::size_t >( n ) );
      T r_min = pow_down( b, static_cast< std::size_t >( n ) );
      T l_max = pow_up( a, static_cast< std::size_t >( n ) );
      T r_max = pow_up( b, static_cast< std::size_t >( n ) );
      return interval<T>{ std::fmin( l_min, r_min ), std::fmax( l_max, r_max ) };
    }( )
      )
      : [&]{ CRANBERRIES_RANGE_ERROR_THROW_IF( a < 0.0 ); return true; }()
      : ( a <= 0.0 && 0.0 <= b )
      ? [&] {
      UPWARD_POLICY;
      T l = pow( a, n );
      T r = pow( b, n );
      return interval<T>{0, std::fmax( l, r )};
    }( )
      : [&] {
      DOWNWARD_POLICY;
      T l_min = pow( a, n );
      T r_min = pow( b, n );
      UPWARD_POLICY;
      T l_max = pow( a, n );
      T r_max = pow( b, n );
      return interval<T>{ std::fmin( l_min, r_min ), std::fmax( l_max, r_max ) };
    }( );
  }

  /*  interval square root function  */

  template < typename T >
  inline interval<T> sqrt( interval<T> const& x )
  {
    using std::sqrt;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.lower() < constants::zero<T> );
    return ACCURACY_ASSURANCE( sqrt( x.lower() ), sqrt( x.upper() ) );
  }

  template < typename T >
  inline interval<T> cbrt( interval<T> const& x ) noexcept
  {
    using std::cbrt;
    return ACCURACY_ASSURANCE( cbrt( x.lower() ), cbrt( x.upper() ) );
  }

  /*  interval expconstants::onential function ( base = e )  */

  template < typename T >
  inline interval<T> exp( interval<T> const& x )
  {
    using std::exp;
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( detail::is_overflow( exp( x.lower() ), exp( x.upper() ) ) );
    return ACCURACY_ASSURANCE( exp( x.lower() ), exp( x.upper() ) );
  }

  /*  interval expconstants::onential function ( base = 2 )  */

  template < typename T >
  inline interval<T> exp2( interval<T> const& x )
  {
    using std::exp2;
    CRANBERRIES_OVERFLOW_ERROR_THROW_IF( detail::is_overflow( exp2( x.lower() ), exp2( x.upper() ) ) );
    return ACCURACY_ASSURANCE( exp2( x.lower() ), exp2( x.upper() ) );
  }

  /*  interval exp( x ) - 1  */

  template < typename T >
  inline interval<T> expm1( interval<T> const& x )
  {
    using std::expm1;
    return detail::is_overflow( expm1( x.lower() ), expm1( x.upper() ) ) ? CRANBERRIES_RANGE_ERROR_THROW_WITH_MSG( "Overflow has occurred." ) : ACCURACY_ASSURANCE( expm1( x.lower() ), expm1( x.upper() ) );
  }

  /*  interval logarithmic function ( base = e ) */

  template < typename T >
  inline interval<T> log( interval<T> const& x )
  {
    using std::log;
    return ( constants::zero<T> < x.lower() ) ? ACCURACY_ASSURANCE( log( x.lower() ), log( x.upper() ) ) : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval log( x ) + 1  */
  template < typename T >
  inline interval<T> log1p( interval<T> const& x )
  {
    using std::log1p;
    return ( -constants::one<T> < x.lower() ) ? ACCURACY_ASSURANCE( log1p( x.lower() ), log1p( x.upper() ) ) : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval logarithmic function ( base = 10 )  */

  template < typename T >
  inline interval<T> log10( interval<T> const& x )
  {
    using std::log10;
    return ( constants::zero<T> < x.lower() ) ? ACCURACY_ASSURANCE( log10( x.lower() ), log10( x.upper() ) ) : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval logarithmic function ( base = 2 )  */
  template < typename T >
  inline interval<T> log2( interval<T> const& x )
  {
    using std::log2;
    return ( constants::zero<T> < x.lower() ) ? ACCURACY_ASSURANCE( log2( x.lower() ), log2( x.upper() ) ) : CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "lower_bound must be greater than zero." );
  }

  /*  interval absolute funtion  */

  template < typename T >
  inline interval<T> abs( interval<T> const& x )
  {
    using std::abs;
    auto&& low = x.lower();
    auto&& up = x.upper();
    return ( low < -constants::zero<T> && constants::zero<T> <= up ) ? [&] { auto&& l = abs( low ); auto&& r = abs( up ); return interval<T>{ constants::zero<T>, std::fmax( l, r ) }; }( )
      : ( up < constants::zero<T> ) ? [&] { auto&& l = abs( up ); auto&& r = abs( low ); return interval<T>{ l, r }; }( )
      : interval<T>{ x };
  }


  /*  interval fused multiply-add function fma( interval<T>, interval<T>, interval<T>)  */
  template < typename T1, typename T2, typename T3, typename T = std::common_type_t<T1, std::common_type_t<T2, T3>> >
  inline interval<T> fma( interval<T1> const& x, interval<T2> const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    auto x_low = x.lower();
    auto y_low = y.lower();
    auto z_low = z.lower();
    auto x_up = x.upper();
    auto y_up = y.upper();
    auto z_up = z.upper();

    return ( x_low >= constants::zero<T1> && y_low >= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_low, y_low, z_low ), fma( x_up, y_up, z_up ) )
      : ( x_low >= constants::zero<T1> && y_low < constants::zero<T2> && y_up > constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z_low ), fma( x_low, y_up, z_up ) )
      : ( x_low >= constants::zero<T1> && y_up <= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z_low ), fma( x_low, y_up, z_up ) )
      : ( x_low < constants::zero<T1> && x_up > constants::zero<T1> && y_low >= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_low, y_up, z_low ), fma( x_up, y_up, z_up ) )
      : ( x_low < constants::zero<T1> && x_up > constants::zero<T1> && y_low <= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z_low ), fma( x_low, y_low, z_up ) )
      : ( x_up <= constants::zero<T1> && y_low >= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_low, y_up, z_low ), fma( x_up, y_low, z_up ) )
      : ( x_up <= constants::zero<T> && y_low < constants::zero<T> && y_up > constants::zero<T> ) ? ACCURACY_ASSURANCE( fma( x_low, y_up, z_low ), fma( x_low, y_low, z_up ) )
      : ( x_up <= constants::zero<T> && y_up <= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_up, z_low ), fma( x_low, y_low, z_up ) )
      : ( x_up*y_low < x_low*y_up ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z_low ), fma( x_low, y_up, z_up ) )
      : ACCURACY_ASSURANCE( fma( x_low, y_up, z_low ), fma( x_up, y_low, z_up ) );
  }

  /*  interval fused multiplyply-add function fma( interval<T>, interval<T>, T)  */

  template < typename T1, typename T2, typename T = std::common_type_t<T1, T2> >
  inline interval<T> fma( interval<T1> const& x, interval<T2> const& y, T const& z ) noexcept
  {
    using std::fma;
    auto x_low = x.lower();
    auto y_low = y.lower();
    auto y_up = y.upper();
    auto x_up = x.upper();

    return ( x_low >= constants::zero<T1> && y_low >= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_low, y_low, z ), fma( x_up, y_up, z ) )
      : ( x_low >= constants::zero<T1> && y_low < constants::zero<T2> && y_up > constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z ), fma( x_low, y_up, z ) )
      : ( x_low >= constants::zero<T1> && y_up <= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z ), fma( x_low, y_up, z ) )
      : ( x_low < constants::zero<T1> && x_up > constants::zero<T1> && y_low >= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_low, y_up, z ), fma( x_up, y_up, z ) )
      : ( x_low < constants::zero<T1> && x_up > constants::zero<T1> && y_low <= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z ), fma( x_low, y_low, z ) )
      : ( x_up <= constants::zero<T1> && y_low >= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_low, y_up, z ), fma( x_up, y_low, z ) )
      : ( x_up <= constants::zero<T> && y_low < constants::zero<T> && y_up > constants::zero<T> ) ? ACCURACY_ASSURANCE( fma( x_low, y_up, z ), fma( x_low, y_low, z ) )
      : ( x_up <= constants::zero<T> && y_up <= constants::zero<T2> ) ? ACCURACY_ASSURANCE( fma( x_up, y_up, z ), fma( x_low, y_low, z ) )
      : ( x_up*y_low < x_low*y_up ) ? ACCURACY_ASSURANCE( fma( x_up, y_low, z ), fma( x_low, y_up, z ) )
      : ACCURACY_ASSURANCE( fma( x_low, y_up, z ), fma( x_up, y_low, z ) );
  }

  /*  interval fused multiplyply-add function fma( interval<T>, T, interval<T>)  */

  template < typename T1, typename T3, typename T = std::common_type_t<T1, T3> >
  inline interval<T> fma( interval<T1> const& x, T const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    return ( y < constants::zero<T> ) ? ACCURACY_ASSURANCE( fma( x.upper(), y, z.lower() ), fma( x.lower(), y, z.upper() ) )
      : ACCURACY_ASSURANCE( fma( x.lower(), y, z.lower() ), fma( x.upper(), y, z.upper() ) );
  }

  /*  interval fused multiplyply-add function fma( T, interval<T>, interval<T>)  */

  template < typename T2, typename T3, typename T = std::common_type_t<T2, T3> >
  inline interval<T> fma( T const& x, interval<T2> const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    return ( x < constants::zero<T> ) ? ACCURACY_ASSURANCE( fma( x, y.upper(), z.lower() ), fma( x, y.lower(), z.upper() ) )
      : ACCURACY_ASSURANCE( fma( x, y.lower(), z.lower() ), fma( x, y.upper(), z.upper() ) );
  }

  /*  interval fused multiplyply-add function fma( interval<T>, T, T)  */

  template < typename T >
  inline interval<T> fma( interval<T> const& x, typename interval<T>::value_type const& y, typename interval<T>::value_type const& z ) noexcept
  {
    using std::fma;
    return ( y < constants::zero<T> ) ? ACCURACY_ASSURANCE( fma( x.upper(), y, z ), fma( x.lower(), y, z ) )
      : ACCURACY_ASSURANCE( fma( x.lower(), y, z ), fma( x.upper(), y, z ) );
  }

  /*  interval fused multiplyply-add function fma( T, interval<T>, T)  */

  template < typename T >
  inline interval<T> fma( typename interval<T>::value_type const& x, interval<T> const& y, typename interval<T>::value_type const& z ) noexcept
  {
    using std::fma;
    return ( x < constants::zero<T> ) ? ACCURACY_ASSURANCE( fma( x, y.upper(), z ), fma( x, y.lower(), z ) )
      : ACCURACY_ASSURANCE( fma( x, y.lower(), z ), fma( x, y.upper(), z ) );
  }

  /*  interval fused multiplyply-add function fma( T, T, interval<T>)  */

  template < typename T >
  inline interval<T> fma( typename interval<T>::value_type const& x, typename interval<T>::value_type const& y, interval<T> const& z ) noexcept
  {
    using std::fma;
    return ACCURACY_ASSURANCE( fma( x, y, z.lower() ), fma( x, y, z.upper() ) );
  }

  /*  interval error function  */

  template < typename T >
  inline interval<T> erf( interval<T> const& x ) noexcept
  {
    using std::erf;
    return ACCURACY_ASSURANCE( erf( x.lower() ), erf( x.upper() ) );
  }

  /*  interval complementary error function  */

  template < typename T >
  inline interval<T> erfc( interval<T> const& x ) noexcept
  {
    using std::erfc;
    return ACCURACY_ASSURANCE( erfc( x.upper() ), erfc( x.lower() ) );
  }

  /*  is_singleton  */

  template < typename T >
  inline constexpr bool is_singleton( interval<T> const& x ) noexcept
  {
    return ( x.lower() == x.upper() ) ? true : false;
  }

  /*  Interval Accessors  */

  template < typename T >
  inline T interval<T>::lower() const& noexcept
  {
    return pImpl->lower();
  }

  template < typename T >
  inline T interval<T>::upper() const& noexcept
  {
    return pImpl->upper();
  }

  template < typename T >
  inline T& interval<T>::lower() & noexcept
  {
    return pImpl->lower();
  }

  template < typename T >
  inline T& interval<T>::upper() & noexcept
  {
    return pImpl->upper();
  }

  template < typename T >
  inline T&& interval<T>::lower() && noexcept
  {
    return pImpl->lower();
  }

  template < typename T >
  inline T&& interval<T>::upper() && noexcept
  {
    return pImpl->upper();
  }

  template < typename T >
  void interval<T>::swap( interval<T>& x ) noexcept
  {
    std::swap( this->pImpl, x.pImpl );
  }

  template < typename T >
  void swap( interval<T>& x, interval<T>& y ) noexcept
  {
    x.swap( y );
  }


} // ! cranberries
} // ! interval_lib
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC2_HPP

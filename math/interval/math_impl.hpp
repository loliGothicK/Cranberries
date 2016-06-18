#ifndef MATH_IMPL_HPP
#define MATH_IMPL_HPP
#include <cfenv>
#include <type_traits>
#include "set_round.hpp"

namespace Cranberries
{
  namespace math_impl
  {

    template < typename T >
    constexpr T PI() { return static_cast<T>( 3.141592653589793238462643383279502884197L ); }

    template < typename T >
    inline constexpr T sq_up( T x ) noexcept
    {
      UPWARD_POLICY;
      return x*x;
    }

    template < typename T >
    inline constexpr T sq_down( T x ) noexcept
    {
      DOWNWARD_POLICY;
      return x*x;
    }

    template < typename T >
    inline T pow_up( T x, size_t n ) noexcept
    {
      if ( n == 0 ) { return 1; }
      else if ( n == 1 ) { return x; }
      UPWARD_POLICY;
      return n % 2 ? x*sq_up( pow_up( x, ( n - 1 ) / 2 ) ) : sq_up( pow_up( x, n / 2 ) );
    }

    template < typename T >
    inline T pow_down( T x, size_t n ) noexcept
    {
      if ( n == 0 ) { return 1; }
      else if ( n == 1 ) { return x; }
      DOWNWARD_POLICY;
      return n % 2 ? x*sq_down( pow_down( x, ( n - 1 ) / 2 ) ) : sq_down( pow_down( x, n / 2 ) );
    }

    template <typename T>
    inline std::remove_reference_t<T> sin_up( T&& arg ) noexcept
    {
      UPWARD_POLICY;
      long double x = arg;
      bool sign = false;
      if ( x < 0 ) x = PI<long double>() - x;
      while ( x > 2.L*PI<long double>() ) x -= 2.L*PI<long double>();
      if ( x > PI<long double>() )
      {
        x -= PI<long double>();
        sign = true;
      }
      long double w = -8.22063524662433E-18L;
      for ( auto&& e : {
        2.81145725434552E-15L,
        -7.647163731819816E-13L,
        1.605904383682161E-10L,
        -2.505210838544172E-8L,
        2.755731922398589E-6L,
        -1.984126984126984E-4L,
        8.33333333333333E-3L,
        -1.666666666666667E-1L,
        1.L
      } )
      {
        w = e + w*x*x;
      }
      return sign ? -w*x : w*x;
    }
    template <typename T>
    inline std::remove_reference_t<T> sin_down( T&& arg ) noexcept
    {
      DOWNWARD_POLICY;
      long double x = arg;
      bool sign = false;
      if ( x < 0 ) x = PI<long double>() - x;
      while ( x > 2.L*PI<long double>() ) x -= 2.L*PI<long double>();
      if ( x > PI<long double>() )
      {
        x -= PI<long double>();
        sign = true;
      }
      long double w = -8.22063524662433E-18L;
      for ( auto&& e : {
        2.81145725434552E-15L,
        -7.647163731819816E-13L,
        1.605904383682161E-10L,
        -2.505210838544172E-8L,
        2.755731922398589E-6L,
        -1.984126984126984E-4L,
        8.33333333333333E-3L,
        -1.666666666666667E-1L,
        1.L
      } )
      {
        w = e + w*x*x;
      }
      return sign ? -w*x : w*x;
    }

    template <typename T>
    inline std::remove_reference_t<T> cos_up( T&& arg ) noexcept
    {
      UPWARD_POLICY;
      long double x = arg;
      bool sign = false;
      if ( x < 0 ) x = -x;
      while ( x > 2.L*PI<long double>() ) x -= 2.L*PI<long double>();
      if ( x > PI<long double>() )
      {
        x -= PI<long double>();
        sign = true;
      }
      long double w = 4.110317623312165E-19L;
      for ( auto&& e : {
        -1.561920696858623E-16L,
        4.779477332387385E-14L,
        -1.147074559772973E-11L,
        2.08767569878681E-9L,
        -2.755731922398589E-7L,
        2.48015873015873E-5L,
        -1.388888888888889E-3L,
        4.166666666666666E-2L,
        -0.5L,
        1.L
      } )
      {
        w = e + w*x*x;
      }
      return sign ? -w : w;
    }
    template <typename T>
    inline std::remove_reference_t<T> cos_down( T&& arg ) noexcept
    {
      DOWNWARD_POLICY;
      long double x = arg;
      bool sign = false;
      if ( x < 0 ) x = -x;
      while ( x > 2.L*PI<long double>() ) x -= 2.L*PI<long double>();
      if ( x > PI<long double>() )
      {
        x -= PI<long double>();
        sign = true;
      }
      long double w = 4.110317623312165E-19L;
      for ( auto&& e : {
        -1.561920696858623E-16L,
        4.779477332387385E-14L,
        -1.147074559772973E-11L,
        2.08767569878681E-9L,
        -2.755731922398589E-7L,
        2.48015873015873E-5L,
        -1.388888888888889E-3L,
        4.166666666666666E-2L,
        -0.5L,
        1.L
      } )
      {
        w = e + w*x*x;
      }
      return sign ? -w : w;
    }


  }
}
#endif // MATH_IMPL_HPP
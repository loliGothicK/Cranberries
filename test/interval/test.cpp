#define CRANBERRIES_INTERVAL_DEBUG_MODE
#include "../../cranberries/interval.hpp"
#include "../../cranberries/utility.hpp"
#include "../../cranberries/unit_test/unit_test.hpp"
#include <iostream>
#include <cassert>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using cranberries::interval;
using cranberries::hull;
using cranberries::compare::less;
using cranberries::compare::OrderPolocy;
namespace unit = cranberries::unit_test_framework;

int main()
try{
  cout << std::boolalpha << std::setprecision( std::numeric_limits<long double>::digits10 + 1 );

  const auto t = interval<>{ 1.1,1.2 };
  interval<>{t};
  interval<>{interval<>{}};
  interval<> s = { 1.0, 1.0 };
  interval<> x{ 1.0, 1.1 };
  auto y = interval<>( 3, 5 );
  auto z = interval<>( -1.0, 1.0 );

  unit::make_unit_test_container(unit::default_logger, "arithmetic tests")
    % unit::assertion::are_equal( x - x, interval<>{} )
    % unit::assertion::are_equal( z * z, pow( z, 2 ) )
    % unit::assertion::are_equal( x / x, interval<>{1} )
    % unit::assertion::are_equal_doubles( (interval<>(1,1) + interval<>(1,2)).lower(), { 2.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,1) + interval<>(1,2)).upper(), { 3.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(3,4) - interval<>(1,2)).lower(), { 1.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(3,4) - interval<>(1,2)).upper(), { 3.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,2) * interval<>(1,2)).lower(), { 1.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,2) * interval<>(1,2)).upper(), { 4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-1,2) * interval<>(1,2)).lower(), { -2.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-1,2) * interval<>(1,2)).upper(), { 4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-2,-1) * interval<>(1,2)).lower(), { -4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-2,-1) * interval<>(1,2)).upper(), { -1.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,2) * interval<>(-1,2)).lower(), { -2.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,2) * interval<>(-1,2)).upper(), { 4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,2) * interval<>(-2,-1)).lower(), { -4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(1,2) * interval<>(-2,-1)).upper(), { -1.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-1,2) * interval<>(-1,2)).lower(), { -2.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-1,2) * interval<>(-1,2)).upper(), { 4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-3,2) * interval<>(-2,1)).lower(), { -4.0, 1.0E-5 } )
    % unit::assertion::are_equal_doubles( (interval<>(-3,2) * interval<>(-2,1)).upper(), { 6.0, 1.0E-5 } )
    % unit::assertion::excact_throw<cranberries::overflow_error>([]{ interval<>{} / interval<>{-1,1}; })
    | unit::collect;

  unit::make_unit_test_container(unit::default_logger, "compound assignment tests")
    % unit::assertion::are_equal( [x_ = interval<>{ 1, 2 }]()mutable{ return x_-=x_; }(), interval<>{} )
    % unit::assertion::are_equal( z *= z, pow( z, 2 ) )
    % unit::assertion::are_equal( [x_ = interval<>{ 1, 2 }]()mutable{ return x_/=x_; }(), interval<>{1, 1} )
    | unit::collect;


  unit::make_unit_test_container(unit::default_logger, "comparison tests")
    % unit::assertion::less( interval<>(0,4), interval<>(0,3) )
    % unit::assertion::less( interval<>(2,4), interval<>(0,3) )
    % unit::assertion::less_equal( interval<>(1,2), interval<>(1,2) )
    % unit::assertion::less_equal( interval<>(0,4), interval<>(0,3) )
    % unit::assertion::greater( interval<>(1,2), interval<>(0,3) )
    % unit::assertion::greater( interval<>(0,4), interval<>(0,3) )
    % unit::assertion::greater( interval<>(-1,2), interval<>(0,3) )
    % unit::assertion::greater_equal( interval<>(1,2), interval<>(1,2) )
    % unit::assertion::greater_equal( interval<>(0,1), interval<>(0,3) )
    % unit::assertion::are_equal( interval<>(0,1), interval<>(0,1) )
    % unit::assertion::is_false( interval<>(1,2) == interval<>(0,3) )
    % unit::assertion::is_false( interval<>(1,2) != interval<>(1,2) )
    % unit::assertion::is_true( interval<>(1,2) != interval<>(0,3) )
    | unit::collect;

  fma( z, 2.0, 2.0 );
  /*  torigonometric functions  */
  sin( x );
  cos( x );
  tan( interval<>{0, 1} );
  sec( t );
  csc( t );
  cot( t );
  asec( t );
  acsc( t );
  acot( t );
  sech( t );
  csch( t );
  coth( t );
  asech( interval<>{0.5, 0.6} );
  acsch( t );
  acoth( t );
  pow( t, 2 );
  pow( t, 0 );
  pow( t, 1.1 );
  asin( s );
  acos( s );
  atan( s );

  /*  hyperbolic functions  */
  sinh( x );
  cosh( x );
  tanh( x );
  asinh( x );
  acosh( x );
  atanh( interval<>{0, 0.5} );

  /*  exponential & logarithmic functions  */
  log( x );
  log10( x );
  log2( x );
  log1p( x );
  exp( x );
  exp2( x );
  expm1( x );

  /*  power math & absolute functions  */
  pow( x, 2 );
  sqrt( x );
  abs( x );

  /*  error functions  */
  erf( x );
  erfc( x );

  /*  interval functions  */
  mid( x );
  middle( x );
  wid( x );
  width( x );
  is_singleton( x );
  in( 1, x );
  subset( interval<>{}, interval<>{} );
  proper_subset( interval<>{}, interval<>{} );
  overlap( interval<>{}, interval<>{} );
  intersect( interval<>{1, 3}, interval<>{2, 4} );

  max( x, interval<>{} );
  cranberries::max( { x,interval<>{},interval<>{} } );
  min( x, interval<>{} );
  cranberries::min( { x,interval<>{},interval<>{} } );
  min( interval<>{}, interval<>{}, interval<>{}, interval<>{} );

  cout << ">> Math Functions Complete!" << endl;

  cout << ">> Expression Complete!" << endl;
  try
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "@success" );
  }
  catch (cranberries::domain_error const&)
  {
    cout << ">> Exception Complete!" << endl;
  }

}
catch (unit::detail_::assert_except const&) {
  std::cout << "Test Failed\n";
  return EXIT_FAILURE;
}
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return EXIT_FAILURE;
}
catch (...) {
  std::cout << "Unhandled exception";
  return EXIT_FAILURE;
}

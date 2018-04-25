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
  interval<> x{ 1,1.1 };
  auto y = hull( 3, 5 );
  auto z = hull( -1.0, 1.0 );

  unit::make_unit_test_container(unit::default_logger, "arithmetic tests")
    % unit::assertion::are_equal( x - x, interval<>{} )
    % unit::assertion::are_equal( z * z, pow( z, 2 ) )
    % unit::assertion::are_equal( x / x, interval<>{1} )
    % unit::assertion::are_equal( hull(1,1) + hull(1,2), hull(2,3) )
    % unit::assertion::are_equal( hull(3,4) - hull(1,2), hull(1,3) )
    % unit::assertion::are_equal( hull(1,2) * hull(1,2), hull(1,4) )
    % unit::assertion::are_equal( hull(-1,2) * hull(1,2), hull(-2,4) )
    % unit::assertion::are_equal( hull(-1,-2) * hull(1,2), hull(-4,-1) )
    % unit::assertion::are_equal( hull(1,2) * hull(-1,2), hull(-2,4) )
    % unit::assertion::are_equal( hull(1,2) * hull(-1,-2), hull(-4,-1) )
    % unit::assertion::are_equal( hull(-1,2) * hull(-1,2), hull(-2,4) )
    % unit::assertion::are_equal( hull(-3,2) * hull(-2,1), hull(-4,6) )
    | unit::collect;

  unit::make_unit_test_container(unit::default_logger, "compound assignment tests")
    % unit::assertion::are_equal( x -= x, interval<>{} )
    % unit::assertion::are_equal( z *= z, pow( z, 2 ) )
    % unit::assertion::are_equal( x /= x, interval<>{1} )
    | unit::collect;


  unit::make_unit_test_container(unit::default_logger, "comparison tests")
    % unit::assertion::less( hull(0,4), hull(0,3) )
    % unit::assertion::less( hull(2,4), hull(0,3) )
    % unit::assertion::less_equal( hull(1,2), hull(1,2) )
    % unit::assertion::less_equal( hull(0,4), hull(0,3) )
    % unit::assertion::greater( hull(1,2), hull(0,3) )
    % unit::assertion::greater( hull(0,4), hull(0,3) )
    % unit::assertion::greater( hull(-1,2), hull(0,3) )
    % unit::assertion::greater_equal( hull(1,2), hull(1,2) )
    % unit::assertion::greater_equal( hull(0,1), hull(0,3) )
    % unit::assertion::are_equal( hull(0,1), hull(0,1) )
    % unit::assertion::is_false( hull(1,2) == hull(0,3) )
    % unit::assertion::is_false( hull(1,2) != hull(1,2) )
    % unit::assertion::is_true( hull(1,2) != hull(0,3) )
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
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return 0;
}
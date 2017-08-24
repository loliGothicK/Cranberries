#include "stdafx.h"
#include "CppUnitTest.h"
#define CRANBERRIES_INTERVAL_DEBUG_MODE
#include "../../cranberries/interval.hpp"
#include "../../cranberries/utility.hpp"
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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntervalTest
{		
	TEST_CLASS(IntervalTest)
	{
	public:
		
		TEST_METHOD(IntervalTestMethod)
		{
        // target version
        cout << ">> Target Version" << endl;
        cout << ">> Stable : " << cranberries::interval_version::stable << endl;
        cout << ">> Latest : " << cranberries::interval_version::latest << endl;
        cout << std::boolalpha << std::setprecision( std::numeric_limits<long double>::digits10 + 1 );
        // ctor
        interval<>{};
        interval<char>{}, interval<signed char>{}, interval<short>{}, interval<short int>{}, interval<signed short>{}, interval<signed short int>{}, interval<int>{}, interval<signed>{}, interval<signed int>{}, interval<long>{}, interval<long int>{}, interval<signed long>{}, interval<signed long int>{}, interval<long long>{}, interval<long long int>{}, interval<signed long long>{}, interval<signed long long int>{}, interval<float>{}, interval<double>{}, interval<long double>{}, interval<std::int8_t>{}, interval<std::int16_t>{}, interval<std::int32_t>{}, interval<std::int64_t>{}, interval<std::int_least8_t>{}, interval<std::int_least16_t>{}, interval<std::int_least32_t>{}, interval<std::int_least64_t>{}, interval<std::int_fast8_t>{}, interval<std::int_fast16_t>{}, interval<std::int_fast32_t>{}, interval<std::int_fast64_t>{};
        interval<char>{0}, interval<signed char>{0}, interval<short>{0}, interval<short int>{0}, interval<signed short>{0}, interval<signed short int>{0}, interval<int>{0}, interval<signed>{0}, interval<signed int>{0}, interval<long>{0}, interval<long int>{0}, interval<signed long>{0}, interval<signed long int>{0}, interval<long long>{0}, interval<long long int>{0}, interval<signed long long>{0}, interval<signed long long int>{0}, interval<float>{0}, interval<double>{0}, interval<long double>{0}, interval<std::int8_t>{0}, interval<std::int16_t>{0}, interval<std::int32_t>{0}, interval<std::int64_t>{0}, interval<std::int_least8_t>{0}, interval<std::int_least16_t>{0}, interval<std::int_least32_t>{0}, interval<std::int_least64_t>{0}, interval<std::int_fast8_t>{0}, interval<std::int_fast16_t>{0}, interval<std::int_fast32_t>{0}, interval<std::int_fast64_t>{0};
        interval<char>{0, 0}, interval<signed char>{0, 0}, interval<short>{0, 0}, interval<short int>{0, 0}, interval<signed short>{0, 0}, interval<signed short int>{0, 0}, interval<int>{0, 0}, interval<signed>{0, 0}, interval<signed int>{0, 0}, interval<long>{0, 0}, interval<long int>{0, 0}, interval<signed long>{0, 0}, interval<signed long int>{0, 0}, interval<long long>{0, 0}, interval<long long int>{0, 0}, interval<signed long long>{0, 0}, interval<signed long long int>{0, 0}, interval<float>{0, 0}, interval<double>{0, 0}, interval<long double>{0, 0}, interval<std::int8_t>{0, 0}, interval<std::int16_t>{0, 0}, interval<std::int32_t>{0, 0}, interval<std::int64_t>{0, 0}, interval<std::int_least8_t>{0, 0}, interval<std::int_least16_t>{0, 0}, interval<std::int_least32_t>{0, 0}, interval<std::int_least64_t>{0, 0}, interval<std::int_fast8_t>{0, 0}, interval<std::int_fast16_t>{0, 0}, interval<std::int_fast32_t>{0, 0}, interval<std::int_fast64_t>{0, 0};
        interval<>{std::make_pair( 1.0, 2.0 )};

        auto t = interval<>{ 1.1,1.2 };
        interval<>{t};
        interval<>{interval<>{}};
        interval<> s = { 1.0, 1.0 };
        interval<> x{ 1,1.1 };
        auto y = hull( 3, 5 );
        auto z = hull( -1.0, 1.0 );

        cout << ">> Constructor Complete!" << endl;

        x + x;
        assert( x - x == interval<>{} );
        assert( z * z == pow( z, 2 ) );
        assert( x / x == interval<>{1} );

        {
          auto tmp = interval<>{};
          tmp += {};
          tmp -= {};
          tmp *= {1};
          tmp /= {1, 1};
          tmp + tmp;
          tmp + interval<>{};
          interval<>{} +tmp;
          tmp + 2;
          2 + tmp;
          tmp + typename decltype(tmp)::value_type{};
          typename decltype(tmp)::value_type{} +tmp;
          interval<>{-1, 1}*interval<>{-1, 1};
        }

        cout << ">> Four Arithmetic Operator Complete!" << endl;

        const interval<double> a{ 1,3 }, b{ 3,4 }, c{ 5,6 }, d{ 6,7 };
        vector<interval<double>> vec{ a,b,c,d };
        std::sort( vec.begin(), vec.end(), less<OrderPolocy::Weak>() );

        cout << ">> Sort Complete!" << endl;

        a < b;
        a > b;
        a <= b;
        a >= b;
        a == b;
        a != b;

        cout << ">> Comparison Complete!" << endl;

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
        assert(mid( x )==1.05);
        assert(middle( x )==1.05);
        assert(wid( x )==0.1);
        assert( width( x ) == 0.1 );
        assert(!is_singleton( x ));
        assert( is_singleton( interval<>{1} ) );
        assert(in( 1, x ));
        assert(subset( interval<>{}, interval<>{} ));
        assert(!proper_subset( interval<>{}, interval<>{} ));
        assert(!overlap( interval<>{}, interval<>{} ));
        assert( ((intersect( interval<>{1, 3}, interval<>{2, 4} )) == interval<>{2,3}) );

        max( x, interval<>{} );
        cranberries::max( { x,interval<>{},interval<>{} } );
        min( x, interval<>{} );
        cranberries::min( { x,interval<>{},interval<>{} } );
        min( interval<>{}, interval<>{}, interval<>{}, interval<>{} );
        /*  accessors  */
        // getter and setter
        //lower( x ); // TODO
        //upper( x ); // TODO
        // swap
        swap( s, z );

        cout << ">> Math Functions Complete!" << endl;

        {
          interval<> hoge{};
          auto f = [](interval<> const& a) { return a - 1; };
          auto expr = cranberries::make_delegate([&] {return max(sin(a) - b, a * max(a, b, c, sin(a))) + min(pow(c, 2), erf(d) / a); });
          hoge = expr();
          hoge = f(expr());
          expr();
        }

        cout << ">> Expression Complete!" << endl;
        try
        {
          CRANBERRIES_DOMAIN_ERROR_THROW_WITH_MSG( "@success" );
        }
        catch (cranberries::domain_error const&)
        {
          cout << ">> Exception Complete!" << endl;
        }

        try
        {
          hull( 1, 0 );
        }
        catch (cranberries::invalid_argument const& e)
        {
          cout << ">> DEBUG MODE TEST" << endl;
          cout << e.what() << endl;
        }
    }

	};
}
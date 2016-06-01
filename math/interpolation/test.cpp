#include "interpolation.hpp"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using Cranberries::math::chebyshev;
using Cranberries::math::lstsq;
//using Cranberries::math::lagra;

template < typename T >
constexpr T PI = T( 3.1415926535 );

int main()
{
    cout << "polynomial evaluation" << endl;
    cout << "========================" << endl;
    cout << Cranberries::math::polynomial( std::vector<int>{ 0, 1, 2, 3, 4 }, 2 ) << endl;

    cout << endl;

    cout << "chebyshev polinomial approximation" << endl;
    cout << "========================" << endl;
    std::cout << std::fixed << std::setprecision( std::numeric_limits<long double>::digits10 + 1 );
    auto cheby = chebyshev<30>( []( long double x ) { return std::tan( x ); }, { /*-PI<double> / 2.,PI<double> / 2.0*/-1,1 } );
    for ( auto iter = std::crbegin( cheby ); iter != crend( cheby ); ++iter )
        cout << *iter << endl;

    cout << endl;

    cout << "least squares method" << endl;
    cout << "========================" << endl;
    for ( auto e : lstsq<4, 1>( { { 1., 2., 3., 4. } }, { { 2.,3.9,5.8,8. } } ) )
        cout << e << endl;

    //lagra<3>({ 1, 2, 3 }, { 2,4,6 }, 1.);
    return 0;
}
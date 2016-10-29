#include "stdafx.h"
#include <iostream>
#include "stream.hpp"
#include "streams\algorithm\radix_sort.hpp"
using std::cout;
using std::endl;
using cranberries::make_finally;


int main()
{
  using namespace cranberries::streams;
  using namespace cranberries;

  make_stream::of<double>({1,2,-3,3,6,5,4,-1})
    >> radix_sorted()
    >> println_to()
    ;

  make_stream::generate_canonical()
    >> taken( 100 )
    >> summary_stat()
    >> print_to()
    ;

  std::vector<std::vector<int>> aa = {
    {1,2,3},
    {1,2,3},
    {1,2,3}
  };

  //int a[] = {1,2,3};

  std::cout << std::boolalpha;

  make_stream::range( 1, 9 ) >> sliced( 3, 3 ) >> println_to();
  
  auto grouped = make_stream::range( 1, 9 )
    >> partitioning_by_hash<std::deque>( []( auto&& a ) { return a % 2 == 0; } )
    ;

  make_stream::generate( [ & ] {return std::vector<int>{1,2,3}; } )
    >> taken(3)
    >> flatten()
    >> partial_sorted(5)
    >> println_to()
    ;
  
  make_stream::of( { aa, aa, aa } ) >> all_flatten() >> println_to() ;

  stream_builder<int>{}
    .add( 1 )
    .add( 2 )
    .build()
    >> merged( std::vector<int>{1, 2, 3} )
    >> println_to()
    ;

  
  stream_builder<int>{}
    << 1 << 2 << 3 << 4 << build
    >> merged( std::vector<int>{1, 2, 3} )
    >> println_to()
    ;

  int arr[] = { 1,2,3 };

  make_stream::range( 1, 3 ) >> concat( arr ) >> println_to();

  cout << typeid(element_type_of_t<int[]>).name() << endl;
  cout << typeid(element_type_of_t<std::vector<int>>).name() << endl;
  cout << typeid(element_type_of_t<std::tuple<int,int,int>>).name() << endl;

  cout << "even" << endl;
  for (auto&& e : grouped[true])
    cout << e << ", ";
  cout << endl;
  cout << "odd" << endl;
  for (auto&& e : grouped[false])
    cout << e << ", ";
  cout << endl;

  make_stream::range( 1, 4 ) // [ 1, 4 )
    >> flat_transformed( []( auto const& a ) { return std::make_tuple( a, a ); } ) // [ i, i ]
    >> println_to() // [ 1, 1, 2, 2, 3, 3 ]
    ;
  make_stream::range( 1, 4 ) // [ 1, 4 )
    >> flat_transformed( []( auto const& a ) { return std::vector<std::decay_t<decltype(a)>>( a, a ); } ) // vector<T>( i, i )
    >> println_to() // [ 1, 2, 2, 3, 3, 3 ]
    ;
  make_stream::range( 1, 4 ) // [ 1, 4 )
    >> flat_transformed( []( auto const& a ) { return make_stream::range(0,a); } ) // [ 0... < i ]
    >> println_to() // [ 0, 0, 1, 0, 1, 2 ]
    ;
  make_stream::counter(1)
    >> flat_transformed([](auto const& a) { return std::make_tuple(a, a); }) // [ i, i ]
    >> taken(7)
    >> println_to()
    ;

  make_stream::cyclic( { 1, 2, 3 } )// [ 1, 2, 3, 1, 2, 3, 1, ... ]
    >> taken( 7 )
    >> println_to()
    ;

  std::list<int> li{ 1, 2, 3 };

  make_stream::cyclic( li ) // from range
    >> taken( 7 ) // take first nth(7th) element
    >> println_to()
    ;
  make_stream::cyclic( li.begin(), li.end() ) // from iterator pair
    >> taken( 7 )
    >> println_to()
    ;
  make_stream::range( 1, 6 ) // [ 1, 6 )
    >> replaced_if( []( auto&& a ) { return a % 2 == 0; }, 4 ) // replace odd to 4
    >> println_to(); // [ 1, 4, 3, 4, 5 ]

  // generate fibonacci series
  // iterator stream generate i+1 th element using i th element
  make_stream::iterate( 1,
    [ prev = 0 ]( auto&& a ) mutable {
      return make_finally([&]{ prev = a; }), prev + a;
    })
    >> taken( 10 )
    >> shuffled()
    >> println_to()
    ;
  
  make_stream::range( 1, 3 )
    >> concat(
      make_stream::counter( 5 )
    )
    >> taken(5)
    >> println_to()
    ;

  make_stream::counter( 1 )
    >> strided( 2 )
    >> dropped(1)
    >> taken(4)
    >> println_to()
    ;

  make_stream::counter( 3 )
    >> merged(
      make_stream::counter( 1 )
    )
    >> taken( 4 )
    >> println_to()
    ;

  make_stream::from( "123,123,123" | split( "," ) ) >> println_to();

  make_stream::range( 1, 13 )
    >> strided(2)
    >> sliced(2,5)
    >> println_to()
    ;

  make_stream::counter( 1 )
    >> transformed( []( auto&& a ) { return a * 2; } )
    >> strided( 2 )
    >> sliced(2,5)
    >> println_to()
    ;

  cout << ( make_stream::of( { 1,2,3,4,5,5,6,6 } ) >> median() ) << endl;

  cout << ( make_stream::of( { 1,2,3,4,5,5,6,6 } ) >> mode() ) << endl;

  cout << ( make_stream::range( 1, 11 ) >> accumulate( 1, []( auto&& a, auto&& b ) {return a*b; } ) ) << endl;

  cout << ( make_stream::range( 1, 4 ) >> product() ) << endl;

  make_stream::range( 1, 7 )
    .transform( []( auto&& a ) {return a * 2; } )
    .take(3)
    .println_to()
    ;

  make_stream::range( 1, 3 )
    .merge(
      make_stream::range( 1, 3 )
    )
    .println_to()
    ;

  make_stream::counter( 0 )
    .unique()
    .take(3)
    .println_to()
    ;

  make_stream::range( 1, 5 )
    >> merged(
      make_stream::counter( 1 )
    )
    >> taken( 10 )
    >> println_to()
    ;

  make_stream::counter( 1 )
    >> merged(
      std::vector<int>{1,2,3,4,5}
    )
    >> taken( 10 )
    >> println_to()
    ;

  make_stream::iterate( 1, []( auto&& a ) {return a * 2; } )
    >> taken_while( []( auto&& a ) { return a <= 1024; } )
    >> println_to()
    ;

  make_stream::counter(1)
    >> transformed( []( auto&& a ) { return a * a; } )
    >> taken( 5 )
    >> println_to()
    ;

  make_stream::of( { 1,1,1,4,2,3,4,3,4,5 } )
    >> uniquely()
    >> println_to()
    ;

  make_stream::range( 1, 10 ) >> println_to();

  make_stream::iterate( 1, []( auto&& a ) { return a * 2; } ) >> taken( 3 ) >> println_to();

  std::cout << "\nRandoms\n" << std::endl;

  make_stream::repeat( 1 ) >> taken( 3 ) >> println_to();

  make_stream::random() >> taken( 3 ) >> println_to();

  make_stream::uniform_dist( 0.0, 1.0 ) >> taken( 3 ) >> println_to();

  make_stream::bernoulli_dist( 0.7 ) >> taken( 3 ) >> println_to();

  make_stream::exponential_dist( 1.0 ) >> taken( 3 ) >> println_to();
 
  make_stream::poisson_dist( 4.0 ) >> taken( 3 ) >> println_to();

  make_stream::gamma_dist( 1.0, 1.0 ) >> taken( 3 ) >> println_to();

  make_stream::weibull_dist( 1.0, 2.0 ) >> taken( 3 ) >> println_to();

  make_stream::extreme_value_dist( 1.0, 2.0 ) >> taken( 3 ) >> println_to();

  make_stream::normal_dist( 1.0, 1.0 ) >> taken( 3 ) >> println_to();

  make_stream::lognormal_dist( 3.14, 1.0 ) >> taken( 3 ) >> println_to();

  make_stream::chi_squared_dist( 1.0 ) >> taken( 3 ) >> println_to();

  make_stream::cauchy_dist( 1.0, 1.0 ) >> taken( 3 ) >> println_to();

  make_stream::fisher_f_dist( 5, 10 ) >> taken( 3 ) >> println_to();

  make_stream::student_t_dist( 1.0 ) >> taken( 3 ) >> println_to();
  
  make_stream::generate_canonical() >> taken( 3 ) >> println_to();


#ifdef _MSC_VER
  getchar();
#endif
}
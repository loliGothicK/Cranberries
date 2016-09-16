#include <iostream>
#include "stream.hpp"

using std::cout;
using std::endl;

int main()
{
  using namespace cranberries::streams;
  std::cout << std::boolalpha;

  make_stream::range( 1, 6 )
    >> replaced_if( []( auto&& a ) { return a % 2 == 0; }, 4 )
    >> print_to();

  make_stream::iterate( 1, [prev2 = 0, prev1 = 0]( auto&& a )mutable{ return prev2 = prev1, prev1 = a, prev1 + prev2; } )
    >> taken( 10 )
    >> shuffled()
    >> print_to()
    ;
  
  make_stream::range( 1, 3 )
    >> concat(
      make_stream::counter( 5 )
    )
    >> taken(5)
    >> print_to()
    ;

  make_stream::counter( 1 )
    >> strided( 2 )
    >> dropped(1)
    >> taken(4)
    >> print_to()
    ;

  make_stream::counter( 3 )
    >> merged(
      make_stream::counter( 1 )
    )
    >> taken( 4 )
    >> print_to()
    ;

  make_stream::from( "123,123,123" | split( "," ) ) >> print_to();

  make_stream::range( 1, 13 )
    >> strided(2)
    >> sliced(2,5)
    >> print_to()
    ;

  make_stream::counter( 1 )
    >> transformed( []( auto&& a ) { return a * 2; } )
    >> strided( 2 )
    >> sliced(2,5)
    //>> taken(5)
    >> print_to()
    ;

  cout << ( make_stream::of( { 1,2,3,4,5,5,6,6 } ) >> median() ) << endl;

  cout << ( make_stream::of( { 1,2,3,4,5,5,6,6 } ) >> mode() ) << endl;

  cout << ( make_stream::range( 1, 11 ) >> sum() ) << endl;

  cout << ( make_stream::range( 1, 4 ) >> prod() ) << endl;

  make_stream::range( 1, 7 )
    .transform( []( auto&& a ) {return a * 2; } )
    .take(3)
    .print_to()
    ;

  make_stream::range( 1, 3 )
    .merge(
      make_stream::range( 1, 3 )
    )
    .print_to()
    ;

  make_stream::counter( 0 )
    .unique()
    .take(3)
    .print_to()
    ;

  make_stream::range( 1, 5 )
    >> merged(
      make_stream::counter( 1 )
    )
    >> taken( 10 )
    >> print_to()
    ;

  make_stream::counter( 1 )
    >> merged(
      make_stream::range( 1, 5 )
    )
    >> taken( 10 )
    >> print_to()
    ;

  make_stream::iterate( 1, []( auto&& a ) {return a * 2; } )
    >> taken_while( []( auto&& a ) { return a <= 1024; } )
    >> print_to()
    ;

  make_stream::counter(1)
    >> transformed( []( auto&& a ) { return a * a; } )
    >> taken( 5 )
    >> print_to()
    ;

  make_stream::of( { 1,1,1,4,2,3,4,3,4,5 } )
    >> uniquely()
    >> print_to()
    ;

  make_stream::range( 1, 10 ) >> print_to();

  make_stream::iterate( 1, []( auto&& a ) { return a * 2; } ) >> taken( 3 ) >> print_to();

  std::cout << "\nRandoms\n" << std::endl;

  make_stream::repeat( 1 ) >> taken( 3 ) >> print_to();

  make_stream::random() >> taken( 3 ) >> print_to();

  make_stream::uniform_dist( 0.0, 1.0 ) >> taken( 3 ) >> print_to();

  make_stream::bernoulli_dist( 0.7 ) >> taken( 3 ) >> print_to();

  make_stream::exponental_dist( 1.0 ) >> taken( 3 ) >> print_to();
 
  make_stream::poisson_dist( 4.0 ) >> taken( 3 ) >> print_to();

  make_stream::gamma_dist( 1.0, 1.0 ) >> taken( 3 ) >> print_to();

  make_stream::weibull_dist( 1.0, 2.0 ) >> taken( 3 ) >> print_to();

  make_stream::extreme_value_dist( 1.0, 2.0 ) >> taken( 3 ) >> print_to();

  make_stream::normal_dist( 1.0, 1.0 ) >> taken( 3 ) >> print_to();

  make_stream::lognormal_dist( 3.14, 1.0 ) >> taken( 3 ) >> print_to();

  make_stream::chi_squared_dist( 1.0 ) >> taken( 3 ) >> print_to();

  make_stream::cauchy_dist( 1.0, 1.0 ) >> taken( 3 ) >> print_to();

  make_stream::fisher_f_dist( 5, 10 ) >> taken( 3 ) >> print_to();

  make_stream::student_t_dist( 1.0 ) >> taken( 3 ) >> print_to();
  
  make_stream::generate_canonical() >> taken( 3 ) >> print_to();




#ifdef _MSC_VER
  getchar();
#endif
}
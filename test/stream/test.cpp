#include <iostream>
#include "../../cranberries/stream.hpp"
using std::cout;
using std::endl;
using cranberries::make_finally;


int main()
try{
  using namespace cranberries;
  using namespace cranberries::streams;

  make_stream::of({ 1,2,3,4 })
    >> lazy::transformed([](auto&& a) { return a*a; })
    >> lazy::transformed([](auto&& a) {return a + 1; })
    >> eager::println_to()
    ;

  make_stream::of({ 1,2,3,4,5,6 })
    >> lazy::chunk<2>()
    >> eager::for_each([](auto&& a) { for (auto const& e : a) std::cout << e << ", "; std::cout << std::endl; })
    ;

  make_stream::of({ 1,2,3,4,5,6 })
    >> eager::chunk_for_each<2>(
      [](auto&& a, auto&& b) { std::cout << a << ":" << b << std::endl; }
      )
    ;

  make_stream::fopen("./file_io_test.dat")
    >> eager::read_line()
    >> lazy::transformed([](auto&& a) { return a += "hoge"; })
    >> eager::write()
    ;

  // make_stream::fopen<char>("./binary.dat")
  //   >> eager::read_byte()
  //   >> eager::println_to()
  //   ;

  std::vector<int> v1 = make_stream::range(1, 4) >> eager::convert;

  auto v2 = make_stream::range(1, 4) >> eager::convert_to<std::list>;

  make_stream::of({ 9,8,7,6,5,4,3,2,1,-1,-1,-2,-1 })
    >> lazy::sorted(1, 3)
    >> eager::println_to()
    ;

  make_stream::of({ 9,8,7,6,5,4,3,2,1,-1,-1,-2,-1 })
    >> lazy::ascending_radix_sorted(1, 4)
    >> eager::println_to()
    ;

  make_stream::of({ 9,8,7,6,5,4,3,2,1,-1,-1,-2,-1 })
    >> lazy::ascending_radix_sorted()
    >> eager::println_to()
    ;

  make_stream::generate_canonical()
    >> lazy::taken(100)
    >> lazy::summary_stat()
    >> eager::print_to()
    ;

  std::vector<std::vector<int>> aa = {
    { 1,2,3 },
    { 1,2,3 },
    { 1,2,3 }
  };

  std::cout << std::boolalpha;

  make_stream::range(1, 9)
    >> lazy::sliced(3, 3)
    >> eager::println_to()
    ;

  auto grouped = make_stream::range(1, 9)
    >> eager::partitioning_by_hash<std::deque>([](auto&& a) { return a % 2 == 0; })
    ;

  cout << "here!" << endl;

  make_stream::generate([&] { return aa; })
    >> lazy::all_flatten()
    >> lazy::taken(9)
    >> eager::println_to()
    ;

  make_stream::generate([&] {return std::vector<int>{1, 2, 3}; })
    >> lazy::flatten()
    >> lazy::taken(9)
    >> lazy::partial_sorted(5)
    >> eager::println_to()
    ;

  make_stream::of({ aa, aa, aa })
    >> lazy::all_flatten()
    >> eager::println_to()
    ;

  stream_builder<int>{}
  .add(1)
    .add(2)
    .build()
    >> lazy::merged(std::vector<int>{1, 2, 3})
    >> eager::println_to()
    ;


  stream_builder<int>{}
  << 1 << 2 << 3 << 4 << build
    >> lazy::merged(std::vector<int>{1, 2, 3})
    >> eager::println_to()
    ;

  int arr[] = { 1,2,3 };

  make_stream::range(1, 3) >> lazy::joined(arr) >> eager::println_to();

  cout << "even" << endl;
  for (auto&& e : grouped[true])
    cout << e << ", ";
  cout << endl;
  cout << "odd" << endl;
  for (auto&& e : grouped[false])
    cout << e << ", ";
  cout << endl;

  make_stream::range(1, 4) // [ 1, 4 )
    >> lazy::flat_transformed([](auto const& a) { return std::make_tuple(a, a); }) // [ i, i ]
    >> eager::println_to() // [ 1, 1, 2, 2, 3, 3 ]
    ;

  make_stream::range(1, 4) // [ 1, 4 )
    >> lazy::flat_transformed([](auto const& a) { return std::vector<std::decay_t<decltype(a)>>(a, a); }) // vector<T>( i, i )
    >> eager::println_to() // [ 1, 2, 2, 3, 3, 3 ]
    ;

  make_stream::range(1, 4) // [ 1, 4 )
    >> lazy::flat_transformed([](auto const& a) { return make_stream::range(0, a); }) // [ 0... < i ]
    >> eager::println_to() // [ 0, 0, 1, 0, 1, 2 ]
    ;

  make_stream::counter(1)
    >> lazy::flat_transformed([](auto const& a) { return std::make_tuple(a, a); }) // [ i, i ]
    >> lazy::taken(7)
    >> eager::println_to()
    ;

  make_stream::cyclic({ 1, 2, 3 })// [ 1, 2, 3, 1, 2, 3, 1, ... ]
    >> lazy::taken(7)
    >> eager::println_to()
    ;

  std::list<int> li{ 1, 2, 3 };

  make_stream::cyclic(li) // from range
    >> lazy::taken(7) // take first nth(7th) element
    >> eager::println_to()
    ;

  make_stream::cyclic(li.begin(), li.end()) // from iterator pair
    >> lazy::taken(7)
    >> eager::println_to()
    ;

  make_stream::range(1, 6) // [ 1, 6 )
    >> lazy::replaced_if([](auto&& a) { return a % 2 == 0; }, 4) // replace odd to 4
    >> eager::println_to() // [ 1, 4, 3, 4, 5 ]
    ;

  // generate fibonacci series
  // iterator stream generate i+1 th element using i th element
  make_stream::iterate(1,
    [prev = 0](auto&& a) mutable {
    return make_finally([&] { prev = a; }), prev + a;
  })
    >> lazy::taken(10)
    >> lazy::shuffled<std::default_random_engine>()
    >> eager::println_to()
    ;

  make_stream::range(1, 3)
    >> lazy::joined(
      make_stream::counter(5)
    )
    >> lazy::taken(5)
    >> eager::println_to()
    ;

  make_stream::counter(1)
    >> lazy::strided(2)
    >> lazy::dropped(1)
    >> lazy::taken(4)
    >> eager::println_to()
    ;

  make_stream::counter(3)
    >> lazy::merged(
      make_stream::counter(1)
    )
    >> lazy::taken(4)
    >> eager::println_to()
    ;

  make_stream::from("123,123,123" | split(","))
    >> eager::println_to()
    ;

  make_stream::range(1, 13)
    >> lazy::strided(2)
    >> lazy::sliced(2, 5)
    >> eager::println_to()
    ;

  make_stream::counter(1)
    >> lazy::transformed([](auto&& a) { return a * 2; })
    >> lazy::strided(2)
    >> lazy::sliced(2, 5)
    >> eager::println_to()
    ;

  cout << (make_stream::of({ 1,2,3,4,5,5,6,6 }) >> eager::median()) << endl;

  cout << (make_stream::of({ 1,2,3,4,5,5,6,6 }) >> eager::mode()) << endl;

  cout << (make_stream::range(1, 11) >> eager::accumulate(1, [](auto&& a, auto&& b) {return a*b; })) << endl;

  cout << (make_stream::range(1, 4) >> eager::product()) << endl;

  make_stream::range(1, 7)
    .transform([](auto&& a) {return a * 2; })
    .take(3)
    .println_to()
    ;

  make_stream::range(1, 3)
    .merge(
      make_stream::range(1, 3)
    )
    .println_to()
    ;

  make_stream::counter(0)
    .unique()
    .take(3)
    .println_to()
    ;

  make_stream::range(1, 5)
    >> merged(
      make_stream::counter(1)
    )
    >> lazy::taken(10)
    >> eager::println_to()
    ;

  make_stream::counter(1)
    >> lazy::merged(
      std::vector<int>{1, 2, 3, 4, 5}
  )
    >> lazy::taken(10)
    >> eager::println_to()
    ;

  make_stream::iterate(1, [](auto&& a) {return a * 2; })
    >> lazy::taken_while([](auto&& a) { return a <= 1024; })
    >> eager::println_to()
    ;

  make_stream::counter(1)
    >> lazy::transformed([](auto&& a) { return a * a; })
    >> lazy::taken(5)
    >> eager::println_to()
    ;

  make_stream::of({ 1,1,1,4,2,3,4,3,4,5 })
    >> lazy::uniquely()
    >> eager::println_to()
    ;

  make_stream::range(1, 10) >> eager::println_to();

  make_stream::iterate(1, [](auto&& a) { return a * 2; }) >> lazy::taken(3) >> eager::println_to();

  std::cout << "\nRandoms\n" << std::endl;

  make_stream::repeat(1) >> lazy::taken(3) >> eager::println_to();

  make_stream::random() >> lazy::taken(3) >> eager::println_to();

  make_stream::uniform_dist(0.0, 1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::bernoulli_dist(0.7) >> lazy::taken(3) >> eager::println_to();

  make_stream::exponential_dist(1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::poisson_dist(4.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::gamma_dist(1.0, 1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::weibull_dist(1.0, 2.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::extreme_value_dist(1.0, 2.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::normal_dist(1.0, 1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::lognormal_dist(3.14, 1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::chi_squared_dist(1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::cauchy_dist(1.0, 1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::fisher_f_dist(5, 10) >> lazy::taken(3) >> eager::println_to();

  make_stream::student_t_dist(1.0) >> lazy::taken(3) >> eager::println_to();

  make_stream::generate_canonical() >> lazy::taken(3) >> eager::println_to();

  stream<int>{}
  >> lazy::shuffled()
    >> eager::println_to()
    ;

}
catch (cranberries::runtime_error const& e) {
  cout << e.what() << endl;
  return 0;
}
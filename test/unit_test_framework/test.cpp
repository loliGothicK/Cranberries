#include <iostream>
#include <initializer_list>
#include <chrono>
#include "../../cranberries/unit_test/unit_test.hpp"


int main() {
  using namespace cranberries::unit_test_framework;
  using std::this_thread::sleep_for;  

  UnitTestContainer{"Unit Test Example"}
  .push( assert::are_equal(
    [] { sleep_for(std::chrono::seconds(3)); return 2; },
    2))
  .push( assert::are_equal(
    [] { sleep_for(std::chrono::seconds(1)); return 2; },
    2))
  .push( assert::permutation_execute<2>(
    [](auto a, auto b) { return a + b; },
    assert::test_skip,
    1, 2, 3, 4, 5))
  .push( assert::are_equal(
    [] { return 100; },
    100))
  .push( assert::range_equal(
    [] { return std::array<int, 4>{ {1, 2, 3, 4}}; },
    1, 2, 3))
  .push( assert::excact_throw<std::runtime_error>(
    [] { throw std::runtime_error{ "error!" }; }))
  ;
return 0;

}
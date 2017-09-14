#include <iostream>
#include <initializer_list>
#include <chrono>
#include "../../cranberries/unit_test/unit_test.hpp"


int main() {
  using namespace cranberries::unit_test_framework;

  unit_test{"Unit Test Example"}
    << assert::are_equal([] { std::this_thread::sleep_for(std::chrono::seconds(3)); return 2; }, 2)
    << assert::permutation_execute<2>([](auto a, auto b) { return a + b; },
                                      assert::test_skip,
                                      1,2,3,4,5)
    << assert::are_equal([] { return 100; }, 100)
    << assert::range_equal([] { return std::vector<int>{1,2,3,4}; }, 1,2,3)
    << assert::excact_throw<std::runtime_error>([] { throw std::runtime_error{ "error!" }; })
    ;

  return 0;
}
#include "../../cranberries/experimental/rkf45.hpp"
#include <iostream>
#include <cassert>
#include <vector>

int main()
{
  auto solver = cranberries::make_rkf45([](long double t, long double x) { return x*std::cos(t); })
      .set_variable_range({ 0,10 })
      .set_init_value(0)
      .set_tolerance(1.0E-8L)
      .set_step_size_range({ 0.001,0.1 });

  std::cout << std::boolalpha << solver.solve() << std::endl;
}
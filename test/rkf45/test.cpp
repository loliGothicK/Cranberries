#include "../../cranberries/experimental/rkf45.hpp"
#include <iostream>
#include <cassert>
#include <vector>

int main()
{
  auto solver = cranberries::make_rkf45([](long double t, long double x) { return x*std::cos(t); })
    .set_integrate_range({ 0,10 })
    .set_tolerance(1.0E-4L)
    .set_step_size_range({ 0.001,0.5 });

  solver.integrate(1.0L, [](auto t, auto x) { std::cout << t << " " << x << '\n'; });
}
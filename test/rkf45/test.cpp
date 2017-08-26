#include "../../cranberries/rkf45.hpp"
#include <iostream>
#include <cassert>
#include <array>
#include <vector>

int main()
{
  using state_type = std::array<long double, 3>;
  const long double p = 10;
  const long double r = 28;
  const long double b = 8. / 3;

  state_type state0 = { 10, 1, 1 };
  bool exit_status1 = cranberries::make_rkf45_solver_for_first_order(
    // Lorenz attractor
    [&](const long double t, const auto& x) {
    state_type dxdt{};
    // x=x[0], y=x[1], z=x[2]
    dxdt[0] = -p*x[0] + p*x[1];              // dx/dt
    dxdt[1] = -x[0] * x[2] + r*x[0] - x[1];  // dy/dt
    dxdt[2] = x[0] * x[1] - b*x[2];          // dz/dt
    return dxdt;
  })
  .set_integrate_range({ 0.0L,25.0L })
  .set_tolerance(8.0E-3L)
  .set_step_size_range({ 1.0E-6L,0.05L })
  .integrate(state0,
      [&](auto t, state_type state){
  });

  bool exit_status2 = cranberries::make_rkf45_solver_for_second_order(
    [](const auto t, const auto& y, const auto& z) {
    return z;
  },
    [](const auto t, const auto& y, const auto& z) {
    return -10.L*y - std::sqrt(40.L)*z;
  })
    .set_integrate_range({ 0,25 })
    .set_tolerance(8.0E-3L)
    .set_step_size_range({ 1.0E-6,0.05 })
    .integrate(10.L, 0.L,
      [](auto t, auto state) {
  });

  return exit_status1 && exit_status2 ? EXIT_SUCCESS : EXIT_FAILURE;
}

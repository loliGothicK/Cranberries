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
  auto solver = cranberries::make_rkf45(
    // Lorenz attractor
    [&](const long double t, const state_type& x) {
    state_type dxdt{};
    // x=x[0], y=x[1], z=x[2]
    dxdt[0] = -p*x[0] + p*x[1];              // dx/dt
    dxdt[1] = -x[0] * x[2] + r*x[0] - x[1];  // dy/dt
    dxdt[2] = x[0] * x[1] - b*x[2];          // dz/dt
    return dxdt;
  })
  .set_integrate_range({ 0,25 })
  .set_tolerance(8.0E-3L)
  .set_step_size_range({ 1.0E-6,0.05 })
  .integrate(state0,
      [&](auto t, state_type state){
      std::cout
        << "t=" << t << " "
        << "x=" << state[0] << " "
        << "y=" << state[1] << " "
        << "z=" << state[2] << "\n";
  });
  
  return EXIT_SUCCESS;
}

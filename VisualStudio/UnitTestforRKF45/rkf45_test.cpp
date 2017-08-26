#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../cranberries/rkf45.hpp"
#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RKF45Test
{		
	TEST_CLASS(RKF45Test)
	{
	public:
    using state_type = std::array<long double, 3>;
    const long double p = 10;
    const long double r = 28;
    const long double b = 8. / 3;


		TEST_METHOD(RKF45TestTestMethod)
		{
        state_type state0 = { 10, 1, 1 };
        auto solver = cranberries::make_rkf45_solver_for_first_order([&](const long double t, const state_type& x)
        {
          state_type dxdt{};
          // x=x[0], y=x[1], z=x[2]
          dxdt[0] = -p*x[0] + p*x[1];              // dx/dt
          dxdt[1] = -x[0] * x[2] + r*x[0] - x[1];   // dy/dt
          dxdt[2] = x[0] * x[1] - b*x[2];            // dz/dt
          return dxdt;
        })
          .set_integrate_range(0.0L, 25.0L)
          .set_tolerance(1.0E-4L)
          .set_step_size_range(0.001L, 0.5L);

        solver.integrate(state0, [](auto t, state_type x) { });
    }

	};
}
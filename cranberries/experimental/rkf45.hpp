#ifndef CRANBERRIES_RKF45_HPP
#define CRANBERRIES_RKF45_HPP
#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <cmath>
#include "../common/exception.hpp"
#include "../type_traits.hpp"

namespace cranberries {
  template < typename F >
  class RKF45 {
    F f;
    long double a,b,tol,h_min,h_max;
  public:
    RKF45(F f) noexcept : f{ f } {}

    // { a, b, alpha, tol, h_min, h_max }
    auto& init(generate_tuple_t<long double,5> params) {
      std::tie(a, b, tol, h_min, h_max) = params;
      return *this;
    }

    auto& set_integrate_range(generate_tuple_t<long double, 2> params) {
      std::tie(a, b) = params;
      return *this;
    }

    auto& set_tolerance(long double param) {
      tol = param;
      return *this;
    }

    auto& set_step_size_range(generate_tuple_t<long double, 2> params) {
      std::tie(h_min, h_max) = params;
      return *this;
    }

    template < typename State >
    bool integrate(State alpha) noexcept {
      return integrate(alpha, [](auto t, auto x) { std::cout << t << " " << x << '\n'; });
    }

    template < typename State, typename Observer >
    bool integrate(State alpha, Observer&& ob ) noexcept {
      /// step 0:
      long double k1{}, k2{}, k3{}, k4{}, k5{}, k6{};
      long double R{}, sigma{};
      /// step 1:
      auto t = a;
      auto x = alpha;
      auto h = h_min;
      auto next = [&] {
        k1 = h*f(t, x);
        k2 = h*f(t + 0.25L*h, x + 0.25L*k1);
        k3 = h*f(t + 3.0L / 8.0L*h, (3.0L*k1 + 9.0L*k2) / 32.0L);
        k4 = h*f(t + 12.0L / 13.0L*h, x + (1932.0L*k1 - 7200.0L*k2 + 7296.0L*k3) / 2197.0L);
        k5 = h*f(t + h, x + 439.0L / 216.0L*k1 - 8.0L*k2 + 3680.0L / 513.0L*k3 - 845.0L / 4104 * k4);
        k6 = h*f(t + 0.5L*h, x - 8.0L / 27.0L*k1 + 2.0L*k2 - 3544.0L / 2565.0L*k3 + 1859.0L / 4104.0L*k4 - 11.0L / 40.0L*k5);
        t += h;
        x += 25.0L / 216.0L*k1 + 1408.0L / 2565.0L*k3 + 2197.0L / 4101.0L*k4 - k5 / 5.0L;
        ob(t, x);
      };

      /// step 2:
      while (true) {
        /// step 3:
        /// step 6:
        next();
        R = std::abs(k1 / 360.0L - 128.0L / 4275.0L*k3 - 2282683.0L / 77139810.0L*k4 + k5 / 50.0L + 2.0L / 55.0L*k6);
        sigma = 0.84L*std::sqrt(std::sqrt(tol / R));
        /// step 7:
        h = sigma <= 0.1L ? 0.1L*h :
            sigma >= 4.0L ? 4.0L*h :
            sigma*h;
        /// step 8:
        if (h_max < h) h = h_max;
        if (b <= t) break;
        if (b < t + h && t < b) goto LAST;
        if (h < h_min) goto FAILURE;
      }
      return true;
    LAST:
      t = b - h;
      next();
      return true;
    FAILURE:
      return false;
    }
  };

  template < typename F >
  RKF45<F> make_rkf45(F&& f) noexcept {
    return std::forward<F>(f);
  }
} // ! namespace cranberries

#endif
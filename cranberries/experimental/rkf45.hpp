#ifndef CRANBERRIES_RKF45_HPP
#define CRANBERRIES_RKF45_HPP
#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include "../common/exception.hpp"
#include "../type_traits.hpp"

namespace cranberries {
  template < typename F >
  class RKF45 {
    F f;
    long double a,b,alpha,tol,h_min,h_max;
    std::vector<generate_tuple_t<long double, 3>> data;
  public:
    RKF45(F f) noexcept : f{ f } {}

    // { a, b, alpha, tol, h_min, h_max }
    auto& init(generate_tuple_t<long double, 6> params) {
      std::tie(a, b, alpha, tol, h_min, h_max) = params;
      return *this;
    }

    auto& set_variable_range(generate_tuple_t<long double, 2> params) {
      std::tie(a, b) = params;
      return *this;
    }

    auto& set_init_value(long double param) {
      alpha = param;
      return *this;
    }

    auto& set_tolerance(long double param) {
      alpha = param;
      return *this;
    }

    auto& set_step_size_range(generate_tuple_t<long double, 2> params) {
      std::tie(h_min, h_max) = params;
      return *this;
    }

    void init() noexcept {
      data.clear();
      data.reserve(1000);
      data.emplace_back(a, alpha, h_max);
    }

    bool solve() noexcept {
      /// step 0:
      init();
      long double k1{}, k2{}, k3{}, k4{}, k5{}, k6{};
      long double R{}, sigma{};
      /// step 1:
      auto t = a;
      auto x = alpha;
      auto h = h_max;
      /// step 2:
      while (true) {
        /// step 3:
        k1 = f(t, x); 
        k2 = f(t + 0.25L*h, x + 0.25L*k1);
        k3 = f(t + 3.0L/8.0L*h, (3.0L*k1 + 9.0L*k2) / 32.0L);
        k4 = f(t + 12.0L/13.0L*h, x + (1932.0L*k1 - 7200.0L*k2 + 7296.0L*k3) / 2197.0L);
        k5 = f(t + h, x + 439.0L/216.0L*k1 - 8.0L*k2 + 3680.0L/513.0L*k3 - 845.0L/4104*k4);
        k6 = f(t + 0.5L*h, x -8.0L/27.0L*k1 + 2.0L*k2 - 3544.0L/2565.0L*k3 + 1859.0L/4104.0L*k4 - 11.0L/40.0L*k5);
        /// step 4:
        R = std::abs(k1 / 360.0L - 128.0L / 4275.0L*k3 - 2197.0L / 75240.0L*k4 + k5 / 50.0L + 2.0L / 55.0L*k6);
        /// step 5:
        t += h;
        x = x + h*(25.0L / 216.0L*k1 + 1408.0L / 2565.0L*k3 + 2197.0L / 4104.0L*k4 - k5 / 5.0L);
        data.emplace_back(t, x, h);
        /// step 6:
        sigma = std::sqrt(std::sqrt(tol / (2 * R)));
        /// step 7:
        h = sigma <= 0.1L ? 0.1L*h :
            sigma >= 4.0L ? 4.0L*h :
            sigma*h;
        std::cout << sigma << std::endl;
        /// step 8:
        if (h_max < h) h = h_max;
        if (b <= t) break;
        if (h < h_min) {
          std::cout << h << std::endl;
          goto FAILURE;
        }
      }
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
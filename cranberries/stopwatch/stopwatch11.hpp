/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STOPWATCH_STOPWATCH11_HPP
#define CRANBERRIES_STOPWATCH_STOPWATCH11_HPP

#include <chrono>

namespace cranberries {
namespace cranberries_magic {

class TimerBaseChrono {
public:
  using tick_t = std::chrono::high_resolution_clock::time_point;
  //  clears the timer
  TimerBaseChrono() noexcept : start_{ std::chrono::high_resolution_clock::time_point::min() } { }

  //  clears the timer
  void clear() noexcept { start_ = std::chrono::high_resolution_clock::time_point::min(); }

  //  returns true if the timer is running
  bool is_started() const noexcept { return start_ != std::chrono::high_resolution_clock::time_point::min(); }

  //  start the timer
  void start() noexcept { start_ = std::chrono::high_resolution_clock::now(); }

  // get elapsed time in ticks
  tick_t get_ticks() noexcept { return is_started() ? std::chrono::high_resolution_clock::now() : tick_t{}; }

  //  get the number of milliseconds since the timer was started
  size_t get_ms(tick_t lap) noexcept {
    return is_started()
      ? static_cast<size_t>(std::chrono::duration_cast<std::chrono::milliseconds>(lap - start_).count())
      : 0;
  }
private:
  tick_t start_;
};

}
}
#include "stopwatch.hpp"

namespace cranberries {
using stopwatch = basic_stopwatch<cranberries_magic::TimerBaseChrono>;
}


# endif
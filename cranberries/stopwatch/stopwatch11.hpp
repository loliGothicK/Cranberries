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

	namespace resolution {
		using hours = std::chrono::hours;
		using minutes = std::chrono::minutes;
		using seconds = std::chrono::seconds;
		using milliseconds = std::chrono::milliseconds;
		using microseconds = std::chrono::microseconds;
		using nanoseconds = std::chrono::nanoseconds;
		template < class >
		std::string unit();
		template < >
		std::string unit<hours>() { return "[h]"; };
		template < >
		std::string unit<minutes>() { return "[min]"; };
		template < >
		std::string unit<seconds>() { return "[sec]"; };
		template < >
		std::string unit<milliseconds>() { return "[msec]"; };
		template < >
		std::string unit<microseconds>() { return "[usec]"; };
		template < >
		std::string unit<nanoseconds>() { return "[nano]"; };

	}

namespace _detail {


template < class DurationType >
class lap_type {
	DurationType duration;
public:
	explicit lap_type(DurationType duration): duration{ duration } {}
	lap_type() = default;

	operator size_t() const {
		return duration.count();
	}

	friend std::ostream& operator<<(std::ostream& os, const lap_type& lap) {
		return os << lap.duration.count() << resolution::unit<DurationType>();
	}
};

template < class Resolution >
class TimerBaseChrono {
	using _clock = std::chrono::high_resolution_clock;
public:
  using tick_t = std::chrono::high_resolution_clock::time_point;
	using lap_t = lap_type<Resolution>;
  //  clears the timer
  TimerBaseChrono() noexcept : start_{ tick_t::min() } { }

  //  clears the timer
  void clear() noexcept { start_ = tick_t::min(); }

  //  returns true if the timer is running
  bool is_started() const noexcept { return start_ != tick_t::min(); }

  //  start the timer
  tick_t start() noexcept { return start_ = _clock::now(); }

  //	get elapsed time in ticks
  tick_t get_ticks() const noexcept { return is_started() ? _clock::now() : tick_t{}; }

	//	get tick 
	tick_t tick() const noexcept { return _clock::now(); }

  //  get the number of milliseconds since the timer was started
	lap_t get_lap(tick_t last_lap) const noexcept {
    return is_started()
      ? lap_t(std::chrono::duration_cast<Resolution>(last_lap - start_))
			: lap_t{};
  }

	// get elapsed time in ticks
	lap_t time_elapsed(tick_t _begin, tick_t _end) throw() {
		return lap_t{ std::chrono::duration_cast<Resolution>(_end - _begin) };
	}


	lap_t duration(tick_t begin_, tick_t last_) { return lap_t{ std::chrono::duration_cast<Resolution>(last_ - begin_) }; }

protected:
	tick_t begin() { return start_; }
private:
  tick_t start_;
};

}
}
#include "stopwatch.hpp"

namespace cranberries {
template < class Resolution = resolution::milliseconds >
using stopwatch = basic_stopwatch<_detail::TimerBaseChrono<Resolution>>;
}


# endif
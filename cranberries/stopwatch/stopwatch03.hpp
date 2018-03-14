/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STOPWATCH_STOPWATCH03_HPP
#define CRANBERRIES_STOPWATCH_STOPWATCH03_HPP

#include "./clock.hpp"
#include <cstddef>
#include <string>

namespace cranberries {

	namespace resolution {
		struct hours { static std::size_t count(tick_counter::tick_type b, tick_counter::tick_type e) { return tick_counter::to_hours(b,e); } };
		struct minutes { static std::size_t count(tick_counter::tick_type b, tick_counter::tick_type e) { return tick_counter::to_min(b,e); } };
		struct seconds { static std::size_t count(tick_counter::tick_type b, tick_counter::tick_type e) { return tick_counter::to_sec(b,e);  } };
		struct milliseconds { static std::size_t count(tick_counter::tick_type b, tick_counter::tick_type e) { return tick_counter::to_milli(b, e); } };
		struct microseconds { static std::size_t count(tick_counter::tick_type b, tick_counter::tick_type e) { return tick_counter::to_micro(b, e); } };

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
	}


namespace _detail {

	template < class Resolution >
	class lap_type {
		tick_counter::tick_type begin_;
		tick_counter::tick_type end_;
	public:
		explicit lap_type(tick_counter::tick_type b, tick_counter::tick_type e) : begin_( b ), end_( e ) {}
		lap_type() : begin_(), end_() {}

		operator std::size_t() const {
			return Resolution::count(begin_, end_);
		}

		friend std::ostream& operator<<(std::ostream& os, const lap_type& lap) {
			return os << Resolution::count(lap.begin_, lap.end_) << resolution::unit<Resolution>();
		}
	};


/*
    TimerBaseClock: timer base class for C++03 and eariler
    Start() sets a start time
    GetMs() returns the delta time in integer milliseconds
*/
template < class Resolution >
class TimerBaseClock {
public:
    // tick type
    typedef tick_counter::tick_type tick_t;

	typedef lap_type<Resolution> lap_t;

    // clears the timer
    TimerBaseClock() throw() { start_ = -1; }

    // clears the timer
    void clear() throw() { start_ = -1; }

    // returns true if the timer is running
    bool is_started() const throw() { return (start_ != -1); }

    // start the timer
    tick_t start() throw() { return start_ = tick_counter::tick(); }

    // get elapsed time in ticks
    tick_t get_ticks() throw() {
        return is_started() ? tick_counter::tick() - start_ : 0;
    }

		//	get tick 
		tick_t tick() const throw() { return tick_counter::tick(); }

		// get elapsed time in ticks
		lap_t time_elapsed(tick_t _begin, tick_t _end) throw() {
			return lap_t( _end - _begin );
		}

    lap_t get_lap(tick_t dt) throw() {
		return lap_t(start_, dt);
    }
    
    tick_t begin() const throw() { return start_; }
    
    lap_t duration(tick_t _begin, tick_t _end) const throw() {
        return lap_t( _begin, _end );
    }
private:
    tick_t start_;
};

}
}
#include "stopwatch.hpp"
namespace cranberries {
template < class Resolution = resolution::milliseconds >
struct stopwatch : public basic_stopwatch<_detail::TimerBaseClock<Resolution> > {
	explicit stopwatch(timer::config start) throw()
		: basic_stopwatch<_detail::TimerBaseClock<Resolution> >(start) {}
	explicit stopwatch(char const* activity = "Stopwatch", timer::config start = timer::config::auto_) throw()
		: basic_stopwatch<_detail::TimerBaseClock<Resolution> >(activity, start) {}

	stopwatch(std::ostream& log, char const* activity = "Stopwatch", timer::config start = timer::config::auto_) throw()
		: basic_stopwatch<_detail::TimerBaseClock<Resolution> >(log, activity, start) {}
};
}
# endif

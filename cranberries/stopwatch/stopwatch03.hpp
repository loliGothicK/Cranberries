/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STOPWATCH_STOPWATCH03_HPP
#define CRANBERRIES_STOPWATCH_STOPWATCH03_HPP

#include <time.h>

namespace cranberries {
namespace cranberries_magic {
/*
    TimerBaseClock: timer base class for C++03 and eariler
    Start() sets a start time
    GetMs() returns the delta time in integer milliseconds
*/
class TimerBaseClock {
public:
    // tick type
    typedef clock_t tick_t;

    // clears the timer
    TimerBaseClock() throw() { start_ = -1; }

    // clears the timer
    void clear() throw() { start_ = -1; }

    // returns true if the timer is running
    bool is_started() const throw() { return (start_ != -1); }

    // start the timer
    void Start() throw() { start_ = clock(); }

    // get elapsed time in ticks
    tick_t get_ticks() throw() {
        return is_started() ? clock() - start_ : 0;
    }

    unsigned get_ms(tick_t dt) throw() {
        return (unsigned long)((dt + (500 / CLOCKS_PER_SEC)) * (1000 / CLOCKS_PER_SEC));
    }
private:
    tick_t start_;
};

}
}
#include "stopwatch.hpp"
namespace cranberries {
typedef basic_stopwatch<cranberries_magic::TimerBaseClock> stopwatch;
}
# endif
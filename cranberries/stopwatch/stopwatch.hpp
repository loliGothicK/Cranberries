/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STOPWATCH_STOPWATCH_HPP
#define CRANBERRIES_STOPWATCH_STOPWATCH_HPP
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
namespace cranberries {

namespace _detail {
std::string to_string(int value) {
  char *buf;
  std::sprintf(buf, "%d", value);
  return std::string(buf);
}
}  // namespace _detail

namespace timer {
// for C++03 enum class emulation
class config {
 private:
  // Note that the private enum cases have underscores to differentiate
  // from the public cases
  typedef enum { manual_mode, auto_mode } config_;
  config_ value_;

 public:
  static const config auto_, manual_;
  explicit config(const config_ &value) : value_(value) {}
  operator bool() { return static_cast<bool>(this->value_); }
};

// Definitions for the public config instances
// in terms of the private enum
const config config::auto_(auto_mode);
const config config::manual_(manual_mode);
}  // namespace timer

/*  class Stopwatch -- simple stopwatch timer for optimization

    Stopwatch prints out elapsed time information for measuring the run time
    of long-running tasks. Stopwatch can also return the elapsed time without
    printing anything.

    Stopwatch is meant to be used with the RAII idiom. In its
    simplest use, the block

        {
            Stopwatch sw("TheThing()");
            TheThing();
        }

    produces two lines of output like

        TheThing(): start
        // anything printed by TheThing()
        TheThing(): stop 63[ms]

    Stopwatch is meant to replace something more complex like

    {
        timeval t1, t2;
        long dt;
        gettimeofday(&t1, NULL);
        TheThing();
        gettimeofday(&t2, NULL);
        dt = (t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 +
   t1.tv_usec); std::cout << "TheThing() took " << dt/1000 << " [ms]" <<
   std::endl;
    }

    What it prints
        ctor():                         "Stopwatch: start"
        ctor(false):                    nothing printed
        Show():                         "Stopwatch: show at xxxx [ms]"
                                        sets lap time. Get with LapGet()
        Stop():                         "Stopwatch: stop xxxx [ms]"
                                        sets lap time. Get with LapGet()
        Stop() when not running:        "Stopwatch: not started"
        Start():                        "Stopwatch: start"
                                        clears lap time
        Start() when running:           "Stopwatch: start xxxx [ms]"
                                        sets lap time. Get with LapGet()
    What it prints when activity name is specified
        ctor("activity"):               "activity: start"
        Show("theEvent"):               "activity: theEvent at xxxx [ms]"
        Stop("theEvent"):               "activity: theEvent xxxx [ms]"
                                        sets lap time. Get with LapGet()
        Stop("theEvent") not running:   "activity: not started"
        Start("theEvent") running:      "activity: theEvent xxxx [ms]"
                                        sets lap time. Get with LapGet()
        Start("theEvent") not running: "activity: theEvent"
                                        clears lap time

    printing can be suppressed (get time using LapGet()
        ctor("") or ctor(CRANBERRIES_NULLPTR)       prints nothing
        Show("") or Show(CRANBERRIES_NULLPTR)       prints nothing, sets lap
   time Start("") or Start(CRANBERRIES_NULLPTR):    prints nothing, sets lap
   time if running Stop("") or Stop(CRANBERRIES_NULLPTR):      sets lap time.
   Get with LapGet()
*/

template <typename T>
class basic_stopwatch : private T {
 public:
  typedef T BaseTimer;
  typedef typename T::tick_t tick_t;
  typedef typename T::lap_t lap_t;

  // create, optionally start timing an activity
  explicit basic_stopwatch(timer::config start) CRANBERRIES_NOEXCEPT;
  explicit basic_stopwatch(char const *activity = "Stopwatch",
                           timer::config start = timer::config::auto_)
      CRANBERRIES_NOEXCEPT;
  basic_stopwatch(std::ostream &log, char const *activity = "Stopwatch",
                  timer::config start = timer::config::auto_)
      CRANBERRIES_NOEXCEPT;

  // stop and destroy a stopwatch
  ~basic_stopwatch() CRANBERRIES_NOEXCEPT;

  // get last lap time (time of last stop)
  lap_t lap() CRANBERRIES_NOEXCEPT;

  // predicate: return true if the stopwatch is running
  bool is_started() const CRANBERRIES_NOEXCEPT;

  // show accumulated time, keep running, set/return lap
  tick_t show(char const *event = "show") CRANBERRIES_NOEXCEPT;

  // (re)start a stopwatch, set/return lap time
  tick_t start() CRANBERRIES_NOEXCEPT;

  // stop a running stopwatch, set/return lap time
  tick_t stop() CRANBERRIES_NOEXCEPT;

  template <class F>
  basic_stopwatch &elapsed(F target);

 private:  //  members
  timer::config mode_;
  char const *activity_;        // "activity" string
  tick_t lap_;                  // last lap time (time of last stop or 0)
  std::ostream &log_;           // stream on which to log events
  std::vector<tick_t> laps_;    // lap list
  std::vector<lap_t> elapsed_;  // lap list
};

//================ Member Function Definitions ================//

//  performs a Start() if start_now == true
template <typename T>
inline basic_stopwatch<T>::basic_stopwatch(timer::config start_now)
    CRANBERRIES_NOEXCEPT : mode_(start_now),
                           activity_("Stopwatch"),
                           lap_(),
                           log_(std::cout),
                           laps_(),
                           elapsed_() {
  if (static_cast<bool>(start_now)) start();
}

//  performs a start if start_now == true, suppress print by ctor("")
template <typename T>
inline basic_stopwatch<T>::basic_stopwatch(
    char const *activity, timer::config start_now) CRANBERRIES_NOEXCEPT
    : mode_(start_now),
      activity_(activity &&activity[0] ? activity : CRANBERRIES_NULLPTR),
      lap_(),
      log_(std::cout),
      laps_(),
      elapsed_() {
  if (static_cast<bool>(start_now))
    activity_ ? start() : start(CRANBERRIES_NULLPTR);
}

//  set log output, optional printout, optional start
template <typename T>
inline basic_stopwatch<T>::basic_stopwatch(
    std::ostream &log, char const *activity,
    timer::config start_now) CRANBERRIES_NOEXCEPT
    : mode_(start_now),
      activity_(activity &&activity[0] ? activity : CRANBERRIES_NULLPTR),
      lap_(),
      log_(log),
      laps_(),
      elapsed_() {
  if (static_cast<bool>(start_now))
    activity_ ? start() : start(CRANBERRIES_NULLPTR);
}

//  stop/destroy stopwatch, print message if activity was set in ctor
template <typename T>
inline basic_stopwatch<T>::~basic_stopwatch() CRANBERRIES_NOEXCEPT {
  if (static_cast<bool>(mode_))
    if (is_started()) stop();
}

//   predicate: return true if the stopwatch is running
template <typename T>
inline bool basic_stopwatch<T>::is_started() const CRANBERRIES_NOEXCEPT {
  return BaseTimer::is_started();
}

//  get the last lap time (time of last stop)
template <typename T>
inline typename basic_stopwatch<T>::lap_t basic_stopwatch<T>::lap()
    CRANBERRIES_NOEXCEPT {
  if (is_started()) {
    lap_ = BaseTimer::get_ticks();
    laps_.emplace_back(lap_);
    return BaseTimer::get_lap(lap_);
  }
  return lap_t();
}

//   show accumulated time, keep running, get/return lap time
template <typename T>
inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::show(
    char const *event_name) CRANBERRIES_NOEXCEPT {
  return is_started()
             ? ((lap_ = BaseTimer::get_ticks(),
                 ((event_name && event_name[0] ? log_ << activity_ << ": "
                                               : log_)
                  << event_name << " at " << BaseTimer::get_lap(lap_) << "[ms]"
                  << std::endl)),
                lap_)
             : activity_
                   ? (log_ << activity_ << ": not started" << std::endl, lap_)
                   : lap_;
}

//   (re)start a stopwatch, set/return lap time
template <typename T>
inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::start()
    CRANBERRIES_NOEXCEPT {
  if (is_started()) {
    stop();
  }
  laps_.clear();
  elapsed_.clear();
  return BaseTimer::start();
}

//   stop a running stopwatch and print the accumulated time
template <typename T>
inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::stop()
    CRANBERRIES_NOEXCEPT {
  if (is_started()) {
    lap_ = BaseTimer::get_ticks();

    // Log Output

    // Basic Info
    log_ << "\t";
    (activity_ ? log_ << activity_ : log_)
        << ": " << BaseTimer::get_lap(lap_) << "," << std::endl;

    // Lap Info
    if (!laps_.empty()) {
      log_ << "\tlap: [\n";
      tick_t start_ = BaseTimer::begin();
      log_ << "\t\t{\n\t\t\ttime: " << BaseTimer::duration(start_, laps_[0])
           << ",\n\t\t\tinterval: " << BaseTimer::duration(start_, laps_[0])
           << "\n\t\t},\n";
      for (size_t i = 1; i < laps_.size(); ++i) {
        log_ << "\t\t{\n\t\t\ttime"
             << ": " << BaseTimer::duration(start_, laps_[i])
             << ",\n\t\t\tinterval: "
             << BaseTimer::duration(laps_[i - 1], laps_[i]) << "\n\t\t},\n";
      }
      log_ << "\t],\n";
    }

    // Elapsed Info
    if (!elapsed_.empty()) {
      log_ << "\telapsed: [";
      log_ << "\t\t\n\t\t\t" << elapsed_[0] << ",";
      for (size_t i = 1; i < elapsed_.size(); ++i) {
        log_ << "\n\t\t\t" << elapsed_[i] << ",";
      }
      log_ << "\n\t]\n";
    }
    log_ << "}" << std::endl;
  }
  BaseTimer::clear();
  return lap_;
}

// time elapsed mesurement
template <typename T>
template <typename F>
basic_stopwatch<T> &basic_stopwatch<T>::elapsed(F target) {
  tick_t _begin = BaseTimer::tick();
  target();
  tick_t _end = BaseTimer::tick();
  elapsed_.push_back(BaseTimer::time_elapsed(_begin, _end));
  return *this;
}

}  // namespace cranberries
#endif
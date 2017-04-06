#ifndef CRANBERRIES_STOPWATCH_STOPWATCH_HPP
#define CRANBERRIES_STOPWATCH_STOPWATCH_HPP
#include <iostream>

namespace cranberries {
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
          dt = (t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec);
          std::cout << "TheThing() took " << dt/1000 << " [ms]" << std::endl;
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
          ctor("") or ctor(nullptr)       prints nothing
          Show("") or Show(nullptr)       prints nothing, sets lap time
          Start("") or Start(nullptr):    prints nothing, sets lap time if running
          Stop("") or Stop(nullptr):      sets lap time. Get with LapGet()
  */

  template < typename T >
  class basic_stopwatch
    : public T
  {
  public:
    typedef T BaseTimer;
    typedef typename T::tick_t tick_t;

    // create, optionally start timing an activity
    explicit basic_stopwatch( bool start ) throw();
    explicit basic_stopwatch( char const* activity = "Stopwatch", bool start = true ) throw();
    basic_stopwatch( std::ostream& log, char const* activity = "Stopwatch", bool start = true ) throw();

    // stop and destroy a stopwatch
    ~basic_stopwatch() throw();

    // get last lap time (time of last stop)
    tick_t lap() const throw();

    // predicate: return true if the stopwatch is running
    bool is_started() const throw();

    // show accumulated time, keep running, set/return lap
    tick_t show( char const* event = "show" ) throw();

    // (re)start a stopwatch, set/return lap time
    tick_t start( char const* event_namee = "start" ) throw();

    // stop a running stopwatch, set/return lap time
    tick_t stop( char const* event_name = "stop" ) throw();

  private:    //  members
    char const*     activity_; // "activity" string
    tick_t          lap_;      // lap time (time of last stop or 0)
    std::ostream&   log_;      // stream on which to log events
  };

  //  performs a Start() if start_now == true
  template <typename T> inline basic_stopwatch<T>::basic_stopwatch( bool start_now ) throw()
    : activity_( "Stopwatch" )
    , lap_{}
    , log_{ std::cout }
  {
    if (start_now) start();
  }

  //  performs a start if start_now == true, suppress print by ctor("")
  template <typename T> inline basic_stopwatch<T>::basic_stopwatch( char const* activity, bool start_now )  throw()
    : activity_( activity && activity[0] ? activity : nullptr )
    , lap_{}
    , log_{ std::cout }
 {
    if (start_now)
      activity_ ? start() : start( nullptr );
  }

  //  set log output, optional printout, optional start
  template <typename T> inline basic_stopwatch<T>::basic_stopwatch( std::ostream& log, char const* activity, bool start_now )  throw()
    : activity_( activity && activity[0] ? activity : nullptr )
    , lap_{}
    , log_{ log }
  {
    if (start_now)
      activity_ ? start() : start( nullptr );
  }

  //  stop/destroy stopwatch, print message if activity was set in ctor
  template <typename T> inline basic_stopwatch<T>::~basic_stopwatch() throw() {
    if (is_started())
      activity_ ? stop() : stop( nullptr );
  }

  //   predicate: return true if the stopwatch is running
  template <typename T> inline bool basic_stopwatch<T>::is_started() const throw() {
    return BaseTimer::is_started();
  }

  //  get the last lap time (time of last stop)
  template <typename T> inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::lap() const {
    return lap_;
  }

  //   show accumulated time, keep running, get/return lap time
  template <typename T> inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::show( char const* event_name ) throw() {
    return is_started() ? (( lap_ = get_ticks()
      ,( (event_name && event_name[0] ? log_ << activity_ << ": " : log_ ) << event_name << " at " << get_ms( lap_ ) << "[ms]" << std::endl))
      , lap_)
      : activity_ ? ( log_ << activity_ << ": not started" << std::endl, lap_ ) : lap_;
  }

  //   (re)start a stopwatch, set/return lap time
  template <typename T> inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::start( char const* event_name ) throw() {
    if (is_started()) {
      stop( event_name );
    }
    else {
      if (event_name && event_name[0]) {
        if (activity_)
        {
          log_ << activity_ << ": ";
        }
        log_ << event_name << std::endl;
      }
    }
    BaseTimer::start();
    return lap_;
  }

  //   stop a running stopwatch and print the accumulated time
  template <typename T>
  inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::stop( char const* event_name ) throw() {
    if (is_started()) {
      lap_ = get_ticks();
      if (event_name && event_name[0]) {
        if (activity_)
        {
          log_ << activity_ << ": ";
        }
        log_ << event_name << " " << get_ms( lap_ ) << "[ms]" << std::endl;
      }
    }
    BaseTimer::clear();
    return lap_;
  }
}
# endif
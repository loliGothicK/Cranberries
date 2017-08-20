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
          ctor("") or ctor(CRANBERRIES_NULLPTR)       prints nothing
          Show("") or Show(CRANBERRIES_NULLPTR)       prints nothing, sets lap time
          Start("") or Start(CRANBERRIES_NULLPTR):    prints nothing, sets lap time if running
          Stop("") or Stop(CRANBERRIES_NULLPTR):      sets lap time. Get with LapGet()
  */

  template < typename T >
  class basic_stopwatch
    : public T
  {
  public:
    typedef T BaseTimer;
    typedef typename T::tick_t tick_t;

    // create, optionally start timing an activity
    explicit basic_stopwatch( bool start ) CRANBERRIES_NOEXCEPT;
    explicit basic_stopwatch( char const* activity = "Stopwatch", bool start = true ) CRANBERRIES_NOEXCEPT;
    basic_stopwatch( std::ostream& log, char const* activity = "Stopwatch", bool start = true ) CRANBERRIES_NOEXCEPT;

    // stop and destroy a stopwatch
    ~basic_stopwatch() CRANBERRIES_NOEXCEPT;

    // get last lap time (time of last stop)
    tick_t lap() const CRANBERRIES_NOEXCEPT;

    // predicate: return true if the stopwatch is running
    bool is_started() const CRANBERRIES_NOEXCEPT;

    // show accumulated time, keep running, set/return lap
    tick_t show( char const* event = "show" ) CRANBERRIES_NOEXCEPT;

    // (re)start a stopwatch, set/return lap time
    tick_t start( char const* event_namee = "start" ) CRANBERRIES_NOEXCEPT;

    // stop a running stopwatch, set/return lap time
    tick_t stop( char const* event_name = "stop" ) CRANBERRIES_NOEXCEPT;

  private:    //  members
    char const*     activity_; // "activity" string
    tick_t          lap_;      // lap time (time of last stop or 0)
    std::ostream&   log_;      // stream on which to log events
  };

  //  performs a Start() if start_now == true
  template <typename T> inline basic_stopwatch<T>::basic_stopwatch( bool start_now ) CRANBERRIES_NOEXCEPT
    : activity_( "Stopwatch" )
    , lap_{}
    , log_{ std::cout }
  {
    if (start_now) start();
  }

  //  performs a start if start_now == true, suppress print by ctor("")
  template <typename T> inline basic_stopwatch<T>::basic_stopwatch( char const* activity, bool start_now )  CRANBERRIES_NOEXCEPT
    : activity_( activity && activity[0] ? activity : CRANBERRIES_NULLPTR )
    , lap_{}
    , log_{ std::cout }
  {
    if (start_now) activity_ ? start() : start( CRANBERRIES_NULLPTR );
  }

  //  set log output, optional printout, optional start
  template <typename T> inline basic_stopwatch<T>::basic_stopwatch( std::ostream& log, char const* activity, bool start_now )  CRANBERRIES_NOEXCEPT
    : activity_( activity && activity[0] ? activity : CRANBERRIES_NULLPTR )
    , lap_{}
    , log_{ log }
  {
    if (start_now) activity_ ? start() : start( CRANBERRIES_NULLPTR );
  }

  //  stop/destroy stopwatch, print message if activity was set in ctor
  template <typename T> inline basic_stopwatch<T>::~basic_stopwatch() CRANBERRIES_NOEXCEPT {
    if (is_started()) activity_ ? stop() : stop( CRANBERRIES_NULLPTR );
  }

  //   predicate: return true if the stopwatch is running
  template <typename T> inline bool basic_stopwatch<T>::is_started() const CRANBERRIES_NOEXCEPT {
    return BaseTimer::is_started();
  }

  //  get the last lap time (time of last stop)
  template <typename T> inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::lap() const CRANBERRIES_NOEXCEPT {
    return lap_;
  }

  //   show accumulated time, keep running, get/return lap time
  template <typename T> inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::show( char const* event_name ) CRANBERRIES_NOEXCEPT {
    return is_started() ? (( lap_ = BaseTimer::get_ticks()
      ,( (event_name && event_name[0] ? log_ << activity_ << ": " : log_ ) << event_name << " at " << BaseTimer::get_ms( lap_ ) << "[ms]" << std::endl))
      , lap_)
      : activity_ ? ( log_ << activity_ << ": not started" << std::endl, lap_ ) : lap_;
  }

  //   (re)start a stopwatch, set/return lap time
  template <typename T> inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::start( char const* event_name ) CRANBERRIES_NOEXCEPT {
    if (is_started()) {
      stop( event_name );
    }
    else {
      if(event_name && event_name[0])
        (activity_ ? log_ << activity_ << ": " : log_) << event_name << std::endl;
    }
    BaseTimer::start();
    return lap_;
  }

  //   stop a running stopwatch and print the accumulated time
  template <typename T>
  inline typename basic_stopwatch<T>::tick_t basic_stopwatch<T>::stop( char const* event_name ) CRANBERRIES_NOEXCEPT {
    if (is_started()) {
      lap_ = BaseTimer::get_ticks();
<<<<<<< HEAD
      if (event_name && event_name[0]) {
        if (activity_)
        {
          log_ << activity_ << ": ";
        }
        log_ << event_name << " " << get_ms( lap_ ) << "[ms]" << std::endl;
      }
=======
      if(event_name && event_name[0])
        (activity_ ? log_ << activity_ << ": " : log_)
        << event_name << " " << BaseTimer::get_ms( lap_ ) << "[ms]" << std::endl;
>>>>>>> dev
    }
    BaseTimer::clear();
    return lap_;
  }
}
# endif
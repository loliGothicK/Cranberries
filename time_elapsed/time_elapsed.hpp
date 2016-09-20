#ifndef TIME_ELAPSED_HPP
#define TIME_ELAPSED_HPP
#include <iostream>
#include <chrono>
#ifdef NO_CRANBERRIES_TIME_ELAPSED
#define CRANBERRIES_TIME_ELAPSED_NANO(...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MICRO(...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MILLI(...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_SECONS(...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MINUITES(...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_HOURS(...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_NANO_MARKER(OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MICRO_MARKER(OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MILLI_MARKER(OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_SECONS_MARKER(OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MINUITES_MARKER(OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_HOURS_MARKER(OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_NANO_REP(N,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MICRO_REP(N,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MILLI_REP(N,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_SECONS_REP(N,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MINUITES_REP(N,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_HOURS_REP(N,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_NANO_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MICRO_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MILLI_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_SECONS_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MINUITES_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_HOURS_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_no([&]{__VA_ARGS__;})
#else
#define CRANBERRIES_TIME_ELAPSED_NANO(...)    cranberries::time_elapsed<std::chrono::nanoseconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MICRO(...)    cranberries::time_elapsed<std::chrono::microseconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MILLI(...)    cranberries::time_elapsed<std::chrono::milliseconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_SECONS(...)    cranberries::time_elapsed<std::chrono::seconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MINUITES(...)    cranberries::time_elapsed<std::chrono::minutes>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_HOURS(...)    cranberries::time_elapsed<std::chrono::hours>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_NANO_MARKER(OUT,...)    cranberries::time_elapsed<std::chrono::nanoseconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_MICRO_MARKER(OUT,...)    cranberries::time_elapsed<std::chrono::microseconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_MILLI_MARKER(OUT,...)    cranberries::time_elapsed<std::chrono::milliseconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_SECONS_MARKER(OUT,...)    cranberries::time_elapsed<std::chrono::seconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_MINUITES_MARKER(OUT,...)    cranberries::time_elapsed<std::chrono::minutes>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_HOURS_MARKER(OUT,...)    cranberries::time_elapsed<std::chrono::hours>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_NANO_REP(N,...)    cranberries::time_elapsed_rep<N,std::chrono::nanoseconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MICRO_REP(N,...)    cranberries::time_elapsed_rep<N,std::chrono::microseconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MILLI_REP(N,...)    cranberries::time_elapsed_rep<N,std::chrono::milliseconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_SECONS_REP(N,...)    cranberries::time_elapsed_rep<N,std::chrono::seconds>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_MINUITES_REP(N,...)    cranberries::time_elapsed_rep<N,std::chrono::minutes>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_HOURS_REP(N,...)    cranberries::time_elapsed_rep<N,std::chrono::hours>([&]{__VA_ARGS__;})
#define CRANBERRIES_TIME_ELAPSED_NANO_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_rep<N,std::chrono::nanoseconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_MICRO_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_rep<N,std::chrono::microseconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_MILLI_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_rep<N,std::chrono::milliseconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_SECONS_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_rep<N,std::chrono::seconds>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_MINUITES_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_rep<N,std::chrono::minutes>([&]{__VA_ARGS__;},OUT)
#define CRANBERRIES_TIME_ELAPSED_HOURS_REP_MARKER(N,OUT,...)    cranberries::time_elapsed_rep<N,std::chrono::hours>([&]{__VA_ARGS__;},OUT)
#endif
namespace cranberries
{
    template < typename Resolution >
    struct resolution
    {
        static constexpr auto str = "[nano sec]";
    };
    template < >
    struct resolution<std::chrono::microseconds>
    {
        static constexpr auto str = "[micro sec]";
    };
    template < >
    struct resolution<std::chrono::milliseconds>
    {
        static constexpr auto str = "[milli sec]";
    };
    template < >
    struct resolution<std::chrono::seconds>
    {
        static constexpr auto str = "[sec]";
    };
    template < >
    struct resolution<std::chrono::minutes>
    {
        static constexpr auto str = "[min]";
    };
    template < >
    struct resolution<std::chrono::hours>
    {
        static constexpr auto str = "[h]";
    };

    template < typename Resolution = std::chrono::milliseconds, typename F >
    inline void time_elapsed( F&& body, std::ostream& o = std::cout )
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        body();
        auto t1 = std::chrono::high_resolution_clock::now();

        o << std::chrono::duration_cast<Resolution>( t1 - t0 ).count() << resolution<Resolution>::str << std::endl;
    }
    template < std::size_t N, typename Resolution = std::chrono::milliseconds, typename F >
    inline void time_elapsed_rep( F&& body, std::ostream& o = std::cout )
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        for ( std::size_t i{}; i < N; ++i )
            body();
        auto t1 = std::chrono::high_resolution_clock::now();

        o << std::chrono::duration_cast<Resolution>( t1 - t0 ).count() << resolution<Resolution>::str << std::endl;
    }

    template < typename F >
    void time_elapsed_no( F&& body )
    {
        body();
    }
}

#endif

#ifndef CRANBERRIES_STOPWATCH_HPP
#define CRANBERRIES_STOPWATCH_HPP

#if defined(_MSC_VER)
#   if _MSC_VER < 1800 
#include "./stopwatch/stopwatch03.hpp"
#   else
#include "./stopwatch/stopwatch11.hpp"
#   endif
#elif __cplusplus <= 199711L
#include "./stopwatch/stopwatch03.hpp"
#else
#include "./stopwatch/stopwatch11.hpp"
#endif

#undef IF_CPP11
# endif
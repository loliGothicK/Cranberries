/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STOPWATCH_HPP
#define CRANBERRIES_STOPWATCH_HPP

#if defined(_MSC_VER)
#   if _MSC_VER < 1800 
#define CRANBERRIES_NOEXCEPT throw
#define CRANBERRIES_NULLPTR NULL
#include "./stopwatch/stopwatch03.hpp"
#   else
#define CRANBERRIES_NOEXCEPT noexcept
#define CRANBERRIES_NULLPTR nullptr
#include "./stopwatch/stopwatch11.hpp"
#   endif
#elif __cplusplus <= 199711L
#define CRANBERRIES_NOEXCEPT throw
#define CRANBERRIES_NULLPTR NULL
#include "./stopwatch/stopwatch03.hpp"
#else
#define CRANBERRIES_NOEXCEPT noexcept
#define CRANBERRIES_NULLPTR nullptr
#include "./stopwatch/stopwatch11.hpp"
#endif

#undef CRANBERRIES_NOEXCEPT
# endif
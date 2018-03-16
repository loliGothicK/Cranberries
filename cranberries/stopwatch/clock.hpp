#pragma once
/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STOPWATCH_CLOCK_HPP
#define CRANBERRIES_STOPWATCH_CLOCK_HPP

#include <cstddef>
#include <string>

#ifdef _WIN32
#include "./clock/QPC03.hpp"
#elif
#include "./clock/posix03.hpp"
#endif

#endif
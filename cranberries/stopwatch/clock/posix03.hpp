#pragma once
#include <time.h>

namespace crnberries {
namespace tick_counter {

typedef timespec tick_type;

tick_type tick() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  return spec;
}

unsigned long long to_micro(tick_type _begin, tick_type _end) {
  return (static_cast<unsigned long long>(_end.tv_sec) -
          static_cast<unsigned long long>(_begin.tv_sec)) *
             1000000 +
         (static_cast<unsigned long long>(_end.tv_nsec) -
          static_cast<unsigned long long>(_begin.tv_nsec)) /
             1000;
}

unsigned long long to_milli(tick_type _begin, tick_type _end) {
  return (static_cast<unsigned long long>(_end.tv_sec) -
          static_cast<unsigned long long>(_begin.tv_sec)) *
             1000 +
         (static_cast<unsigned long long>(_end.tv_nsec) -
          static_cast<unsigned long long>(_begin.tv_nsec)) /
             10000000;
}

unsigned long long to_sec(tick_type _begin, tick_type _end) {
  return (static_cast<unsigned long long>(_end.tv_sec) -
          static_cast<unsigned long long>(_begin.tv_sec));
}

unsigned long long to_min(tick_type _begin, tick_type _end) {
  return (static_cast<unsigned long long>(_end.tv_sec) -
          static_cast<unsigned long long>(_begin.tv_sec)) /
         60
}

unsigned long long to_hours(tick_type _begin, tick_type _end) {
  return (static_cast<unsigned long long>(_end.tv_sec) -
          static_cast<unsigned long long>(_begin.tv_sec)) /
         360
}

}  // namespace tick_counter
}  // namespace crnberries
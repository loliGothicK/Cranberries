#pragma once
#include <Windows.h>
#include <mmsystem.h>
#undef max
#undef min
namespace cranberries {
namespace tick_counter {

typedef LARGE_INTEGER tick_type;

tick_type tick() {
  tick_type filetime;
  QueryPerformanceCounter(&filetime);
  return filetime;
}

unsigned long long to_micro(tick_type _begin, tick_type _end) {
  tick_type Elapsed, Frequency;
  QueryPerformanceFrequency(&Frequency);
  Elapsed.QuadPart = _end.QuadPart - _begin.QuadPart;
  return static_cast<unsigned long long>(Elapsed.QuadPart * 1000000 /
                                         Frequency.QuadPart);
}

unsigned long long to_milli(tick_type _begin, tick_type _end) {
  tick_type Elapsed, Frequency;
  QueryPerformanceFrequency(&Frequency);
  Elapsed.QuadPart = _end.QuadPart - _begin.QuadPart;
  return static_cast<unsigned long long>(Elapsed.QuadPart * 1000 /
                                         Frequency.QuadPart);
}

unsigned long long to_sec(tick_type _begin, tick_type _end) {
  tick_type Elapsed, Frequency;
  QueryPerformanceFrequency(&Frequency);
  Elapsed.QuadPart = _end.QuadPart - _begin.QuadPart;
  return static_cast<unsigned long long>(Elapsed.QuadPart / Frequency.QuadPart);
}

unsigned long long to_min(tick_type _begin, tick_type _end) {
  tick_type Elapsed, Frequency;
  QueryPerformanceFrequency(&Frequency);
  Elapsed.QuadPart = _end.QuadPart - _begin.QuadPart;
  return static_cast<unsigned long long>(Elapsed.QuadPart / 60. /
                                         Frequency.QuadPart);
}

unsigned long long to_hours(tick_type _begin, tick_type _end) {
  tick_type Elapsed, Frequency;
  QueryPerformanceFrequency(&Frequency);
  Elapsed.QuadPart = _end.QuadPart - _begin.QuadPart;
  return static_cast<unsigned long long>(Elapsed.QuadPart / 360. /
                                         Frequency.QuadPart);
}

}  // namespace tick_counter
}  // namespace cranberries

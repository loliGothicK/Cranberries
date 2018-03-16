/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_UTILITY_HPP
#define CRANBERRIES_INTERVAL_LIB_UTILITY_HPP

#include <iterator>
#include <tuple>
#include <utility>
#include "interval.hpp"

namespace cranberries {

template <typename T>
inline constexpr interval<T> hull(T &&low, T &&up) {
  return CRANBERRIES_INVALID_ARGUMENT_THROW_CONDITIONAL_WITH_MSG(
      low > up, "upper_bound less than lower_bound!")
      : make_interval([&] { return std::forward<T>(low); },
                      [&] { return std::forward<T>(up); });
}

template <typename T>
inline constexpr interval<T> hull(T &&x) noexcept {
  return interval<T>{std::forward<T>(x)};
}

template <typename T>
inline constexpr T mid(interval<T> const &x) noexcept {
  return x.upper() + (x.upper() - x.lower()) / static_cast<T>(2.0L);
}

template <typename T>
inline constexpr T middle(interval<T> const &x) noexcept {
  return x.upper() + (x.upper() - x.lower()) / static_cast<T>(2.0L);
}

/*  width  */

template <typename T>
inline constexpr T wid(interval<T> const &x) noexcept {
  return x.upper() - x.lower();
}

template <typename T>
inline constexpr T width(interval<T> const &x) noexcept {
  return x.upper() - x.lower();
}

template <typename T>
inline constexpr T rad(interval<T> const &x) noexcept {
  return x.upper() - x.lower() / static_cast<T>(2.0L);
}

template <typename T>
inline constexpr T norm(interval<T> const &x) noexcept {
  using std::abs;
  auto &&l = abs(x.lower());
  auto &&r = abs(x.upper());
  return l < r ? r : l;
}

template <typename T>
inline constexpr T mag(interval<T> const &x) noexcept {
  using std::abs;
  auto &&l = abs(x.lower());
  auto &&r = abs(x.upper());
  return l < r ? r : l;
}

template <typename T>
inline constexpr T mig(interval<T> const &x) noexcept {
  using std::abs;
  auto &&l = abs(x.lower());
  auto &&r = abs(x.upper());
  return l < r ? l : r;
}

template <typename T>
inline constexpr bool in(typename interval<T>::value_type const &x,
                         interval<T> const &y) noexcept {
  return y.lower() < x && x < y.upper() ? true : false;
}

template <typename T>
inline constexpr bool subset(interval<T> const &x,
                             interval<T> const &y) noexcept {
  return y.lower() <= x.lower() && x.lower() <= y.upper() ? true : false;
}

template <typename T>
inline constexpr bool proper_subset(interval<T> const &x,
                                    interval<T> const &y) noexcept {
  return y.lower() < x.lower() && x.lower() < y.upper() ? true : false;
}

template <typename T>
inline constexpr bool overlap(interval<T> const &x,
                              interval<T> const &y) noexcept {
  return x.lower() < y.upper() || y.lower() < x.upper() ? true : false;
}

template <typename T>
inline constexpr interval<T> intersect(interval<T> const &x,
                                       interval<T> const &y) {
  return x.lower() < y.lower() && y.upper() < x.upper()
             ? interval<T>(y)
             : y.lower() < x.lower() && x.upper() < y.upper()
                   ? interval<T>(x)
                   : x.lower() < y.upper()
                         ? interval<T>{x.lower(), y.upper()}
                         : y.lower() < x.upper()
                               ? interval<T>{y.lower(), x.upper()}
                               : CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG(
                                     "x and y do not overlap each other.");
}

/*  This function returns two interval subparts just split middle point as
 * vector<interval<T>> */
template <class T>
inline std::pair<interval<T>, interval<T>> subpart(interval<T> &x) noexcept {
  return std::make_pair(interval<T>{x.lower(), x.mid()},
                        interval<T>{x.mid(), x.upper()});
}

}  // namespace cranberries

#endif

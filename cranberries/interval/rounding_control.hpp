/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#define CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#include "../common/exception.hpp"
#include "../common/macros.hpp"
#include "interval.hpp"

/*
workaround for MSVC below
[ Note : Before Visual Studio 2017, FE_DOWNWARD and FE_UPWARD are reverse
defined in MSVC. - end note]
*/
#if defined(_MSC_VER) && _MSC_FULL_VER < 191025017
#define UPWARD_POLICY std::fesetround(FE_DOWNWARD)
#define DOWNWARD_POLICY std::fesetround(FE_UPWARD)
#else
#define UPWARD_POLICY std::fesetround(FE_UPWARD)
#define DOWNWARD_POLICY std::fesetround(FE_DOWNWARD)
#endif

namespace cranberries {
template <class L, class U,
          typename T =
              std::common_type_t<std::result_of_t<L()>, std::result_of_t<U()>>>
constexpr auto make_interval(L &&low, U &&up) -> interval<T> {
  DOWNWARD_POLICY;
  T lower = low();
  UPWARD_POLICY;
  T upper = up();
  return {lower, upper};
}

namespace cranberries_magic {
template <typename T>
inline constexpr auto downward_multiply(T x, T y) noexcept {
  DOWNWARD_POLICY;
  auto result = x * y;
  return result;
}

template <typename T>
inline constexpr auto upward_multiply(T x, T y) noexcept {
  UPWARD_POLICY;
  auto result = x * y;
  return result;
}
}  // namespace cranberries_magic
}  // namespace cranberries

#endif  // ! SET_ROUND_HPP

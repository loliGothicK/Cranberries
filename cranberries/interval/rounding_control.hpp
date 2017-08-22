/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#define CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#include "../common/exception.hpp"
#include "interval.hpp"
#include "../common/macros.hpp"

#ifdef CRANBERRIES_INTERVAL_DEBUG_MODE
#define CRANBERRIES_MAKE_INTERVAL(TYPE, LOW_EXPR, UP_EXPR) [&]{\
  DOWNWARD_POLICY;\
  auto lower_p_cranberries_unique_0000 = LOW_EXPR;\
  UPWARD_POLICY;\
  auto upper_p_cranberries_unique_0000 = UP_EXPR;\
  return  upper_p_cranberries_unique_0000 < lower_p_cranberries_unique_0000\
    ? throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, "up < low" )\
    : interval<TYPE>(lower_p_cranberries_unique_0000,upper_p_cranberries_unique_0000);\
}()
#else
#define CRANBERRIES_MAKE_INTERVAL(TYPE, LOW_EXPR, UP_EXPR) [&]{\
  DOWNWARD_POLICY;\
  auto lower_p_cranberries_unique_0000 = LOW_EXPR;\
  UPWARD_POLICY;\
  auto upper_p_cranberries_unique_0000 = UP_EXPR;\
  return interval<TYPE>(lower_p_cranberries_unique_0000,upper_p_cranberries_unique_0000);\
}()
#endif

/*
workaround for MSVC below
[ Note : Before Visual Studio 2017, FE_DOWNWARD and FE_UPWARD are reverse defined in MSVC. - end note]
*/
#if defined(_MSC_VER) && _MSC_FULL_VER < 191025017
#define UPWARD_POLICY std::fesetround(FE_DOWNWARD)
#define DOWNWARD_POLICY std::fesetround(FE_UPWARD)
#else
#define UPWARD_POLICY std::fesetround(FE_UPWARD)
#define DOWNWARD_POLICY std::fesetround(FE_DOWNWARD)
#endif


namespace cranberries {
namespace cranberries_magic {
  template < typename T >
  inline
  constexpr auto downward_multiply(T x, T y) noexcept {
    DOWNWARD_POLICY;
    auto result = x*y;
    return result;
  }

  template < typename T >
  inline
  constexpr auto upward_multiply(T x, T y) noexcept {
    UPWARD_POLICY;
    auto result = x*y;
    return result;
  }
}
}


#endif // ! SET_ROUND_HPP

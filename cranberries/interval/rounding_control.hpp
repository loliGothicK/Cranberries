#ifndef CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#define CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#include "exception.hpp"
#include "interval.hpp"
#include "../common/macros.hpp"

#if defined(CRANBERRIES_INTERVAL_DEBUG_MODE)
#define CRANBERRIES_MAKE_INTERVAL(TYPE, LOW_EXPR, UP_EXPR) [&]{\
  DOWNWARD_POLICY;\
  auto low = LOW_EXPR;\
  UPWARD_POLICY;\
  auto up = UP_EXPR;\
  return  up < low ? throw cranberries::invalid_argument( "invalid_argument", __FILE__, __FUNCTION__, __LINE__, "up < low" ) : interval<TYPE>(low,up);\
}()
#else
#define CRANBERRIES_MAKE_INTERVAL(TYPE, LOW_EXPR, UP_EXPR) [&]{\
  DOWNWARD_POLICY;\
  auto low = LOW_EXPR;\
  UPWARD_POLICY;\
  auto up = UP_EXPR;\
  return interval<TYPE>(low,up);\
}()
#endif

/*
workaround for MSVC below
[ Note : FE_DOWNWARD and FE_UPWARD are reverse defined in MSVC. - end note]
*/
#ifdef _MSC_VER
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

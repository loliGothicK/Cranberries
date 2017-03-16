#ifndef CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#define CRANBERRIES_INTERVAL_LIB_ROUNDING_CONTROL_HPP
#include "exception.hpp"
#include "../common/macros.hpp"
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
namespace cranberries {
namespace cranberries_magic {
  template < typename T >
  inline
  constexpr auto make_interval_with_accuracy_assurance(T lower, T upper) noexcept {
    DOWNWARD_POLICY;
    auto l = lower;
    UPWARD_POLICY;
    auto r = upper;
    return interval<T>{ l, r };
  }

}
}


#endif // ! SET_ROUND_HPP

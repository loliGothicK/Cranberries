/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
 
#ifndef CRANBERRIES_FUNC_UTIL_APPLY_PIPE_HPP
#define CRANBERRIES_FUNC_UTIL_APPLY_PIPE_HPP

#  ifdef _MSC_VER
#include "func_util_detail/function_utility_for_msvc.hpp"
#  else
#include "func_util_detail/function_utility_for_gcc_clang.hpp"
#  endif // _MSC_VER

namespace cranberries {
namespace literals {
  template<char... Chars>
  constexpr func_util::randomized<to_decimal<Chars...>::value> 
  operator"" _randomized() {
    return {};
  }
  using cranberries::literals::operator"" _randomized;
}	// ! namespace literals
} // ! namespace cranberries

#endif
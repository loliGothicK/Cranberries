/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
 
#ifndef CRANBERRIES_FUNC_UTIL_APPLY_PIPE_HPP
#define CRANBERRIES_FUNC_UTIL_APPLY_PIPE_HPP

#include "func_util_detail/function_utility_core.hpp"

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
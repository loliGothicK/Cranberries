#ifndef CRANBERRIES_FUNCTION_UTILITY_HPP
#define CRANBERRIES_FUNCTION_UTILITY_HPP

#  ifdef _MSC_VER
#include "func_util_detail/function_utility_for_msvc.hpp"
#  else
#include "func_util_detail/function_utility_for_gcc_clang.hpp"
#  endif // _MSC_VER

#endif // CRANBERRIES_FUNCTION_UTILITY_HPP
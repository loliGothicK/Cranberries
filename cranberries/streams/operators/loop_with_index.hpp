/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef LOOP_WITH_INDEX_HPP
#define LOOP_WITH_INDEX_HPP
#include <utility>
#include "../utility.hpp"

#define tranform_with_index(index, ...)\
transform(\
[index = 0]( auto&& cranberries_arg )mutable{\
  return cranberries::make_finally( [ & ] { ++index; } ),\
    __VA_ARGS__(\
     std::forward<\
      decltype(cranberries_arg)\
     >( cranberries_arg )\
    ); }\
)

#define transformed_with_index(index, ...)\
cranberries::streams::transformed(\
[index = 0]( auto&& cranberries_arg )mutable{\
  return cranberries::make_finally( [ & ] { ++index; } ),\
    __VA_ARGS__(\
     std::forward<\
      decltype(cranberries_arg)\
     >( cranberries_arg )\
    ); }\
)

#endif
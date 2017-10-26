#ifndef CRANBERRIES_RANGES_RANGE_PIPE_HPP
#define CRANBERRIES_RANGES_RANGE_PIPE_HPP
#include "ranges_tag.hpp"
#include "../../type_traits.hpp"
#include "../../detection_toolkit.hpp"

namespace cranberries {
namespace experimental {
namespace ranges {


template < class Range,
           class Adaptor,
           enabler_t<is_callable_v<Adaptor(Range&&)>> = nullptr,
           range_require<Range> = nullptr>
decltype(auto) operator| (Range&& range, Adaptor&& adaptor)
{
  return adaptor(std::forward<Range>(range));
}

template < class Target, class Range,
           range_require<Range> = nullptr >
decltype(auto) operator|=(Target&& target, Range&& range) {
  target.clear();
  target.reserve(range.size());
  for (const auto& e : range) target.emplace_back(e);
  return std::forward<Target>(target);
}


}}}
#endif
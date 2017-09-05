#ifndef CRANBERRIES_RANGES_RANGE_PIPE_HPP
#define CRANBERRIES_RANGES_RANGE_PIPE_HPP
#include "ranges_tag.hpp"
#include "../../type_traits.hpp"

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range,
           class Adaptor,
           enabler_t<is_action_adaptor_v<std::decay_t<Adaptor>>> = nullptr >
  decltype(auto) operator| (Range&& range, Adaptor&& adaptor)
{
  return adaptor.apply(std::forward<Range>(range));
}

template < class Range,
           class AdaptorProxy,
           enabler_t<is_adaptor_proxy_v<std::decay_t<AdaptorProxy>>> = nullptr >
  decltype(auto) operator| (Range&& range, AdaptorProxy&& proxy)
{
  return proxy.make_adaptor(std::forward<Range>(range));
}


}}}
#endif
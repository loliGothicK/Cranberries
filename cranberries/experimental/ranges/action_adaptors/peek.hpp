#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_PEEK_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_PEEK_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {


template < class F >
class Peek
  : private tag::adaptor_proxy_tag
{
  F f;
public:
  Peek(F f) : f{ f } {}
  template < class Range >
  decltype(auto) adapt_to(Range&& range) {
    for (const auto& e : range) f(e);
    return std::forward<Range>(range);
  }
};

namespace action {
  template < typename F >
  Peek<F> peek(F&& f) { return { std::forward<F>(f) }; }
}

}}}
#endif
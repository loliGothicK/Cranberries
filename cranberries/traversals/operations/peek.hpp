#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_PEEK_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_PEEK_HPP
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class F>
class Peek : private tag::adaptor_tag {
  F f;

 public:
  Peek(F f) : f{f} {}

  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires = std::enable_if_t<conjunction_v<
      concepts::required<Range, concepts::iterable>,
      always_t<decltype(std::declval<F>()(std::declval<const ElemType &>()))>>>;

  template <class Range>
  decltype(auto) operator()(Range &&traversal) {
    for (const auto &e : traversal) f(e);
    return std::forward<Range>(traversal);
  }
};

namespace operation {
template <class F>
Peek<F> peek(F &&f) {
  return {std::forward<F>(f)};
}

template <class Range, class Func>
decltype(auto) peek(Range &&traversal, Func &&func) {
  for (const auto &e : traversal) func(e);
  return std::forward<Range>(traversal);
}
}  // namespace operation

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
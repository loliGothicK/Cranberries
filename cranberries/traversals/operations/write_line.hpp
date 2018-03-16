#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
#include <iostream>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

class WriteLine : private tag::adaptor_tag, private tag::action_tag {
  std::ostream &os;

 public:
  WriteLine() noexcept : os(std::cout) {}
  explicit WriteLine(std::ostream &os) noexcept : os(os) {}

  template <class Traversable>
  using requires = typename concepts::iterable::template requires<Traversable>;

  template <class SentinelRange,
            std::enable_if_t<
                !is_tuple_v<typename remove_cvr_t<SentinelRange>::value_type>,
                std::nullptr_t> = nullptr>
  decltype(auto) operator()(SentinelRange &&traversal) {
    using std::begin;
    using std::end;

    auto iter = begin(traversal);
    os << *iter;
    ++iter;
    for (; iter != end(traversal); ++iter) os << " " << *iter;
    os << std::endl;
    return std::forward<SentinelRange>(traversal);
  }

  template <class SentinelRange,
            std::enable_if_t<
                is_tuple_v<typename remove_cvr_t<SentinelRange>::value_type>,
                std::nullptr_t> = nullptr>
  decltype(auto) operator()(SentinelRange &&traversal) {
    auto printf_ = [](auto &&head, auto &&... tail) -> void {
      std::cout << "[" << head;
      (void)std::initializer_list<int>{(void(std::cout << "," << tail), 0)...};
      std::cout << "]";
    };

    auto iter = std::begin(traversal);
    cranberries::apply(printf_, *iter);
    ++iter;
    for (; iter != std::end(traversal); ++iter) {
      os << " ";
      cranberries::apply(printf_, *iter);
    }
    os << std::endl;
    return std::forward<SentinelRange>(traversal);
  }
};

namespace operation {
inline WriteLine write_line(std::ostream &os = std::cout) noexcept {
  return WriteLine{os};
}
}  // namespace operation

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

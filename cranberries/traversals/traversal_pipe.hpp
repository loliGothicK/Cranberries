#ifndef CRANBERRIES_RANGES_RANGE_PIPE_HPP
#define CRANBERRIES_RANGES_RANGE_PIPE_HPP
#include "../common/concepts.hpp"
namespace cranberries {
inline namespace experimental {
namespace traversals {

namespace _detail {

template <class Trav, class, class = void>
struct trav_require : std::is_base_of<forward_traversal, remove_cvr_t<Trav>> {};

template <class Trav, class X>
struct trav_require<Trav, X,
                    cranberries::void_t<typename remove_cvr_t<
                        X>::template requires<remove_cvr_t<Trav>>>>
    : std::true_type {};

}  // namespace _detail

template <class Range, class Adaptor,
          enabler_t<conjunction_v<concepts::required<Range, concepts::iterable>,
                                  is_adaptor<std::decay_t<Adaptor>>>> = nullptr,
          enabler_t<_detail::trav_require<Range, Adaptor>::value> = nullptr>
decltype(auto) operator<=(Range &&traversal, Adaptor &&adaptor) noexcept(
    noexcept(std::forward<Adaptor>(adaptor)(std::forward<Range>(traversal)))) {
  return std::forward<Adaptor>(adaptor)(std::forward<Range>(traversal));
}

template <
    class Target, class Range,
    concepts::concept_required<Range, concepts::iterable> = nullptr,
    std::enable_if_t<
        std::is_constructible<typename std::decay_t<Range>::value_type,
                              typename std::decay_t<Target>::value_type>::value,
        std::nullptr_t> = nullptr>
decltype(auto) operator|=(Target &&target, Range &&traversal) {
  target.clear();
  target.reserve(traversal.size());
  for (const auto &e : traversal) target.emplace_back(e);
  return std::forward<Target>(target);
}

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
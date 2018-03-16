#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_FOR_EACH_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_FOR_EACH_HPP
#include <memory>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Range, class F>
class ForEach : private tag::sentinel_traversal_tag {
  class forward_sentinel {
    Range traversal_;
    F f_;
    typename std::decay_t<Range>::iterator iter;

   public:
    using value_type = typename std::decay_t<Range>::value_type;
    forward_sentinel(Range traversal, F f)
        : traversal_{traversal},
          f_(f),
          iter{cranberries::back_magic::begin(traversal_)} {}

    auto get() {
      return [&] {
        auto v = *iter;
        f_(v);
        return v;
      }();
    }
    void next() { ++iter; }
    bool is_end() { return iter == cranberries::back_magic::end(traversal_); }
  };

  class reverse_sentinel {
    Range traversal_;
    F f_;
    typename std::decay_t<Range>::iterator iter;

   public:
    using value_type =
        std::result_of_t<F(typename std::decay_t<Range>::value_type)>;
    reverse_sentinel(Range traversal, F f)
        : traversal_{traversal},
          f_(f),
          iter{cranberries::back_magic::rbegin(traversal_)} {}

    auto get() {
      return [&] {
        auto v = *iter;
        f_(v);
        return v;
      }();
    }
    void next() { ++iter; }
    bool is_end() { return iter == cranberries::back_magic::rend(traversal_); }
  };

 public:
  using value_type =
      std::result_of_t<F(typename std::decay_t<Range>::value_type)>;
  using forward_sentinel = forward_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_sentinel = reverse_sentinel;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  ForEach(Range traversal, F f) : traversal{traversal}, f(f) {}

  iterator begin() const {
    return {std::make_unique<forward_sentinel>(traversal, f)};
  }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const {
    return {std::make_unique<reverse_sentinel>(traversal, f)};
  }
  reverse_iterator rend() const { return {}; }

 private:
  Range traversal;
  F f;
};

namespace view {
template <typename F>
auto for_each(F &&f) {
  return make_proxy<ForEach>::embedded(std::forward<F>(f));
}
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
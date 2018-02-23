#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Drop
  : private tag::sentinel_traversal_tag
{
  class sentinel_impl {
    Range traversal_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range traversal, size_t n)
      : traversal_{ traversal }
      , iter{ cranberries::back_magic::begin(traversal_) }
      , last{ cranberries::back_magic::end(traversal_) }
    {
      do { ++iter; } while (iter != last && 0 < --n);
    }

    auto get() { return *iter; }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  };
public:
  using value_type = typename std::decay_t<Range>::value_type;
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  Drop(Range traversal, size_t n)
    : traversal{ traversal }
    , n(n) {}

  iterator begin() const { return { std::make_unique<sentinel>(traversal, n) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range traversal;
  long long int n;
};

namespace view {
  inline auto drop(size_t n) noexcept { return make_proxy<Drop>::forward(n); }
}

}}}
#endif
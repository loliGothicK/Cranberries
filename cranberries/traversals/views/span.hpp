#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_SPAN_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_SPAN_HPP
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class, bool>
class Span;

template <class Range>
class Span<Range, true> : private tag::sentinel_traversal_tag {
  class forward_sentinel {
    Range traversal_;
    size_t offset, count, index;
    typename std::decay_t<Range>::iterator iter;

   public:
    using value_type = typename std::decay_t<Range>::value_type;

    forward_sentinel(Range traversal, size_t offset, size_t count) noexcept
        : traversal_{traversal},
          offset{offset},
          count{count},
          index{},
          iter{::cranberries::back_magic::begin(traversal_)} {
      repeat(offset, [&] { ++iter; });
    }

    forward_sentinel(const forward_sentinel &) noexcept {}

    auto get() const noexcept { return *iter; }
    void next() noexcept { ++index, ++iter; }
    bool is_end() const noexcept { return index == count; }
  };

 public:
  using value_type = typename std::decay_t<Range>::value_type;
  using forward_sentinel = forward_sentinel;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using constraints =
      std::enable_if_t<concepts::required_v<Range, concepts::iterable>>;

  Span(Range traversal, size_t offset, size_t count) noexcept
      : traversal{traversal}, offset{offset}, count{count} {}

  iterator begin() const noexcept {
    return {std::make_unique<forward_sentinel>(traversal, offset, count)};
  }
  iterator end() const noexcept { return {}; }
  reverse_iterator rbegin() const noexcept {
    return {std::make_unique<reverse_sentinel>(*this)};
  }
  reverse_iterator rend() const noexcept { return {}; }

 private:
  Range traversal;
  size_t offset, count;
};

template <class Range>
class Span<Range, false> : private tag::sentinel_traversal_tag {
  class forward_sentinel {
    Range traversal_;
    size_t offset, count, index;
    typename std::decay_t<Range>::iterator iter;

   public:
    using value_type = typename std::decay_t<Range>::value_type;

    forward_sentinel(Range traversal, size_t offset, size_t count) noexcept
        : traversal_{traversal},
          offset{offset},
          count{count},
          index{},
          iter{::cranberries::back_magic::begin(traversal_)} {
      repeat(offset, [&] { ++iter; });
    }

    forward_sentinel(const forward_sentinel &) noexcept {}

    auto get() const noexcept { return *iter; }
    void next() noexcept { ++index, ++iter; }
    bool is_end() const noexcept { return index == count; }
  };

 public:
  using value_type = typename std::decay_t<Range>::value_type;
  using forward_sentinel = forward_sentinel;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using constraints =
      std::enable_if_t<concepts::required_v<Range, concepts::iterable>>;

  Span(Range traversal, size_t offset, size_t count) noexcept
      : traversal{traversal}, offset{offset}, count{count} {}

  iterator begin() const noexcept {
    return {std::make_unique<forward_sentinel>(traversal, offset, count)};
  }
  iterator end() const noexcept { return {}; }
  reverse_iterator rbegin() const noexcept {
    return std::make_unique<reverse_iterator>(*this);
  }
  reverse_iterator rend() const noexcept { return {}; }

 private:
  Range traversal;
  size_t offset, count;
};

struct SpanProxy {
  size_t offset, count;

  template <class Traversable>
  using requires = concepts::iterable::requires<Traversable>;

  template <class Range>
  Span<Range, concepts::required_v<Range, concepts::reverse_iterable>>
  operator()(Range &&traversal) const noexcept {
    return {std::forward<Range>(traversal), offset, count};
  }
};

namespace view {
template <typename Range>
Span<Range, concepts::required_v<Range, concepts::reverse_iterable>> span(
    Range traversal, size_t offset, size_t count) noexcept {
  return {std::forward<Range>(traversal), offset, count};
}

inline SpanProxy span(size_t offset, size_t count) noexcept {
  return {offset, count};
}
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

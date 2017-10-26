#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"


namespace cranberries {
namespace experimental {
namespace ranges {


template < class Range >
class Cyclic
  : private tag::sentinel_range_tag
{
  class forward_sentinel {
    Range range_;
    typename std::decay_t<Range>::iterator iter;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    forward_sentinel(Range range) : range_{ range }, iter{ back_magic::begin(range_) } {}
    forward_sentinel(const forward_sentinel&) = default;

    auto get() { return *iter; }
    void next() { if (++iter == back_magic::end(range_)) iter = back_magic::begin(range_); }
    std::false_type is_end() { return {}; }
  };

  class reverse_sentinel {
    Range range_;
    typename std::decay_t<Range>::reverse_iterator iter;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    reverse_sentinel(Range range) : range_{ range }, iter{ back_magic::rbegin(range_) } {}
    reverse_sentinel(const reverse_sentinel&) = default;

    auto get() { return *iter; }
    void next() { if (++iter == back_magic::rend(range_) ) iter = back_magic::rbegin(range_); }
    std::false_type is_end() { return {}; }
  };

public:
  using forward_sentinel = forward_sentinel;
  using reverse_sentinel = reverse_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;
  using value_type = typename std::decay_t<Range>::value_type;

  Cyclic(Range range) : range{ range } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(range) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(range) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
};

namespace view {
  template < typename Range >
  Cyclic<Range> cyclic(Range range) { return {std::forward<Range>(range)}; }

  auto cyclic() { return [](auto&& range) { cyclic(std::forward<decltype(range)>(range)); }; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

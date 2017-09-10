#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_REVERSE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_REVERSE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../../../detection_toolkit.hpp"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Reverse
  : private tag::sentinel_range_tag
{
  class forward_sentinel {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    forward_sentinel(Range range)
      : range_{ range }
      , iter{ cranberries::back_magic::rbegin(range_) }
      , last{ cranberries::back_magic::rend(range_) }
    {}

    auto get() { return *iter; }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  private:
    Range range_;
    typename std::decay_t<Range>::reverse_iterator iter;
    typename std::decay_t<Range>::reverse_iterator last;
  };

  class reverse_sentinel {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    reverse_sentinel(Range range)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
    {}

    auto get() { return *iter; }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  private:
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
  };

public:
  using forward_sentinel = forward_sentinel;
  using reverse_sentinel = reverse_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;
  using value_type = typename std::decay_t<Range>::value_type;

  Reverse(Range range)
    : range{ range } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(range) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(range) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
};

class ReverseProxy
  : private tag::adaptor_proxy_tag
{
public:
  ReverseProxy() = default;
  template < class Range, reversable_range_require<Range> = nullptr >
  Reverse<Range> adapt_to(Range&& range) {
    return { std::forward<Range>(range) };
  }
};

namespace action {
  ReverseProxy reverse() { return {}; }
}

}}}
#endif
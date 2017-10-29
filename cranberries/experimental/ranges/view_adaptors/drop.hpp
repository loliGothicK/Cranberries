#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Drop
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range range, size_t n)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
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

  Drop(Range range, size_t n)
    : range{ range }
    , n(n) {}

  iterator begin() const { return { std::make_unique<sentinel>(range, n) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
  long long int n;
};

class DropProxy
  : private tag::adaptor_proxy_tag
{
  size_t n;
public:
  DropProxy(size_t n) : n{ n } {}
  template < class Range >
  Drop<Range> operator()(Range&& range) {
    return { std::forward<Range>(range), n };
  }
};

namespace view {
  DropProxy drop(size_t n) { return { n }; }
}

}}}
#endif
#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <tuple>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Indexed
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    size_t idx;
  public:
    using value_type = std::tuple<size_t, typename std::decay_t<Range>::value_type>;
    sentinel_impl(Range range)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
      , idx{}
    {
    }

    auto get() { return std::make_tuple(idx, *iter); }
    void next() { ++idx; ++iter; }
    bool is_end() { return iter == last ; }
  private:
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
  };
public:
  using value_type = std::tuple<size_t, typename std::decay_t<Range>::value_type>;
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  Indexed(Range range)
    : range{ range } {}

  iterator begin() const { return { std::make_unique<sentinel>(range) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
};

class IndexedProxy
  : private tag::adaptor_proxy_tag
{
public:
  IndexedProxy() = default;

  template < class Range >
  Indexed<Range> operator()(Range&& range) {
    return { std::forward<Range>(range) };
  }
};

namespace view {
  IndexedProxy indexed() { return {}; }
}

}}}
#endif
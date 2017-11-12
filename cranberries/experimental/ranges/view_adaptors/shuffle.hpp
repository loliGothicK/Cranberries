#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_SHUFFLE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_SHUFFLE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <algorithm>
#include "../../randomize.hpp"

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Shuffle
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range range)
      : range_{}
      , iter{}
      , last{}
    {
      using std::begin; using std::end;
      for (auto const& e : range) range_.emplace_back(e);
      std::shuffle(begin(range_), end(range_), gen_mt_64(std::random_device{}));
      iter = begin(range_);
      last = end(range_);
    }

    auto get() { return *iter; }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  private:
    std::vector<value_type> range_;
    typename std::vector<value_type>::iterator iter;
    typename std::vector<value_type>::iterator last;
  };
public:
  using value_type = typename std::decay_t<Range>::value_type;
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  template < class Range_ >
  Shuffle(Range_&& range)
    : range{ std::forward<Range_>(range) }
  {}

  iterator begin() const { return { std::make_unique<sentinel>(range) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
};

class ShuffleProxy
  : private tag::adaptor_proxy_tag
{
public:
  ShuffleProxy() {}

  template < class Range >
  Shuffle<Range> operator()(Range&& range) {
    return { std::forward<Range>(range) };
  }
};

namespace view {
  ShuffleProxy shuffle() { return {}; }
}

}}}
#endif
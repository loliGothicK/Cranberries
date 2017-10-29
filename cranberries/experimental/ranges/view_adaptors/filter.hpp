#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_FILTER_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_FILTER_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range, class Pred >
class Filter
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range range, Pred pred)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
      , pred(pred)
    {
      if (!pred(*iter)) next();
    }

    auto get() { return *iter; }
    void next() {
      ++iter;
      while (!pred(*iter) && iter != last) ++iter;
    }
    bool is_end() { return iter == last; }
  private:
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
    Pred pred;
  };
public:
  using value_type = typename std::decay_t<Range>::value_type;
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  template < class Range_, class Pred_ >
  Filter(Range_&& range, Pred_&& pred)
    : range{ std::forward<Range_>(range) }
    , pred{ std::forward<Pred_>(pred) }
  {}

  iterator begin() const { return { std::make_unique<sentinel>(range, pred) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
  Pred pred;
};

template < class Pred >
class FilterProxy
  : private tag::adaptor_proxy_tag
{
  Pred pred;
public:
  template < class Pred_ >
  FilterProxy(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

  template < class Range >
  Filter<Range, Pred> operator()(Range&& range) {
    return { std::forward<Range>(range), std::move(pred) };
  }
};

namespace view {
  template < class Pred >
  FilterProxy<Pred> filter(Pred&& pred) { return { std::forward<Pred>(pred) }; }
}

}}}
#endif
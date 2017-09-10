#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_FOR_EACH_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_FOR_EACH_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range, class F >
class ForEach
  : private tag::sentinel_range_tag
{
  class forward_sentinel {
    Range range_;
    F f_;
    typename std::decay_t<Range>::iterator iter;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    forward_sentinel(Range range, F f)
      : range_{ range }
      , f_(f)
      , iter{ cranberries::back_magic::begin(range_) } {}

    auto get() { return [&] { auto v = *iter; f_(v); return v; }(); }
    void next() { ++iter; }
    bool is_end() { return iter == cranberries::back_magic::end(range_); }
  };

  class reverse_sentinel {
    Range range_;
    F f_;
    typename std::decay_t<Range>::iterator iter;
  public:
    using value_type = std::result_of_t<F(typename std::decay_t<Range>::value_type)>;
    reverse_sentinel(Range range, F f)
      : range_{ range }
      , f_(f)
      , iter{ cranberries::back_magic::rbegin(range_) } {}

    auto get() { return [&] { auto v = *iter; f_(v); return v; }(); }
    void next() { ++iter; }
    bool is_end() { return iter == cranberries::back_magic::rend(range_); }
  };

public:
  using value_type = std::result_of_t<F(typename std::decay_t<Range>::value_type)>;
  using forward_sentinel = forward_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_sentinel = reverse_sentinel;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  ForEach(Range range, F f)
    : range{ range }
    , f(f) {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(range, f) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(range, f) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
  F f;
};

template < class F >
class ForEachProxy
  : private tag::adaptor_proxy_tag
{
  F f;
public:
  ForEachProxy(F f) : f{ f } {}
  template < class Range >
  ForEach<Range,F> adapt_to(Range&& range) {
    return { std::forward<Range>(range), std::forward<F>(f) };
  }
};

namespace view {
  template < typename F >
  ForEachProxy<F> for_each(F&& f) { return { std::forward<F>(f) }; }
}

}}}
#endif
#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_WHILE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_WHILE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range, class F >
class DropWhile
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range range, F f)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
    {
      do { ++iter; } while (iter != last && f(*iter));
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

  DropWhile(Range range, F f)
    : range{ range }
    , f(f) {}

  iterator begin() const { return { std::make_unique<sentinel>(range, f) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
  F f;
};

template < class F >
class DropWhileProxy
  : private tag::adaptor_proxy_tag
{
  F f;
public:
  DropWhileProxy(F f) : f{ f } {}
  template < class Range,
             enabler_t< is_callable_v<F(typename std::decay_t<Range>::value_type),bool>> = nullptr >
  DropWhile<Range,F> adapt_to(Range&& range) {
    return { std::forward<Range>(range), f };
  }
};

namespace view {
  template < class F >
  DropWhileProxy<F> drop_while(F&& f) { return { std::forward<F>(f) }; }
}

}}}
#endif
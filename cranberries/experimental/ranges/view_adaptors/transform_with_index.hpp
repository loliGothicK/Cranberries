#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_WITH_INDEX_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_WITH_INDEX_HPP
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
class TransformWithIndex
  : private tag::sentinel_range_tag
{
  class forward_sentinel {
    Range range_;
    F f_;
    size_t index;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
  public:
    using value_type = std::result_of_t<F(size_t, typename std::decay_t<Range>::value_type)>;
    forward_sentinel(Range range, F f, size_t init)
      : range_{ range }
      , f_(f)
      , index{ init }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) } {}

    auto get() { return f_(index++, *iter); }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  };

public:
  using value_type = std::result_of_t<F(size_t,typename std::decay_t<Range>::value_type)>;
  using forward_sentinel = forward_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  TransformWithIndex(Range range, F f, size_t init)
    : range{ range }
    , f(f)
    , init{ init } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(range, f, init) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
  F f;
  size_t init;
};

template < class F >
class TransformWithIndexProxy
  : private tag::adaptor_proxy_tag
{
  F f;
  size_t init;
public:
  TransformWithIndexProxy(F f, size_t init) : f{ f }, init{ init } {}
  template < class Range >
  TransformWithIndex<Range,F> operator()(Range&& range) {
    return { std::forward<Range>(range), std::forward<F>(f), init };
  }
};

namespace view {
  template < typename F >
  TransformWithIndexProxy<F> transform_with_index(F&& f, size_t init = 0) { return { std::forward<F>(f), init }; }
}

}}}
#endif
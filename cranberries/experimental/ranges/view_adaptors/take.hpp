#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Take
  : private tag::sentinel_range_tag
{
  Range range;
  long long int n;

  class sentinel_impl {
    Range range_;
    long long int n_;
    typename std::decay_t<Range>::iterator iter;
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range range, long long int n)
      : range_{ range }
      , n_(n)
      , iter{ cranberries::back_magic::begin(range_) } {}

    auto get() { return *iter; }
    bool next() { return ++iter, --n_ < 0; }
    bool is_end() { return n_ <= 0 || iter == cranberries::back_magic::begin(range_); }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;

  Take(Range range, size_t n)
    : range{ range }
    , n(n) {}

  iterator begin() { return { std::make_unique<sentinel>(range, n) }; }
  Sentinel end() { return {}; }
};

class TakeProxy
  : private tag::adaptor_proxy_tag
{
  size_t n;
public:
  TakeProxy(size_t n) : n{ n } {}
  template < class Range >
  Take<Range> make_adaptor(Range&& range) {
    return { std::forward<Range>(range), n };
  }
};

namespace lazy {
  TakeProxy take(size_t n) { return { n }; }
}

}}}
#endif
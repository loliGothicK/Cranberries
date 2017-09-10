#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_ZIP_WITH_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_ZIP_WITH_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class LeftRange, class RightRange >
class ZipWith
  : private tag::sentinel_range_tag
{
  class forward_sentinel {
    LeftRange left_;
    RightRange right_;
    typename std::decay_t<LeftRange>::iterator left_iter;
    typename std::decay_t<LeftRange>::iterator left_end;
    typename std::decay_t<RightRange>::iterator right_iter;
    typename std::decay_t<RightRange>::iterator right_end;
  public:
    using value_type = std::tuple<
      typename std::decay_t<LeftRange>::value_type,
      typename std::decay_t<RightRange>::value_type>;

    forward_sentinel(LeftRange left, RightRange right)
      : left_{ left }
      , right_{ right }
      , left_iter{ cranberries::back_magic::begin(left_) }
      , left_end{ cranberries::back_magic::end(left_) }
      , right_iter{ cranberries::back_magic::begin(right_) }
      , right_end{ cranberries::back_magic::end(right_) } {}

    auto get() { return std::make_tuple(*left_iter, *right_iter); }
    void next() { ++left_iter, ++right_iter; }
    bool is_end() { return left_iter == left_end || right_iter == right_end; }
  };

public:
  using value_type = std::tuple<
    typename std::decay_t<LeftRange>::value_type,
    typename std::decay_t<RightRange>::value_type>;
  using forward_sentinel = forward_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  ZipWith(LeftRange left, RightRange right)
    : left{ left }
    , right{ right } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(left, right) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  LeftRange left;
  RightRange right;
};

template < class RightRange >
class ZipWithProxy
  : private tag::adaptor_proxy_tag
{
  RightRange right_range;
public:
  ZipWithProxy(RightRange range) : right_range{ range } {}
  template < class LeftRange >
  ZipWith<LeftRange, RightRange> adapt_to(LeftRange&& range) {
    return { std::forward<LeftRange>(range), right_range };
  }
};

namespace view {
  template < class RightRange >
  ZipWithProxy<RightRange> zip_with(RightRange&& range) { return { range }; }
}

}}}
#endif
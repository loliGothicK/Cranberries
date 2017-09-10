#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_CONCAT_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_CONCAT_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class LeftRange, class RightRange >
class Concat
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
    using value_type = std::common_type_t<
      typename std::decay_t<LeftRange>::value_type,
      typename std::decay_t<RightRange>::value_type>;

    forward_sentinel(LeftRange left, RightRange right)
      : left_{ left }
      , right_{ right }
      , left_iter{ cranberries::back_magic::begin(left_) }
      , left_end{ cranberries::back_magic::end(left_) }
      , right_iter{ cranberries::back_magic::begin(right_) }
      , right_end{ cranberries::back_magic::end(right_) } {}

    value_type get() { return left_iter != left_end ? *left_iter : *right_iter; }
    void next() { if (left_iter != left_end) ++left_iter; else ++right_iter; }
    bool is_end() { return left_iter == left_end && right_iter == right_end; }
  };

  class reverse_sentinel {
    LeftRange left_;
    RightRange right_;
    typename std::decay_t<LeftRange>::reverse_iterator left_iter;
    typename std::decay_t<LeftRange>::reverse_iterator left_end;
    typename std::decay_t<RightRange>::reverse_iterator right_iter;
    typename std::decay_t<RightRange>::reverse_iterator right_end;
  public:
    using value_type = std::common_type_t<
      typename std::decay_t<LeftRange>::value_type,
      typename std::decay_t<RightRange>::value_type>;

    reverse_sentinel(LeftRange left, RightRange right)
      : left_{ left }
      , right_{ right }
      , left_iter{ cranberries::back_magic::rbegin(left_) }
      , left_end{ cranberries::back_magic::rend(left_) }
      , right_iter{ cranberries::back_magic::rbegin(right_) }
      , right_end{ cranberries::back_magic::rend(right_) } {}

    value_type get() { return right_iter != right_end ? *right_iter : *left_iter; }
    void next() { if (right_iter != right_end) ++right_iter; else ++left_iter; }
    bool is_end() { return left_iter == left_end && right_iter == right_end; }
  };
public:
  using forward_sentinel = forward_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_sentinel = reverse_sentinel;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;
  using value_type = std::common_type_t<
    typename std::decay_t<LeftRange>::value_type,
    typename std::decay_t<RightRange>::value_type>;

  Concat(LeftRange left, RightRange right)
    : left{ left }
    , right{ right } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(left, right) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(left, right) }; }
  reverse_iterator rend() const { return {}; }

private:
  LeftRange left;
  RightRange right;
};

template < class RightRange >
class ConcatProxy
  : private tag::adaptor_proxy_tag
{
  RightRange right_range;
public:
  ConcatProxy(RightRange range) : right_range{ range } {}
  template < class LeftRange >
  Concat<LeftRange, RightRange> adapt_to(LeftRange&& range) {
    return { std::forward<LeftRange>(range), right_range };
  }
};

namespace view {
  template < class RightRange >
  ConcatProxy<RightRange> concat(RightRange&& range) { return { range }; }
}

}}}
#endif
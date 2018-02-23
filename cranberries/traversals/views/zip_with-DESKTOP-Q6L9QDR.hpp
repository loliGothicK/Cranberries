#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_ZIP_WITH_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_ZIP_WITH_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class LeftRange, class RightRange >
class ZipWith
  : private tag::sentinel_traversal_tag
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

namespace view {
  template < class RightRange >
  auto zip_with(RightRange&& traversal) { return make_proxy<ZipWith>::embedded( std::forward<RightRange>(traversal) ); }
}

template <class, class...>
class ZippedRange;

template < size_t... I, class... Ranges >
class ZippedRange<std::index_sequence<I...>, Ranges...> {

  class forward_sentinel {
    std::tuple<typename std::decay_t<Ranges>::iterator...> iters_;
    std::tuple<typename std::decay_t<Ranges>::iterator...> ends_;
    using swallow = std::initializer_list<int>;
  public:
    using value_type = std::tuple<typename std::decay_t<Ranges>::value_type...>;

    template < class... Rs >
    forward_sentinel(Rs&&... traversals)
      : iters_{ std::begin(traversals)... }
      , ends_{ std::end(traversals)... }
    {}

    bool is_end() {
      bool _ = false;
      (void)swallow {
        (void(_ = _ || std::get<I>(iters_) == std::get<I>(ends_)), 0)...
      };
      return _;
    }
    void next() {
      (void)swallow {
      (void(++std::get<I>(iters_)), 0)...
    };
    }
    auto get() { return std::forward_as_tuple(*std::get<I>(iters_)...); }
  };
public:
  using value_type = std::tuple<typename std::decay_t<Ranges>::value_type...>;
  using iterator = sentinel_iterator<forward_sentinel>;

  ZippedRange(Ranges... traversals)
    : traversals_{ std::forward_as_tuple(std::forward<Ranges>(traversals)...) }
  {}

  iterator begin() const {
    return { std::make_unique<forward_sentinel>(std::get<I>(traversals_)...) };
  }

  iterator end() const { return {}; }

private:
  std::tuple<Ranges...> traversals_;
};

namespace view {
  template < class... Ranges >
  ZippedRange<std::index_sequence_for<Ranges...>, Ranges...>
    make_zipped(Ranges&&... traversals)
  {
    return { std::forward<Ranges>(traversals)... };
  }
}

}}}
#endif
#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_SHUFFLE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_SHUFFLE_HPP
#include <algorithm>
#include <type_traits>
#include "../../randomize.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Traversable>
class Shuffle
    : private tag::sentinel_traversal_tag,
      private reversable_traversal,
      public _detail::size_impl_injection<Shuffle<Traversable>, Traversable> {
  friend struct view_get;
  template <class, class, class>
  friend struct _detail::size_impl_injection;

 public:
  using value_type = typename std::decay_t<Traversable>::value_type;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;

  template <class Trav_>
  Shuffle(Trav_ &&traversal) : traversal{std::forward<Trav_>(traversal)} {}
  Shuffle(Shuffle const &) = delete;
  Shuffle &operator=(Shuffle const &) = delete;
  Shuffle(Shuffle &&) = default;
  Shuffle &operator=(Shuffle &&) = default;

 private:
  class view_ final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view;
    std::vector<value_type> seq;
    typename std::vector<value_type>::const_iterator iter;
    std::mt19937_64 mt;

   public:
    view_(std::unique_ptr<polymorphic_view<value_type>> _view)
        : view{std::move(_view)},
          seq{},
          mt{cranberries::gen_mt_64(std::random_device{})} {
      for (; !view->is_end(); view->next()) seq.emplace_back(view->get());
      std::shuffle(seq.begin(), seq.end(), mt);
      iter = seq.cbegin();
    }
    value_type get() const override final { return *iter; }
    void next() override final { ++iter; }
    bool is_end() const override final { return iter == seq.cend(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(view_get::fwd(traversal));
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<view_>(view_get::fwd(traversal));
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return {this->rev()}; }
  reverse_iterator rend() const { return {}; }

 private:
  Traversable traversal;
};

namespace _proxy {
struct ShuffleX : tag::adaptor_tag {
  template <class Trav>
  auto operator()(Trav &&trav) const {
    return Shuffle<Trav>{std::forward<Trav>(trav)};
  }
};
}  // namespace _proxy

namespace view {
constexpr _proxy::ShuffleX shuffle{};
}

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries

#endif

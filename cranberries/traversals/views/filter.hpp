#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_FILTER_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_FILTER_HPP
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Traversal, class Pred, bool = true>
class Filter : private tag::sentinel_traversal_tag,
               private reversable_traversal {
  friend struct view_get;

 public:
  using value_type = typename std::decay_t<Traversal>::value_type;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;

  Filter(Traversal traversal, Pred pred) : traversal{traversal}, pred{pred} {}
  Filter(Filter const &) = delete;
  Filter &operator=(Filter const &) = delete;
  Filter(Filter &&) = default;
  Filter &operator=(Filter &&) = default;

 private:
  class view_ final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view;
    Pred pred;

   public:
    view_(std::unique_ptr<polymorphic_view<value_type>> _view, Pred _pred)
        : view{std::move(_view)}, pred{_pred} {
      while (!pred(view->get())) view->next();
    }
    value_type get() const override final { return view->get(); }
    void next() override final {
      view->next();
      while (!pred(view->get())) view->next();
    }
    bool is_end() const override final { return view->is_end(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(view_get::fwd(traversal), pred);
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<default_rev_view<value_type>>(this->fwd());
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return {this->rev()}; }
  reverse_iterator rend() const { return {}; }

 private:
  Traversal traversal;
  Pred pred;
};

template <class Traversal, class Pred>
class Filter<Traversal, Pred, false> : private tag::sentinel_traversal_tag,
                                       private forward_traversal {
  friend struct view_get;

 public:
  using value_type = typename std::decay_t<Traversal>::value_type;
  using iterator = sentinel_iterator<value_type>;

  Filter(Traversal traversal, Pred pred) : traversal{traversal}, pred{pred} {}
  Filter(Filter const &) = delete;
  Filter &operator=(Filter const &) = delete;
  Filter(Filter &&) = default;
  Filter &operator=(Filter &&) = default;

 private:
  class view_ final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view;
    Pred pred;

   public:
    view_(std::unique_ptr<polymorphic_view<value_type>> _view, Pred _pred)
        : view{std::move(_view)}, pred{_pred} {
      while (!pred(view->get())) view->next();
    }
    value_type get() const override final { return view->get(); }
    void next() override final {
      view->next();
      while (!pred(view->get())) view->next();
    }
    bool is_end() const override final { return view->is_end(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(view_get::fwd(traversal), pred);
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }

 private:
  Traversal traversal;
  Pred pred;
};

namespace _proxy {
template <class Pred>
struct FilterX : tag::adaptor_tag {
  Pred pred;

 public:
  template <class Pred_>
  FilterX(Pred_ &&pred) : pred{std::forward<Pred_>(pred)} {}

  template <class Trav>
  auto operator()(Trav &&trav) {
    return Filter<Trav, Pred,
                  std::is_base_of<reversable_traversal, Trav>::value>{
        std::forward<Trav>(trav), pred};
  }
};
}  // namespace _proxy
namespace view {
template <class Pred>
inline auto filter(Pred pred) noexcept {
  return _proxy::FilterX<Pred>{std::forward<Pred>(pred)};
}
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
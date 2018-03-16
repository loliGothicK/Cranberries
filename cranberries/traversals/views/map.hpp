#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_HPP
#include <memory>
#include <type_traits>
#include "../../common/concepts.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Traversal, class F, bool = true>
class Map
    : private tag::sentinel_traversal_tag,
      public reversable_traversal,
      public _detail::size_impl_injection<Map<Traversal, F, true>, Traversal> {
  friend struct view_get;
  template <class, class, class>
  friend struct _detail::size_impl_injection;

 public:
  using value_type = cranberries::invoke_result_t<
      F, typename remove_cvr_t<Traversal>::value_type>;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using finite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  Map(Traversal traversal, F f)
      : traversal{std::forward<Traversal>(traversal)}, f(std::forward<F>(f)) {}
  Map(Map const &) = delete;
  Map &operator=(Map const &) = delete;
  Map(Map &&) = default;
  Map &operator=(Map &&) = default;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view_;
    F f;

   public:
    _view(std::unique_ptr<polymorphic_view<value_type>> p, F f)
        : view_{std::move(p)}, f{std::forward<F>(f)} {}
    _view(const _view &) = default;

   protected:
    value_type get() const override final { return f(view_->get()); }
    void next() override final { view_->next(); }
    bool is_end() const override final { return view_->is_end(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<_view>(view_get::fwd(traversal), f);
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<_view>(view_get::rev(traversal), f);
  }

 public:
  auto begin() const { return iterator{this->fwd()}; }
  auto end() const { return iterator{}; }
  auto rbegin() const { return reverse_iterator{this->rev()}; }
  auto rend() const { return reverse_iterator{}; }

 private:
  Traversal traversal;
  F f;
};

template <class Traversal, class F>
class Map<Traversal, F, false> : private tag::sentinel_traversal_tag,
                                 public forward_traversal {
  friend struct view_get;

 public:
  using value_type = cranberries::invoke_result_t<
      F, typename remove_cvr_t<Traversal>::value_type>;
  using iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  Map(Traversal traversal, F f)
      : traversal{std::forward<Traversal>(traversal)}, f(std::forward<F>(f)) {}
  Map(Map const &) = delete;
  Map &operator=(Map const &) = delete;
  Map(Map &&) = default;
  Map &operator=(Map &&) = default;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view_;
    const F &f;

   public:
    _view(std::unique_ptr<polymorphic_view<value_type>> p, const F &f)
        : view_{std::move(p)}, f{f} {}
    _view(const _view &) = default;

   protected:
    value_type get() const override final { return f(view_->get()); }
    void next() override final { view_->next(); }
    bool is_end() const override final { return view_->is_end(); }
  };

 protected:
  virtual std::unique_ptr<polymorphic_view<value_type>> fwd()
      const override final {
    return std::make_unique<_view>(traversal->fwd(), f);
  }

 public:
  auto begin() const { return iterator{this->fwd()}; }
  auto end() const { return iterator{}; }

 private:
  Traversal traversal;
  F f;
};

namespace _proxy {
template <class F>
class MapX : tag::adaptor_tag {
  F f;

 public:
  template <class F_>
  explicit MapX(F_ &&f) : f{std::forward<F_>(f)} {}

  template <class Traversable>
  auto operator()(Traversable &&trav) {
    return Map<Traversable, F,
               std::is_base_of<reversable_traversal,
                               remove_cvr_t<Traversable>>::value>{
        std::forward<Traversable>(trav), std::forward<F>(f)};
  }
};
}  // namespace _proxy

namespace view {
template <typename F>
auto map(F &&f) {
  return _proxy::MapX<F>{std::forward<F>(f)};
}
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
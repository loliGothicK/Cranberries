#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_HPP
#include <memory>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class T, class F, bool = true>
class Map : private tag::sentinel_traversal_tag,
            public reversable_traversal<std::result_of_t<F(T)>> {
 public:
  using value_type = std::result_of_t<F(T)>;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<T>> view_;
    const F &f;

   public:
    _view(std::unique_ptr<polymorphic_view<T>> p, const F &f)
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
  virtual std::unique_ptr<polymorphic_view<value_type>> rev()
      const override final {
    return std::make_unique<_view>(traversal->rev(), f);
  }

 public:
  Map(std::shared_ptr<reversable_traversal<T>> traversal, F f)
      : traversal{std::move(traversal)}, f(std::forward<F>(f)) {}

  auto begin() const { return iterator{this->fwd()}; }
  auto end() const { return iterator{}; }
  auto rbegin() const { return reverse_iterator{this->rev()}; }
  auto rend() const { return reverse_iterator{}; }

 private:
  std::shared_ptr<reversable_traversal<T>> traversal;
  F f;
};

template <class T, class F>
class Map<T, F, false> : private tag::sentinel_traversal_tag,
                         public forward_traversal<std::result_of_t<F(T)>> {
 public:
  using value_type = std::result_of_t<F(T)>;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<T>> view_;
    const F &f;

   public:
    _view(std::unique_ptr<polymorphic_view<T>> p, const F &f)
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
  Map(std::shared_ptr<forward_traversal<T>> traversal, F f)
      : traversal{std::move(traversal)}, f(std::forward<F>(f)) {}

  auto begin() const { return iterator{this->fwd()}; }
  auto end() const { return iterator{}; }

 private:
  std::shared_ptr<forward_traversal<T>> traversal;
  F f;
};

namespace _proxy {
template <class F>
class MapX : tag::adaptor_tag {
  F f;

 public:
  template <class F_>
  explicit MapX(F_ &&f) : f{std::forward<F_>(f)} {}

  template <class Traversable,
            typename T = typename remove_cvr_t<Traversable>::value_type>
  auto operator()(Traversable &&trav) {
    return Map<T, F,
               std::is_base_of<reversable_traversal<T>,
                               remove_cvr_t<Traversable>>::value>{
        std::make_shared<remove_cvr_t<Traversable>>(
            std::forward<Traversable>(trav)),
        std::forward<F>(f)};
  }
};
}  // namespace _proxy

namespace view {
template <typename F>
auto map(F &&f) {
  return _proxy::MapX<F &&>{std::forward<F>(f)};
}
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
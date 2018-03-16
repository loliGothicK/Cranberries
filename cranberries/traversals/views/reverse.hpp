#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_REVERSE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_REVERSE_HPP
#include <memory>
#include "../../common/concepts.hpp"
#include "../../detection_toolkit.hpp"
#include "../../utility/utility.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Traversal>
class Rev : private tag::sentinel_traversal_tag,
            private reversable_traversal,
            public _detail::size_impl_injection<Rev<Traversal>, Traversal> {
  friend struct view_get;
  template <class, class, class>
  friend struct _detail::size_impl_injection;

 public:
  using value_type = typename remove_cvr_t<Traversal>::value_type;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using finite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  Rev(Traversal traversal) : traversal{traversal} {}

  Rev(Rev const &) = delete;
  Rev &operator=(Rev const &) = delete;
  Rev(Rev &&) = default;
  Rev &operator=(Rev &&) = default;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> resouce;

   public:
    ~_view() = default;
    _view(std::unique_ptr<polymorphic_view<value_type>> p)
        : resouce{std::move(p)} {}
    _view(const _view &) = default;

   protected:
    virtual value_type get() const override final { return resouce->get(); }
    virtual void next() override final { resouce->next(); }
    virtual bool is_end() const override final { return resouce->is_end(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<_view>(view_get::rev(traversal));
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<_view>(view_get::fwd(traversal));
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return {this->rev()}; }
  reverse_iterator rend() const { return {}; }

 private:
  Traversal traversal;
};

namespace _proxy {
struct RevX : private tag::adaptor_tag {
  template <class Trav>
  using requires = always_t<decltype(view_get::rev(std::declval<Trav &>()))>;

  template <class Traversal>
  auto operator()(Traversal &&trav) const {
    return Rev<Traversal>{std::forward<Traversal>(trav)};
  }
};
}  // namespace _proxy

namespace view {
static constexpr _proxy::RevX rev{};
}

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
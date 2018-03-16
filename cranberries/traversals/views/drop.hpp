#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DROP_HPP
#include <memory>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class, bool = true>
class Drop;

namespace _detail {
template <class, class = void>
struct size_impl_injection_for_drop {};

template <class Traversal>
struct size_impl_injection_for_drop<
    Drop<Traversal, true>,
    ::cranberries::void_t<decltype(1 !=
                                   std::declval<const Traversal &>().size())>> {
  constexpr size_t size() const {
    auto has =
        static_cast<const Drop<Traversal, true> *>(this)->traversal.size();
    auto drop = static_cast<const Drop<Traversal, true> *>(this)->n;
    return has < drop ? size_t{} : has - drop;
  }
};
}  // namespace _detail

template <class Traversal>
class Drop<Traversal, true>
    : private tag::sentinel_traversal_tag,
      private reversable_traversal,
      public _detail::size_impl_injection_for_drop<Drop<Traversal, true>> {
  friend struct view_get;
  template <class, class>
  friend struct _detail::size_impl_injection_for_drop;

 public:
  using value_type = typename std::decay_t<Traversal>::value_type;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;

  Drop(Traversal traversal, size_t n) : traversal{traversal}, n(n) {}
  Drop(Drop const &) = delete;
  Drop &operator=(Drop const &) = delete;
  Drop(Drop &&) = default;
  Drop &operator=(Drop &&) = default;

 private:
  class view_ final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view;

   public:
    view_(std::unique_ptr<polymorphic_view<value_type>> _view, size_t n)
        : view{std::move(_view)} {
      for (size_t i{}; i < n || view->is_end(); ++i, view->next())
        ;
    }
    value_type get() const override final { return view->get(); }
    void next() override final { view->next(); }
    bool is_end() const override final { return view->is_end(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(view_get::fwd(traversal), n);
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
  size_t n;
};

template <class Traversal>
class Drop<Traversal, false> : private tag::sentinel_traversal_tag,
                               private forward_traversal {
  friend struct view_get;

 public:
  using value_type = typename std::decay_t<Traversal>::value_type;
  using iterator = sentinel_iterator<value_type>;

  Drop(Traversal traversal, size_t n) : traversal{traversal}, n(n) {}
  Drop(Drop const &) = delete;
  Drop &operator=(Drop const &) = delete;
  Drop(Drop &&) = default;
  Drop &operator=(Drop &&) = default;

 private:
  class view_ final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<value_type>> view;

   public:
    view_(std::unique_ptr<polymorphic_view<value_type>> _view, size_t n)
        : view{std::move(_view)} {
      for (size_t i{}; i < n || view->is_end(); ++i, view->next())
        ;
    }
    value_type get() const override final { return view->get(); }
    void next() override final { view->next(); }
    bool is_end() const override final { return view->is_end(); }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(view_get::fwd(traversal), n);
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }

 private:
  Traversal traversal;
  size_t n;
};

namespace _proxy {
struct DropX : tag::adaptor_tag {
  size_t n;

 public:
  DropX(size_t n) : n{n} {}

  template <class Trav>
  auto operator()(Trav &&trav) {
    return Drop<Trav, std::is_base_of<reversable_traversal, Trav>::value>{
        std::forward<Trav>(trav), n};
  }
};
}  // namespace _proxy
namespace view {
inline auto drop(size_t n) noexcept { return _proxy::DropX{n}; }
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
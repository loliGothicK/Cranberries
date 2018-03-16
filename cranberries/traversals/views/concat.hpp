#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_CONCAT_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_CONCAT_HPP
#include <memory>
#include <type_traits>
#include "../../utility/utility.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class, class, bool = true>
class Concat;

namespace _detail {
template <class, class = void>
struct size_impl_injection_for_concat {};

template <class Trav1, class Trav2>
struct size_impl_injection_for_concat<
    Concat<Trav1, Trav2, true>,
    cranberries::void_t<decltype(std::declval<const Trav1 &>().size(),
                                 std::declval<const Trav2 &>().size())>> {
  constexpr size_t size() const {
    return static_cast<const Concat<Trav1, Trav2, true> *>(this)->left.size() +
           static_cast<const Concat<Trav1, Trav2, true> *>(this)->right.size();
  }
};
}  // namespace _detail

template <class Trav1, class Trav2>
class Concat<Trav1, Trav2, true>
    : private tag::sentinel_traversal_tag,
      public reversable_traversal,
      public _detail::size_impl_injection_for_concat<
          Concat<Trav1, Trav2, true>> {
  friend struct view_get;
  template <class, class>
  friend struct _detail::size_impl_injection_for_concat;

 public:
  using value_type =
      std::common_type_t<typename remove_cvr_t<Trav1>::value_type,
                         typename remove_cvr_t<Trav1>::value_type>;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using finite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  template <class Trav1_, class Trav2_>
  Concat(Trav1_ &&left, Trav2_ &&right)
      : left{std::forward<Trav1_>(left)}, right{std::forward<Trav2_>(right)} {}
  Concat(Concat const &) = delete;
  Concat &operator=(Concat const &) = delete;
  Concat(Concat &&) = default;
  Concat &operator=(Concat &&) = default;

 private:
  class fwd_view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<typename remove_cvr_t<Trav1>::value_type>>
        view_l;
    std::unique_ptr<polymorphic_view<typename remove_cvr_t<Trav2>::value_type>>
        view_r;

   public:
    fwd_view(std::unique_ptr<
                 polymorphic_view<typename remove_cvr_t<Trav1>::value_type>>
                 l,
             std::unique_ptr<
                 polymorphic_view<typename remove_cvr_t<Trav2>::value_type>>
                 r)
        : view_l{std::move(l)}, view_r{std::move(r)} {}
    fwd_view(const fwd_view &) = default;

   protected:
    value_type get() const override final {
      return view_l->is_end() ? view_r->get() : view_l->get();
    }
    void next() override final {
      if (view_l->is_end())
        view_r->next();
      else
        view_l->next();
    }
    bool is_end() const override final {
      return view_l->is_end() && view_r->is_end();
    }
  };
  class rev_view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<typename remove_cvr_t<Trav2>::value_type>>
        view_l;
    std::unique_ptr<polymorphic_view<typename remove_cvr_t<Trav1>::value_type>>
        view_r;

   public:
    rev_view(std::unique_ptr<
                 polymorphic_view<typename remove_cvr_t<Trav2>::value_type>>
                 l,
             std::unique_ptr<
                 polymorphic_view<typename remove_cvr_t<Trav1>::value_type>>
                 r)
        : view_l{std::move(l)}, view_r{std::move(r)} {}
    rev_view(const rev_view &) = default;

   protected:
    value_type get() const override final {
      return view_l->is_end() ? view_r->get() : view_l->get();
    }
    void next() override final {
      if (view_l->is_end())
        view_r->next();
      else
        view_l->next();
    }
    bool is_end() const override final {
      return view_l->is_end() && view_r->is_end();
    }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<fwd_view>(view_get::fwd(left),
                                      view_get::fwd(right));
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<rev_view>(view_get::rev(right),
                                      view_get::rev(left));
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  iterator rbegin() const { return {this->rev()}; }
  iterator rend() const { return {}; }

 private:
  Trav1 left;
  Trav2 right;
};

template <class Trav1, class Trav2>
class Concat<Trav1, Trav2, false> : private tag::sentinel_traversal_tag,
                                    public forward_traversal {
  friend struct view_get;

 public:
  using value_type =
      std::common_type_t<typename remove_cvr_t<Trav1>::value_type,
                         typename remove_cvr_t<Trav1>::value_type>;
  using iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  template <class Trav1_, class Trav2_>
  Concat(Trav1_ &&left, Trav2_ &&right)
      : left{std::forward<Trav1_>(left)}, right{std::forward<Trav2_>(right)} {}
  Concat(Concat const &) = delete;
  Concat &operator=(Concat const &) = delete;
  Concat(Concat &&) = default;
  Concat &operator=(Concat &&) = default;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::unique_ptr<polymorphic_view<typename remove_cvr_t<Trav1>::value_type>>
        view_l;
    std::unique_ptr<polymorphic_view<typename remove_cvr_t<Trav2>::value_type>>
        view_r;

   public:
    _view(std::unique_ptr<
              polymorphic_view<typename remove_cvr_t<Trav1>::value_type>>
              l,
          std::unique_ptr<
              polymorphic_view<typename remove_cvr_t<Trav2>::value_type>>
              r)
        : view_l{std::move(l)}, view_r{std::move(r)} {}
    _view(const _view &) = default;

   protected:
    value_type get() const override final {
      return view_l->is_end() ? view_r->get() : view_l->get();
    }
    void next() override final {
      if (view_l->is_end())
        view_r->next();
      else
        view_l->next();
    }
    bool is_end() const override final {
      return view_l->is_end() && view_r->is_end();
    }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<_view>(view_get::fwd(left), view_get::fwd(right));
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }

 private:
  Trav1 left;
  Trav2 right;
};

namespace _proxy {
template <class Traversal, bool = true>
class ConcatX : private tag::adaptor_tag {
  Traversal trav;

 public:
  template <class Traversal_>
  ConcatX(Traversal_ &&trav) : trav{std::forward<Traversal_>(trav)} {}

  template <class Traversal_>
  Concat<Traversal_, Traversal> operator()(Traversal_ &&traversal) {
    return {std::forward<Traversal_>(traversal), std::forward<Traversal>(trav)};
  }
};

template <class Traversal>
class ConcatX<Traversal, false> : private tag::adaptor_tag {
  Traversal trav;

 public:
  template <class Traversal_>
  ConcatX(Traversal_ &&trav) : trav{std::forward<Traversal_>(trav)} {}

  template <class Traversal_>
  Concat<Traversal_, Traversal, false> operator()(Traversal_ &&traversal) {
    return {std::forward<Traversal_>(traversal), std::forward<Traversal>(trav)};
  }
};

}  // namespace _proxy
namespace view {
template <class Right>
auto concat(Right &&trav) {
  return _proxy::ConcatX<
      Right, std::is_base_of<reversable_traversal, remove_cvr_t<Right>>::value>{
      std::forward<Right>(trav)};
}
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif
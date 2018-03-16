#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP
#include "../../utility/utility.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Traversal>
class Cyclic : private tag::sentinel_traversal_tag, forward_traversal {
  friend struct view_get;

 public:
  using value_type = typename remove_cvr_t<Traversal>::value_type;
  using iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  Cyclic(Traversal traversal) : traversal{std::forward<Traversal>(traversal)} {}
  Cyclic(Cyclic const &) = delete;
  Cyclic &operator=(Cyclic const &) = delete;
  Cyclic(Cyclic &&) = default;
  Cyclic &operator=(Cyclic &&) = default;

 private:
  class _view final : public polymorphic_view<value_type> {
    std::vector<value_type> range;
    size_t index{};
    size_t length;

   public:
    _view(std::unique_ptr<polymorphic_view<value_type>> view) : range{} {
      for (; !view->is_end(); view->next()) range.emplace_back(view->get());
      length = range.size();
    }
    _view(const _view &) = default;
    ~_view() = default;

   protected:
    value_type get() const override final { return range[index]; }
    void next() override final {
      if (!(++index < length)) index = 0;
    }
    bool is_end() const override final { return false; }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<_view>(view_get::fwd(traversal));
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }

 private:
  Traversal traversal;
};

namespace _proxy {
struct CyclicX : tag::adaptor_tag {
  template <class Traversal>
  auto operator()(Traversal &&trav) const {
    return Cyclic<Traversal>{std::forward<Traversal>(trav)};
  }
};
}  // namespace _proxy

namespace view {
inline auto cyclic() noexcept { return _proxy::CyclicX{}; }
}  // namespace view

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

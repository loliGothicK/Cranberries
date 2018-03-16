#ifndef CRANBERRIES_TRAVERSAL_CREATE_MODULE_ITERATE_HPP
#define CRANBERRIES_TRAVERSAL_CREATE_MODULE_ITERATE_HPP
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class ValueType, class Advance>
class Iterate : private tag::sentinel_traversal_tag, public forward_traversal {
  friend struct view_get;

 public:
  using value_type = ValueType;
  using iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  Iterate(ValueType _1, Advance _2) : value{_1}, advance{_2} {}

 private:
  class view_ final : public polymorphic_view<value_type> {
    ValueType value;
    Advance advance;

   public:
    view_(const ValueType &value, const Advance &advance)
        : value{value}, advance{advance} {}
    view_(const view_ &) = default;

    virtual value_type get() const override final { return value; }
    virtual void next() override final { value = advance(value); }
    virtual bool is_end() const override final { return false; }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>(value, advance);
  }

 public:
  iterator begin() const { return {std::make_unique<view_>(value, advance)}; }
  iterator end() const { return {}; }

 private:
  ValueType value;
  Advance advance;
};

namespace create {
template <typename ValueType, class F>
Iterate<ValueType, std::decay_t<F>> iterate(ValueType init, F &&f) {
  return {init, std::forward<F>(f)};
}
}  // namespace create

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_TRAVERSAL_CREATE_MODULE_ITERATE_HPP

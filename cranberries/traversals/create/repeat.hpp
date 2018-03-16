#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_REPEAT_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_REPEAT_HPP
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class ValueType>
class Repeat : private tag::sentinel_traversal_tag,
               public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = ValueType;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  Repeat(ValueType _1) : value{_1} {}
  Repeat(const Repeat &) = default;

 private:
  class _view final : public polymorphic_view<ValueType> {
    ValueType value_;

   public:
    _view(ValueType init) : value_{init} {}
    _view(const _view &) = default;

   protected:
    ValueType get() const override final { return value_; }
    void next() override final {}
    bool is_end() const override final { return false; }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<_view>(value);
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<_view>(value);
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return {this->rev()}; }
  reverse_iterator rend() const { return {}; }

 private:
  value_type value;
};

namespace create {
template <typename ValueType>
Repeat<ValueType> repeat(ValueType value) {
  return {value};
}
}  // namespace create

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_RANGES_SENTINEL_RANGES_REPEAT_HPP

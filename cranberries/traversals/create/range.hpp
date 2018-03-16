#ifndef CRANBERRIES_TRAVERSAL_CREATE_MODULE_RANGED_HPP
#define CRANBERRIES_TRAVERSAL_CREATE_MODULE_RANGED_HPP
#include "../../common/concepts.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class ValueType,
          bool = concepts::required_v<ValueType, concepts::decrementable>>
class Ranged;

template <class ValueType>
class Ranged<ValueType, true> final : private tag::sentinel_traversal_tag,
                                      public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = ValueType;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using finite_ = ::std::nullptr_t;
  using immutable_ = ::std::nullptr_t;

  Ranged(value_type _1, value_type _2) : value{_1}, upper_bound{_2} {}
  Ranged(const Ranged &) = default;

 private:
  class fwd_view final : public polymorphic_view<value_type> {
    ValueType value_;
    ValueType upper_bound;

   public:
    fwd_view(ValueType init, ValueType upper)
        : value_{init}, upper_bound{upper} {}
    fwd_view(const fwd_view &) = default;

   protected:
    value_type get() const override final { return value_; }
    void next() override final { ++value_; }
    bool is_end() const override final { return upper_bound <= value_; }
  };

  class rev_view final : public polymorphic_view<value_type> {
    value_type value_;
    value_type lower_bound;

   public:
    rev_view(value_type init, value_type upper)
        : value_{--upper}, lower_bound{--init} {}
    rev_view(const rev_view &) = default;

   protected:
    value_type get() const override final { return value_; }
    void next() override final { --value_; }
    bool is_end() const override final { return value_ <= lower_bound; }
  };

  ::std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>>
  fwd() const {
    return std::make_unique<fwd_view>(value, upper_bound);
  }
  ::std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>>
  rev() const {
    return std::make_unique<rev_view>(value, upper_bound);
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  iterator rbegin() const { return {this->rev()}; }
  iterator rend() const { return {}; }
  size_t size() const noexcept { return upper_bound - value; }

 private:
  value_type value;
  value_type upper_bound;
};

template <class ValueType>
class Ranged<ValueType, false> final : private tag::sentinel_traversal_tag,
                                       public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = ValueType;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using finite_ = ::std::nullptr_t;
  using immutable_ = ::std::nullptr_t;

  Ranged(value_type _1, value_type _2) noexcept : value{_1}, upper_bound{_2} {}
  Ranged(const Ranged &) = default;

 private:
  class fwd_view final : public polymorphic_view<value_type> {
    value_type value_;
    value_type lower_bound;

   public:
    fwd_view(value_type init, value_type upper)
        : value_{init}, lower_bound{upper} {}
    fwd_view(const fwd_view &) = default;

   protected:
    value_type get() const override final { return value_; }
    void next() override final { ++value_; }
    bool is_end() const override final { return upper_bound <= value_; }
  };

  ::std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>>
  fwd() const {
    return std::make_unique<fwd_view>(value, upper_bound);
  }
  ::std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>>
  rev() const {
    return std::make_unique<default_rev_view>(value, upper_bound);
  }

 public:
  iterator begin() const { return {this->fwd()}; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const noexcept { return {this->rev()}; }
  reverse_iterator rend() const noexcept { return {}; }
  size_t size() const noexcept { return upper_bound - value; }

 private:
  value_type value;
  value_type upper_bound;
};

namespace create {

template <typename ValueType,
          concepts::concept_required<ValueType, concepts::incrementable,
                                     concepts::regular_type> = nullptr>
Ranged<ValueType> semi_irange(ValueType init, ValueType upper_bound) {
  return {init, upper_bound};
}

template <typename ValueType,
          concepts::concept_required<ValueType, concepts::incrementable,
                                     concepts::regular_type> = nullptr>
Ranged<ValueType> irange(ValueType init, ValueType upper_bound) {
  return {init, upper_bound + 1};
}
}  // namespace create

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_TRAVERSAL_CREATE_MODULE_RANGED_HPP

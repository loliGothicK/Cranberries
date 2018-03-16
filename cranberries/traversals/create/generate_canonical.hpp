#ifndef CRANBERRIES_TRAVERSAL_CREATE_MODULE_GENERATE_CANONICAL_HPP
#define CRANBERRIES_TRAVERSAL_CREATE_MODULE_GENERATE_CANONICAL_HPP
#include <limits>
#include <random>
#include "../../randomize.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class RealType>
class GenerateCanonical : private tag::sentinel_traversal_tag,
                          public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = RealType;
  using iterator = sentinel_iterator<RealType>;
  using reverse_iterator = sentinel_iterator<RealType>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

 private:
  class view_ final : public polymorphic_view<value_type> {
    mutable std::mt19937_64 engine;

   public:
    view_() : engine(gen_mt_64(std::random_device{})) {}
    view_(const view_ &) = default;

    value_type get() const override final {
      return std::generate_canonical<RealType,
                                     std::numeric_limits<RealType>::digits>(
          engine);
    }
    void next() override final {}
    bool is_end() const override final { return false; }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>();
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<view_>();
  }

 public:
  iterator begin() { return {this->fwd()}; }
  iterator end() { return {}; }
  reverse_iterator rbegin() { return {this->rev()}; }
  reverse_iterator rend() { return {}; }
};

template <class Distribution, class Tuple>
class GenerateDistribution : private tag::sentinel_traversal_tag,
                             public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = typename Distribution::result_type;
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  template <class Tuple_>
  GenerateDistribution(Tuple_ &&tup) : params{std::forward<Tuple_>(tup)} {}

 private:
  class view_ final : public polymorphic_view<value_type> {
    mutable randomize<Distribution> dist;

   public:
    template <class... Args>
    view_(Args &&... args)
        : dist{std::random_device{}, std::forward<Args>(args)...} {}
    view_(const view_ &) = default;

    value_type get() const override final { return dist(); }
    void next() override final {}
    bool is_end() const override final { return false; }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return ::cranberries::apply(
        [](auto &&... args) {
          return std::make_unique<view_>(std::forward<decltype(args)>(args)...);
        },
        params);
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return ::cranberries::apply(
        [](auto &&... args) {
          return std::make_unique<view_>(std::forward<decltype(args)>(args)...);
        },
        params);
  }

 public:
  iterator begin() { return {this->fwd()}; }
  iterator end() { return {}; }
  reverse_iterator rbegin() { return {this->rev()}; }
  reverse_iterator rend() { return {}; }

 private:
  Tuple params;
};

namespace create {
template <typename RealType = double,
          std::enable_if_t<std::is_floating_point<RealType>::value,
                           std::nullptr_t> = nullptr>
GenerateCanonical<RealType> generate_canonical() {
  return {};
}

template <class Dist, class... Args>
auto generate_distribution(Args &&... args) {
  return GenerateDistribution<Dist, decltype(std::forward_as_tuple(
                                        std::forward<Args>(args)...))>{
      std::forward_as_tuple(std::forward<Args>(args)...)};
}

template <class IntType, enabler_t<std::is_integral<IntType>::value> = nullptr>
auto uniform_distribution(IntType a, IntType b) {
  return GenerateDistribution<std::uniform_int_distribution<IntType>,
                              std::tuple<IntType, IntType>>{
      std::make_tuple(a, b)};
}

template <class RealType,
          enabler_t<std::is_floating_point<RealType>::value> = nullptr>
auto uniform_distribution(RealType a, RealType b) {
  return GenerateDistribution<std::uniform_real_distribution<RealType>,
                              std::tuple<RealType, RealType>>{
      std::make_tuple(a, b)};
}

}  // namespace create

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_TRAVERSAL_CREATE_MODULE_GENERATE_CANONICAL_HPP

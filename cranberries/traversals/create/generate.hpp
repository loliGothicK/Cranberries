#ifndef CRANBERRIES_TRAVERSAL_CREATE_MODULE_GENERATE_HPP
#define CRANBERRIES_TRAVERSAL_CREATE_MODULE_GENERATE_HPP
#include "../../randomize.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template <class Generator>
class Generate final : private tag::sentinel_traversal_tag,
                       public reversable_traversal {
  friend struct view_get;

 public:
  using value_type = decltype(std::declval<Generator>()());
  using iterator = sentinel_iterator<value_type>;
  using reverse_iterator = sentinel_iterator<value_type>;
  using infinite_ = std::nullptr_t;
  using immutable_ = std::nullptr_t;

  template <class Generator_>
  Generate(Generator_ &&gen) noexcept
      : generator{std::forward<Generator_>(gen)} {}

 private:
  class view_ final : public polymorphic_view<value_type> {
    mutable Generator generator_;

   public:
    view_(const Generator &generator) : generator_{generator} {}
    view_(const view_ &) = default;

    virtual value_type get() const override final { return generator_(); }
    virtual void next() override final {}
    virtual bool is_end() const override final { return false; }
  };

  std::unique_ptr<polymorphic_view<value_type>> fwd() const {
    return std::make_unique<view_>((generator));
  }
  std::unique_ptr<polymorphic_view<value_type>> rev() const {
    return std::make_unique<view_>((generator));
  }

 public:
  iterator begin() noexcept { return {this->fwd()}; }
  iterator end() noexcept { return {}; }
  iterator rbegin() noexcept { return {this->rev()}; }
  iterator rend() noexcept { return {}; }

 private:
  Generator generator;
};

namespace create {
template <class Generator>
Generate<Generator> generate(Generator &&generator) noexcept {
  return {std::forward<Generator>(generator)};
}

inline auto random() noexcept {
  return generate([mt = ::cranberries::gen_mt_64(
                       std::random_device())]() mutable { return mt(); });
}
}  // namespace create

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_TRAVERSAL_CREATE_MODULE_GENERATE_HPP

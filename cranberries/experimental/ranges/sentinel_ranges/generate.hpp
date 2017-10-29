#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../randomize.hpp"
#include <random>
#include <limits>

namespace cranberries {
namespace experimental {
namespace ranges {


template < class Generator >
class Generate
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    Generator generator_;
  public:
    using value_type = decltype(std::declval<Generator>()());
    sentinel_impl(Generator gen) : generator_{ std::forward<Generator>(gen) } {}
    sentinel_impl(const sentinel_impl&) = default;
    auto get() { return generator_(); }
    void next() {}
    std::false_type is_end() { return {}; }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using value_type = decltype(std::declval<Generator>()());

  Generate(Generator gen) : generator{ std::forward<Generator>(gen) } {}

  iterator begin() { return { std::make_unique<sentinel>(generator) }; }
  iterator end() { return {}; }
private:
  Generator generator;
};

namespace create {
  template < class Generator >
  Generate<Generator>
    generate(Generator&& generator) { return {std::forward<Generator>(generator)}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP

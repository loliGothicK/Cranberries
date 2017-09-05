#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include <random>
#include <limits>

namespace cranberries {
namespace experimental {
namespace ranges {


template < class RealType >
class GenerateCanonical
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    std::mt19937 engine;
  public:
    using value_type = RealType;
    sentinel_impl() : engine{ std::random_device{}() } {}
    sentinel_impl(const sentinel_impl&) = default;
    auto get() { return std::generate_canonical<RealType, std::numeric_limits<RealType>::digits>(engine); }
    std::false_type next() { return {}; }
    std::false_type is_end() { return {}; }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;

  GenerateCanonical() = default;
  using value_type = RealType;

  iterator begin() { return { std::make_unique<sentinel>() }; }
  iterator end() { return {}; }
};

namespace create {
  template < typename RealType = double >
  GenerateCanonical<RealType> generate_canonical() { return {}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP

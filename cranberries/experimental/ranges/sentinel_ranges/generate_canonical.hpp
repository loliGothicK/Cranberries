#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../randomize.hpp"
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
    std::mt19937_64 engine;
  public:
    using value_type = RealType;
    sentinel_impl() : engine(gen_mt_64(std::random_device{})) {}
    sentinel_impl(const sentinel_impl&) = default;
    auto get() { return std::generate_canonical<RealType, std::numeric_limits<RealType>::digits>(engine); }
    std::false_type next() { return {}; }
    std::false_type is_end() { return {}; }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using reverse_iterator = sentinel_iterator<sentinel>;
  using value_type = RealType;

  GenerateCanonical() {}

  iterator begin() { return { std::make_unique<sentinel>() }; }
  iterator end() { return {}; }
  reverse_iterator rbegin() { return { std::make_unique<sentinel>() }; }
  reverse_iterator rend() { return {}; }
};

namespace create {
  template < typename RealType = double >
  GenerateCanonical<RealType> generate_canonical() { return {}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP

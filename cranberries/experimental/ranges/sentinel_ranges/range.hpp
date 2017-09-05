#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_RANGE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_RANGE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"


namespace cranberries {
namespace experimental {
namespace ranges {


template < class ValueType >
class Range
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    ValueType value;
    ValueType upper_bound;
  public:
    using value_type = ValueType;
    sentinel_impl(ValueType _1, ValueType _2) : value{ _1 }, upper_bound{ _2 } {}
    sentinel_impl(const sentinel_impl&) = default;
    auto get() { return value; }
    bool next() { return upper_bound <= ++value; }
    bool is_end() { return upper_bound <= value; }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using value_type = ValueType;

  Range(value_type _1, value_type _2) : init{ _1 }, bound{ _2 } {}

  iterator begin() { return { std::make_unique<sentinel>(init, bound) }; }
  iterator end() { return {}; }
private:
  value_type init;
  value_type bound;
};

namespace create {
  template < typename ValueType >
  Range<ValueType> range(ValueType init, ValueType upper_bound) { return {init, upper_bound}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP

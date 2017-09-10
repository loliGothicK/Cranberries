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
  class forward_sentinel {
    ValueType value;
    ValueType upper_bound;
  public:
    using value_type = ValueType;
    forward_sentinel(ValueType _1, ValueType _2) : value{ _1 }, upper_bound{ _2 } {}
    forward_sentinel(const forward_sentinel&) = default;
    auto get() { return value; }
    bool next() { return upper_bound <= ++value; }
    bool is_end() { return upper_bound <= value; }
  };
  class reverse_sentinel {
    ValueType value;
    ValueType lower_bound;
  public:
    using value_type = ValueType;
    reverse_sentinel(ValueType init, ValueType upper) : value{ --upper }, lower_bound{ --init } {}
    reverse_sentinel(const reverse_sentinel&) = default;
    auto get() { return value; }
    void next() { --value; }
    bool is_end() { return value <= lower_bound; }
  };
public:
  using forward_sentinel = forward_sentinel;
  using reverse_sentinel = reverse_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;
  using value_type = ValueType;

  Range(value_type _1, value_type _2) : init{ _1 }, bound{ _2 } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(init, bound) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(init, bound) }; }
  reverse_iterator rend() const { return {}; }
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

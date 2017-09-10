#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_REPEAT_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_REPEAT_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"


namespace cranberries {
namespace experimental {
namespace ranges {


template < class ValueType >
class Repeat
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
    ValueType value;
  public:
    using value_type = ValueType;
    sentinel_impl(ValueType _1) : value{ _1 } {}
    sentinel_impl(const sentinel_impl&) = default;
    auto get() { return value; }
    void next() {}
    std::false_type is_end() { return {}; }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using value_type = ValueType;

  Repeat(value_type _1) : value{ _1 } {}

  iterator begin() const { return { std::make_unique<sentinel>(value) }; }
  iterator end() const { return {}; }
private:
  value_type value;
};

namespace create {
  template < typename ValueType >
  Repeat<ValueType> repeat(ValueType value) { return {value}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_REPEAT_HPP

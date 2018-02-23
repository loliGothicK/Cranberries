#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_ITERATE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_ITERATE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"


namespace cranberries {
inline namespace experimental {
namespace traversals {


template < class ValueType, class Advance >
class Iterate
  : private tag::sentinel_traversal_tag
{
  class sentinel_impl {
    ValueType value;
    Advance advance;
  public:
    using value_type = ValueType;
    sentinel_impl(ValueType _1, Advance _2) : value{ _1 }, advance{ _2 } {}
    sentinel_impl(const sentinel_impl&) = default;
    auto get() const { return value; }
    std::false_type next() { value = advance(value); return {}; }
    std::false_type is_end() { return {}; }
  };
public:
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using value_type = ValueType;

  Iterate(value_type _1, Advance _2) : init{ _1 }, f{ _2 } {}

  iterator begin() const { return { std::make_unique<sentinel>(init, f) }; }
  iterator end() const { return {}; }
  size_t size() const { return -1; }
private:
  value_type init;
  Advance f;
};

namespace create {
  template < typename ValueType, class F >
  Iterate<ValueType,std::decay_t<F>> iterate(ValueType init, F&& f) { return {init, std::forward<F>(f)}; }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_CANONICAL_HPP

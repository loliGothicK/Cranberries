#ifndef CRANBERRIES_RANGES_DETAIL_DEFAULT_SENTINEL_HPP
#define CRANBERRIES_RANGES_DETAIL_DEFAULT_SENTINEL_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <memory>
#include <vector>

namespace cranberries {
namespace experimental {
namespace ranges {

namespace magic_arts {
  template < class ValueType >
  class default_forward_sentinel {
  public:
    using value_type = ValueType;
    template < class Range >
    default_forward_sentinel(Range&& range_)
      : range{}
      , iter{}
      , last{}
    {
      for (const auto& e : range_) range.emplace_back(e);
      iter = range.begin();
      last = range.end();
    }

    auto get() { return *iter; }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  private:
    std::vector<value_type> range;
    typename std::vector<value_type>::iterator iter;
    typename std::vector<value_type>::iterator last;
  };


  template < class ValueType >
  class default_reverse_sentinel {
  public:
    using value_type = ValueType;
    template < class Range >
    default_reverse_sentinel(Range&& range_)
      : range{}
      , iter{}
      , last{}
    {
      for (const auto& e : range_) range.emplace_back(e);
      iter = range.rbegin();
      last = range.rend();
    }

    auto get() { return *iter; }
    void next() { ++iter; }
    bool is_end() { return iter == last; }
  private:
    std::vector<value_type> range;
    typename std::vector<value_type>::reverse_iterator iter;
    typename std::vector<value_type>::reverse_iterator last;
  };

}

}}}
#endif
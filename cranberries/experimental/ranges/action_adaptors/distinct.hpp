#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DISTINCT_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DISTINCT_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Distinct
  : private tag::sentinel_range_tag
{
  class forward_sentinel {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    forward_sentinel(Range range)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
      , hash{}
    {
      hash[*iter] = true;
    }

    auto get() { return *iter; }
    bool next() { return (++iter != last) && ( hash[*iter] ? next() : (hash[*iter]=true) ); }
    bool is_end() { return iter == last; }
  private:
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
    std::unordered_map<value_type, bool> hash;
  };
  class reverse_sentinel {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    reverse_sentinel(Range range)
      : range_{ range }
      , iter{ cranberries::back_magic::rbegin(range_) }
      , last{ cranberries::back_magic::rend(range_) }
      , hash{}
    {
      hash[*iter] = true;
    }

    auto get() { return *iter; }
    bool next() { return (++iter != last) && (hash[*iter] ? next() : (hash[*iter] = true)); }
    bool is_end() { return iter == last; }
  private:
    Range range_;
    typename std::decay_t<Range>::reverse_iterator iter;
    typename std::decay_t<Range>::reverse_iterator last;
    std::unordered_map<value_type, bool> hash;
  };

public:
  using forward_sentinel = forward_sentinel;
  using reverse_sentinel = reverse_sentinel;
  using iterator = sentinel_iterator<forward_sentinel>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;
  using value_type = typename std::decay_t<Range>::value_type;

  Distinct(Range range)
    : range{ range } {}

  iterator begin() const { return { std::make_unique<forward_sentinel>(range) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(range) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
};

class DistinctProxy
  : private tag::adaptor_proxy_tag
{
public:
  DistinctProxy() = default;
  template < class Range >
  Distinct<Range> adapt_to(Range&& range) {
    return { std::forward<Range>(range) };
  }
};

namespace action {
  DistinctProxy distinct() { return {}; }
}

}}}
#endif
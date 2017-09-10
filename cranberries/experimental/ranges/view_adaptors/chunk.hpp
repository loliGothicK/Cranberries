#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_CHUNK_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_CHUNK_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <array>

namespace cranberries {
namespace experimental {
namespace ranges {

template < size_t N, class Range >
class Chunk
  : private tag::sentinel_range_tag
{
  class sentinel_impl {
  public:
    using value_type = typename std::decay_t<Range>::value_type;
    sentinel_impl(Range range)
      : range_{ range }
      , iter{ cranberries::back_magic::begin(range_) }
      , last{ cranberries::back_magic::end(range_) }
      , chunk{}
    {
      chunk[0] = *iter;
      for (size_t i{ 1 }; i < N; ++i) {
        if(++iter != last)
          { chunk[i] = *iter; }
        else break;
      }
    }

    auto get() { return chunk; }
    void next() { 
      for (auto& e : chunk) {
        if(++iter != last)
          { e = *iter; }
        else break;
        }
    }
    bool is_end() { return iter == last ; }
  private:
    Range range_;
    typename std::decay_t<Range>::iterator iter;
    typename std::decay_t<Range>::iterator last;
    std::array<value_type, N> chunk;
  };
public:
  using value_type = typename std::decay_t<Range>::value_type;
  using sentinel = sentinel_impl;
  using iterator = sentinel_iterator<sentinel>;
  using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
  using reverse_iterator = sentinel_iterator<reverse_sentinel>;

  Chunk(Range range)
    : range{ range } {}

  iterator begin() const { return { std::make_unique<sentinel>(range) }; }
  iterator end() const { return {}; }
  reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
  reverse_iterator rend() const { return {}; }

private:
  Range range;
};

template < size_t N >
class ChunkProxy
  : private tag::adaptor_proxy_tag
{
public:
  ChunkProxy() = default;

  template < class Range >
  Chunk<N, Range> adapt_to(Range&& range) {
    return { std::forward<Range>(range) };
  }
};

namespace view {
  template < size_t N >
  ChunkProxy<N> chunk() { return {}; }
}

}}}
#endif
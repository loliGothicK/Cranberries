#ifndef CRANBERRIES_STREAMS_OPERATORS_RADIX_SORT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_RADIX_SORT_HPP
#include <utility>
#include <type_traits>
#include "..\stream_error.hpp"
#include "..\utility.hpp"
#include "..\algorithm\radix_sort.hpp"
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename G,
    bool B
  >
  class RadixSort
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    RadixSort( G get_key ) : get_key{ std::forward<G>( get_key ) } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept
    {
      if (B)
        cranberries::ascending_radix_sort( stream_, std::forward<G>(get_key) );
      else
        cranberries::descending_radix_sort( stream_, std::forward<G>( get_key ) );

      return std::forward<Stream>( stream_ );
    }
  private:
    G get_key;
  };

  template <
    bool B
  >
  class RadixSort<detail::defaulted,B>
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    RadixSort() = default;

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept
    {
      if (B) {
        cranberries::ascending_radix_sort( stream_ );
      }
      else {
        cranberries::descending_radix_sort( stream_ );
      }
      return std::forward<Stream>( stream_ );
    }
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
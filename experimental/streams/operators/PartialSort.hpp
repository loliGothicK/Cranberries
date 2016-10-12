#ifndef CRANBERRIES_STREAMS_OPERATORS_PARTIAL_SORT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PARTIAL_SORT_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "..\stream_error.hpp"
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename Pred
  >
  class PartialSort
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    PartialSort( size_t n, Pred pred ) : n_{ n }, pred_{ std::forward<Pred>( pred ) } {}

    template <
      typename Stream
    >
      inline
      decltype(auto)
      operator()
      (
        Stream&& stream_
        ) {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::partial_sort( stream_.begin(), stream_.begin() + n_, stream_.end(), pred_ );
      return std::forward<Stream>( stream_ );
    }

  private:
    size_t n_;
    Pred pred_;
  };

  // Intermidiate Operation
  template < >
  class PartialSort <
    detail::defaulted_t // lookup operator < using ADL.
  >
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    PartialSort( size_t n ) : n_{ n } {};

    template <
      typename Stream
    >
      inline
      decltype(auto)
      operator()(
        Stream&& stream_
        ) {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::partial_sort( stream_.begin(), stream_.begin() + n_, stream_.end() );
      return std::forward<Stream>( stream_ );
    }

  private:
    size_t n_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
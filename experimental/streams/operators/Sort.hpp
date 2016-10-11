#ifndef CRANBERRIES_STREAMS_OPERATORS_SORT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SORT_HPP
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
  class Sort
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Sort( Pred pred ) noexcept
      : first{}
      , last{}
      , pred_{ std::forward<Pred>( pred ) }
    {}

    Sort( Pred pred, size_t first, size_t last ) noexcept
      : first{first}
      , last{last}
      , pred_{ std::forward<Pred>( pred ) }
    {}


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
      std::sort(stream_.begin(), stream_.end(), pred_);
      return std::forward<Stream>(stream_);
    }

  private:
    size_t first;
    size_t last;
    Pred pred_;
  };

  // Intermidiate Operation
  template < >
  class Sort <
    detail::defaulted // lookup operator < using ADL.
  >
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Sort() = default;

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    ) {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::sort(stream_.begin(), stream_.end());
      return std::forward<Stream>(stream_);
    }
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
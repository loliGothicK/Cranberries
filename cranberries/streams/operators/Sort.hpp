/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_SORT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SORT_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "../stream_error.hpp"
#include "../utility.hpp"
#include "../../common/defaulted_type.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename Pred
  >
  class Sort
    : private cranberries_magic::LazyOperationModuleBase
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
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      if (first == last)
        std::sort( stream_.begin(), stream_.end(), pred_ );
      else
        std::sort( stream_.begin() + first - 1, stream_.begin() + last, pred_ );
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
    cranberries_magic::defaulted_t // lookup operator < using ADL.
  >
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    Sort() = default;
    Sort( size_t first, size_t last ) noexcept
      : first{ first }
      , last{ last }
    {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      if (first == last)
        std::sort( stream_.begin(), stream_.end() );
      else
        std::sort( stream_.begin() + first - 1, stream_.begin() + last );
      return std::forward<Stream>(stream_);
    }
  private:
    size_t first{};
    size_t last{};
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
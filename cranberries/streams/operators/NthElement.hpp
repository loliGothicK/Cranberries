/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_NTH_ELEMENT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_NTH_ELEMENT_HPP
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
    class NthElement
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    NthElement( size_t n, Pred pred ) noexcept
      : n_{ n }
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
      std::nth_element( stream_.begin(), stream_.begin() + n_, stream_.end(), pred_ );
      return std::forward<Stream>( stream_ );
    }

  private:
    size_t n_;
    Pred pred_;
  };

  // Intermidiate Operation
  template < >
  class NthElement <
    cranberries_magic::defaulted_t // lookup operator < using ADL.
  >
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    NthElement( size_t n ) noexcept
      : n_{ n }
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
      std::nth_element( stream_.begin(), stream_.begin() + n_, stream_.end() );
      return std::forward<Stream>( stream_ );
    }

  private:
    size_t n_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
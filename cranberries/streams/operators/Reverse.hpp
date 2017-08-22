/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_REVERSE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_REVERSE_HPP
#include <utility>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  class Reverse
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    
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
      std::reverse(stream_.begin(), stream_.end());
      return std::forward<Stream>(stream_);
    }
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
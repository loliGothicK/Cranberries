/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_UNIQUE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_UNIQUE_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  struct UniqueProxy{};

  // Intermidiate Operation
  template <
    typename T
  >
  class Unique
    : private cranberries_magic::LazyOperationModuleBase
    , private cranberries_magic::StreamFilterBase
  {
  public:

    Unique() = default;

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
      auto&& source_ = stream_.get();
      source_.erase( std::unique( source_.begin(), source_.end() ), source_.end() );
      return std::forward<Stream>(stream_);
    }

    bool
    operator[]
    (
      T const& arg
    )
      noexcept
    {
      once( arg );
      if ( arg == prev ) {
        return true;
      }
      prev = arg;
      return false;
    }

    void once( T const& a ) {
      prev = a + 1;
      once_flag = false;
    }

  private:
    T prev;
    bool once_flag = true;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
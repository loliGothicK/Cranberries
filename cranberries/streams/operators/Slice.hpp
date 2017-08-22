/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_SLICE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SLICE_HPP
#include <utility>
#include "../cranberries_magic/tag.hpp"
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  class Slice
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    Slice( size_t l, size_t u ) noexcept
      : i{ 1 }, low_{ l }, up_{ u }
    {}

    Slice() = default;
    Slice( Slice const& ) = default;
    Slice( Slice&& ) = default;
    Slice& operator=( Slice const& ) = default;
    Slice& operator=(Slice&&) = default;

    template <
      typename Stream
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      auto&& src = stream_.get();
      for ( auto&& iter = src.begin(); iter != src.end(); ++i ) {
        if ( i < low_ || up_ < i ) iter = src.erase( iter );
        else { ++iter; }
      }
      return std::forward<Stream>( stream_ );
    }

    template <
      typename T
    >
    bool
    operator[]
    (
      T&& // no use
    )
      noexcept
    {
      return i++ < low_ || up_ < i;
    }

    size_t low() const { return low_; }
    size_t up() const { return up_; }

  private:
    size_t i;
    size_t low_;
    size_t up_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif

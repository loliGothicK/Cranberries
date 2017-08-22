/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_FLAT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FLAT_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "../utility.hpp"
#include "../workaround.hpp"

namespace cranberries {
namespace streams {
namespace operators {




  class FlatProxy {};

  template <
    typename Stream
  >
  class Flat
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    Flat( Stream x ) noexcept
      : old_{ std::forward<Stream>( x ) }
    {}

    template <
      typename STREAM
    >
    decltype(auto)
    operator()
    (
      STREAM&& stream_
    )
      noexcept(false)
    {
     old_.eval();
     CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( old_.empty() );
      using std::begin; using std::end;
      for(auto&& e : old_)
        stream_.insert( stream_.end(), begin(e), end(e) );

      return std::forward<STREAM>( stream_ );
    }


  private:
    Stream old_;
  };

  class FlatAllProxy {};

  template <
    typename Stream
  >
  class FlatAll
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    FlatAll( Stream x ) noexcept
      : old_{ std::forward<Stream>( x ) }
    {}

    template <
      typename STREAM
    >
    decltype(auto)
    operator()
    (
      STREAM&& stream_
    )
      noexcept(false)
    {
      old_.eval();
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( old_.empty() );
      using std::begin; using std::end;
      for (auto&& e : old_)
        streams::workaround::expand_left < is_range_v<decltype(e)> >::loop( std::forward<STREAM>( stream_ ), std::forward<decltype(e)>( e ) );
      return std::forward<STREAM>( stream_ );
    }


  private:
    Stream old_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
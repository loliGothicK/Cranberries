/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_INVOKER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_INVOKER_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Adaptor for user defined operation and Stream operation interface.
  // Concept : Operator must take Stream in argument and return reference of it.
  template <
    typename Operator // user defined operation
  >
  class Tinvoker
    : cranberries_magic::EagerOperationModuleBase
  {
  public:
    Tinvoker( Operator op ) noexcept
      :op_{ std::forward<Operator>( op ) }
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
      static_assert(
        is_callable_v<Operator,Stream&&>,
        "Invalid operator designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      op_(stream_);
      return std::forward<Stream>(stream_);
    }

  private:
    Operator op_;
  };

  template <
    typename Operator // user defined operation
  >
  class Iinvoker
    : cranberries_magic::LazyOperationModuleBase
  {
  public:
    Iinvoker( Operator op ) noexcept
      : op_{ std::forward<Operator>( op ) }
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
      static_assert(
        is_callable_v<Operator,Stream&&>,
        "Invalid operator designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      op_(stream_);
      return std::forward<Stream>(stream_);
    }

  private:
    Operator op_;
  };



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
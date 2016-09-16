#ifndef CRANBERRIES_STREAMS_OPERATORS_INVOKER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_INVOKER_HPP
#include <utility>
#include <type_traits>
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Adaptor for user defined operation and Stream operation interface.
  // Concept : Operator must call with arg of any Stream and return it.
  template <
    typename Operator // user defined operation
  >
  class Tinvoker
    : detail::TerminateStreamOperatorBase
  {
  public:
    Tinvoker( Operator op ) :op_{ std::forward<Operator>( op ) } {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
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
    : detail::IntermidiateStreamOperatorBase
  {
  public:
    Iinvoker( Operator op ) : op_{ std::forward<Operator>( op ) } {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream
    ) {
      static_assert(
        is_callable_v<Operator,Stream&&>,
        "Invalid operator designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      op_(stream);
      return std::forward<Stream>(stream);
    }

  private:
    Operator op_;
  };



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
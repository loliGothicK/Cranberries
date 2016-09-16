#ifndef CRANBERRIES_STREAMS_OPERATORS_FOREACH_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FOREACH_HPP
#include <utility>
#include <type_traits>
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename UnaryFunc
  >
  class ForEach
    : private detail::TerminateStreamOperatorBase
  {
  public:
    ForEach( UnaryFunc f ) : f_{ std::forward<UnaryFunc>( f ) } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      static_assert(
        is_callable_v<UnaryFunc,E&>,
        "Invalid unary function designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      for (auto&& e : stream_) f_(e);
      return std::forward<Stream>(stream_);
    }

  private:
    UnaryFunc f_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
#ifndef CRANBERRIES_STREAMS_OPERATORS_PEEK_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PEEK_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename UnaryFunc
  >
  class Peek
    : private detail::IntermidiateStreamOperatorBase
    , private detail::StreamOperatorBase
  {
  public:
    Peek( UnaryFunc f ) noexcept
      : f_{ std::forward<UnaryFunc>( f ) }
    {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
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
        is_callable_v<UnaryFunc,Stream&&>,
        "Invalid unary function designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      for (auto&& e : stream_) f_(e);
      return std::forward<Stream>(stream_);
    }

    template <
      typename T
    >
    decltype(auto)
    operator[]
    (
      T&& a 
    )
      noexcept
    {
      return f_( a ), a;
    }

  private:
    UnaryFunc f_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
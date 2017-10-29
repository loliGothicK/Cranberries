/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_PEEK_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PEEK_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"
#include "../../common/defaulted_type.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template < >
  class Peek<cranberries_magic::defaulted_t>
    : private cranberries_magic::LazyOperationModuleBase
    , private cranberries_magic::StreamOperatorBase
  {
  public:
    constexpr Peek() noexcept
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
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF(stream_.empty());
      auto&& iter = stream_.begin();
      std::cout << *iter;
      ++iter;
      for (; iter != stream_.end(); ++iter)
        std::cout << ", " << *iter;
      std::cout << std::endl;
      return std::forward<Stream>(stream_);
    }
  };


  template <
    typename UnaryFunc
  >
  class Peek
    : private cranberries_magic::LazyOperationModuleBase
    , private cranberries_magic::StreamOperatorBase
    , private cranberries_magic::SequencialOperatorBase
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
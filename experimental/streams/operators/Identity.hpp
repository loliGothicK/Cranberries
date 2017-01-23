#ifndef CRANBERRIES_STREAMS_OPERATORS_ROOTOPERATOIN_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ROOTOPERATOIN_HPP
#include <utility>
#include "../cranberries_magic/tag.hpp"
#include "../utility.hpp"
#include "../io.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  struct Identity
  {
    template <
      typename Stream,
      std::enable_if_t<is_range_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept
    {
      return std::forward<Stream>(stream_);
    }

    template <
      typename T,
      std::enable_if_t<!is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
    decltype(auto)
    operator()( T&& a ) noexcept {
      return std::forward<T>( a );
    }

  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
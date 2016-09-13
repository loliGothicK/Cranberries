#ifndef CRANBERRIES_STREAMS_OPERATORS_ROOTOPERATOIN_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ROOTOPERATOIN_HPP
#include <utility>
#include "../detail/tag.hpp"
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  struct Identity
  {
    using tree_tag = detail::not_tree;

    template <
      typename STREAM,
      std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
    >
    inline
    decltype(auto)
    operator()
    (
      STREAM&& stream
    )
      noexcept
    {
      return std::forward<STREAM>(stream);
    }

    template <
      typename T,
      std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
    decltype(auto)
    operator()( T&& a ) {
      return std::forward<T>( a );
    }

  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
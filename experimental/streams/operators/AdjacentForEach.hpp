#ifndef CRANBERRIES_STREAMS_OPERATORS_ADJACENTFOREACH_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ADJACENTFOREACH_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename BinaryFunc
  >
  class AdjacentForEach
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    AdjacentForEach( BinaryFunc f ) noexcept
      : f_{ std::forward<BinaryFunc>( f ) }
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
    ) {
      static_assert(
        is_callable_v<BinaryFunc,E&,E&>,
        "Invalid binary operator designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      cranberries::adjacent_for_each(  stream_, f_ );
      return std::forward<Stream>(stream_);
    }

  private:
    BinaryFunc f_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
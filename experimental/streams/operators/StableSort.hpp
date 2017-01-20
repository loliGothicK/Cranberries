#ifndef CRANBERRIES_STREAMS_OPERATORS_STABLE_SORT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_STABLE_SORT_HPP
#include <string>
#include <regex>
#include <utility>
#include <algorithm>
#include "../stream_error.hpp"
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename Pred
  >
    class StableSort
    : private cranberries_magic::LazyOpeartionModuleBase
  {
  public:
    StableSort( Pred pred ) : pred_{ std::forward<Pred>( pred ) } {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::stable_sort( stream_.begin(), stream_.end(), pred_ );
      return std::forward<Stream>( stream_ );
    }

  private:
    Pred pred_;
  };

  // Intermidiate Operation
  template < >
  class StableSort <
    cranberries_magic::defaulted_t // lookup operator < using ADL.
  >
    : private cranberries_magic::LazyOpeartionModuleBase
  {
  public:
    StableSort() = default;

    template <
      typename Stream
    >
      inline
      decltype(auto)
      operator()(
        Stream&& stream_
        ) {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::stable_sort( stream_.begin(), stream_.end() );
      return std::forward<Stream>( stream_ );
    }
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
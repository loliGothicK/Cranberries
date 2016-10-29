#ifndef CRANBERRIES_STREAMS_OPERATORS_LIMIT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_LIMIT_HPP
#include <utility>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  class Take
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Take( size_t n ) noexcept
      : lim{ n }
    {}
    
    template <
      typename Stream,
      typename Range = typename std::decay_t<Stream>::range_type
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      auto&& src = stream_.get();
      auto&& iter = src.begin() + lim;
      while ( iter != src.end() ) iter = src.erase( iter );
      return std::forward<Stream>(stream_);
    }

    std::size_t lim;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
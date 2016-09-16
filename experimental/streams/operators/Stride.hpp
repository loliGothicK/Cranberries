#ifndef CRANBERRIES_STREAMS_OPERATORS_STRIDE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_STRIDE_HPP
#include <utility>
#include "..\detail\tag.hpp"
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  class Stride
    : private detail::IntermidiateStreamOperatorBase
    , private detail::StreamFilterBase
  {
  public:
    Stride(size_t s) : step{s}, i{} {}

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
      auto&& src = stream_.get();
      for ( auto&& iter = src.begin(); iter != src.end(); ++i ) {
        if ( i%step == 0 ) ++iter;
        else { iter = src.erase( iter ); }
      }
      src.shrink_to_fit();
      return std::forward<Stream>(stream_);
    }

    template <
      typename T
    >
    inline
    decltype(auto)
    operator[]
    (
      T&& // ! no use
    )
      noexcept
    {
      return ++i%step!=1; 
    }

  private:
    size_t i{};
    size_t step;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
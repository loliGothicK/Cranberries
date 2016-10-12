#ifndef CRANBERRIES_STREAMS_OPERATORS_TAKEWHILE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_TAKEWHILE_HPP
#include <string>
#include <regex>
#include <utility>
#include "..\stream_error.hpp"
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename Pred
  >
  class TakeWhile
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    TakeWhile( Pred pred ) noexcept
      : pred_{ std::forward<Pred>( pred ) }
    {}

    template <
      typename Stream
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      auto&& src = stream_.get();
      auto&& iter = src.begin();
      // peel loop
      while ( pred_( *iter ) ) ++iter;
      // delete loop
      while ( iter != src.end() ) iter = src.erase( iter );
      return std::forward<Stream>( stream_ );
    }

    template <
      typename T
    >
    bool
    operator[]
    (
      T&& arg
    )
      noexcept
    {
      if ( flag && pred_(arg) ) {
        return true;
      }
      else if ( !flag ) {
        return false;
      }
      else {
        flag = false;
        return false;
      }
      return false;
    }
  private:
    Pred pred_;
    bool flag = true;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
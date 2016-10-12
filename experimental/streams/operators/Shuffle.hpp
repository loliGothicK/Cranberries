#ifndef CRANBERRIES_STREAMS_OPERATORS_SHUFFLE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SHUFFLE_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include <random>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename Engine
  >
  class Shuffle
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Shuffle( Engine e ) noexcept
      : engine_{ std::forward<Engine>( e ) }
    {}

    Shuffle() = default;

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::shuffle( stream_.begin(), stream_.end(), engine_ );
      return std::forward<Stream>(stream_);
    }

  private:
    Engine engine_;
  };



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
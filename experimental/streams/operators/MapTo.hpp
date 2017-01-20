#ifndef CRANBERRIES_STREAMS_OPERATORS_MAPTO_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MAPTO_HPP
#include <string>
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  struct ToString {};

  template < typename To >
  struct MapToProxy {};


  template <
    typename FromStream,
    typename To
  >
  class MapTo
    : private cranberries_magic::LazyOpeartionModuleBase
  {
  public:
    MapTo( FromStream s ) noexcept
      : from_{ std::forward<FromStream>( s ) }
    {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      from_.eval();
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( from_.empty() );
      stream_.get() = std::vector<To>{ from_.begin(), from_.end() };
      return std::forward<Stream>(stream_);
    }


  private:
    FromStream from_;
  };

  template <
    typename FromStream
  >
  class MapTo<
    FromStream,
    std::string
  >
    : cranberries_magic::LazyOpeartionModuleBase
  {
  public:
    MapTo( FromStream s ) noexcept
      : from_{ std::forward<FromStream>( s ) }
    {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {   
      using std::to_string; // For ADL
      from_.eval();
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( from_.empty() );
      for (auto&& e : from_) stream_.emplace_back(to_string(e));
      return std::forward<Stream>(stream_);
    }


  private:
    FromStream from_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
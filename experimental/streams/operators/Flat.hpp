#ifndef CRANBERRIES_STREAMS_OPERATORS_FLAT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FLAT_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {


namespace {
namespace workaround {
  template < bool B >
  struct for_msvc2 {
    template < typename Stream, typename Elem >
    static void loop(Stream&& stream_, Elem&& elem_) {
      for (auto&& e : elem_)
        for_msvc2 <
          is_range_v<decltype(e)>
        >::loop( std::forward<Stream>( stream_ ), std::forward<decltype(e)>( e ) );
    }
  };
  template < >
  struct for_msvc2<false> {
    template < typename Stream, typename Elem >
    static void loop( Stream&& stream_, Elem&& elem_ ) {
      stream_.emplace_back( elem_ );
    }
  };
}
}


  class FlatProxy {};

  template <
    typename Stream
  >
  class Flat
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Flat( Stream x ) : old_{ std::forward<Stream>( x ) } {}

    template <
      typename STREAM
    >
    decltype(auto)
    operator()
    (
      STREAM&& stream_
    ) {
     old_.eval();
     CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( old_.empty() );
      using std::begin; using std::end;
      for(auto&& e : old_)
        stream_.insert( stream_.end(), begin(e), end(e) );

      return std::forward<STREAM>( stream_ );
    }


  private:
    Stream old_;
  };

  class FlatAllProxy {};

  template <
    typename Stream
  >
  class FlatAll
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    FlatAll( Stream x ) : old_{ std::forward<Stream>( x ) } {}

    template <
      typename STREAM
    >
    decltype(auto)
    operator()
    (
      STREAM&& stream_
    ) {
      old_.eval();
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( old_.empty() );
      using std::begin; using std::end;
      for (auto&& e : old_)
        workaround::for_msvc2 < is_range_v<decltype(e)> >::loop( std::forward<STREAM>( stream_ ), std::forward<decltype(e)>( e ) );
      return std::forward<STREAM>( stream_ );
    }


  private:
    Stream old_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
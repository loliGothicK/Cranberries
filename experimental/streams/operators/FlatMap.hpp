#ifndef CRANBERRIES_STREAMS_OPERATORS_FLATMAP_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FLATMAP_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "..\utility.hpp"


namespace cranberries {
namespace streams {
namespace operators {

namespace {
namespace workaround {

  template <
      bool IsStream
  >
  struct for_msvc
  {
    template <
      typename Stream,
      typename Range
    >
    static
    void
    invoke
    (
      Stream&& stream_,
      Range&& range_
    ) {
      using std::begin; using std::end;
      stream_.insert( stream_.end(), begin( range_ ), end( range_ ) );
    }
  };

  template < >
  struct for_msvc<true>
  {
    template <
      typename Stream,
      typename Range
    >
    static
    void
    invoke
    (
      Stream&& stream_,
      Range&& range_
    ) {
      range_.eval();
      stream_.insert( stream_.end(), range_.begin(), range_.end() );
    }
  };

} // ! namespace workaround
} // ! anonymous-namespace


  template <
    typename UnaryOp
  >
  class FlatMap
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    FlatMap( UnaryOp op ) : op_{ std::forward<UnaryOp>( op ) } {}

    template <
      typename Stream,
      typename T
    >
    void
    push_tuple
    (
      Stream&& stream_,
      T&& arg
    ) {
      stream_.emplace_back(std::move(arg));
      return;
    }

    template <
      typename Stream,
      typename Head,
      typename ...Tail
    >
    void
    push_tuple(
      Stream&& stream_,
      Head&& head,
      Tail&& ...tail
    ) {
      return stream_.emplace_back( std::move( head ) ), push_tuple( std::forward<Stream>( stream_ ), std::forward<Tail>( tail )... );
    }


    template <
      typename T,
      typename Stream,
      std::enable_if_t<is_tuple_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
    void
    push
    (
      Stream&& stream_,
      T&& proj
    ) {
      cranberries::apply(
        [&]( auto&& ...args ) {
        push_tuple( std::forward<Stream>( stream_ ), std::forward<decltype( args )>( args )... ); 
      }, std::forward<T>( proj ) );
    }

    template <
      typename Stream,
      typename Range,
      std::enable_if_t<
        is_range_v<std::decay_t<Range>>,
        std::nullptr_t
      > = nullptr
    >
    void
    push
    (
      Stream&& stream_,
      Range&& range_
    ) {
      workaround::for_msvc<
        detail::is_finite_stream_v<std::decay_t<Range>>
      >::invoke(std::forward<Stream>(stream_), std::forward<Range>(range_));
    }

    template <
      typename Stream
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      size_t ini = stream_.size();
      for ( auto&& e : stream_ ) {
        push( stream_, op_( e ) );
      }
      stream_.erase( stream_.begin(), stream_.begin() + ini );
      return std::forward<Stream>( stream_ );
    }


    template <
      typename T
    >
    decltype(auto)
    operator[]
    (
      T&& a
    ) {
      return op_(a);
    }

  private:
    UnaryOp op_;
  };

} // ! namespace operators
} // ! namespace streams
} // ! namespace cranberries


#endif
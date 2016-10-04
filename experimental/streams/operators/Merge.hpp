#ifndef CRANBERRIES_STREAMS_OPERATORS_MERGE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MERGE_HPP
#include <string>
#include <iostream>
#include <utility>
#include <type_traits>
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {


  template <
    typename Range,
    bool IsFiniteStream = detail::is_finite_stream_v<std::decay_t<Range>>,
    bool IsInfiniteStream = detail::is_infinite_stream_v<std::decay_t<Range>>
  >
  class Merge
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Merge() = default;
    Merge( Range range ) : range_{ std::forward<Range>( range ) } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      static_assert(
        std::is_constructible<
          E, element_type_of_t<Range>
        >::value,
        "" // TODO
      );
      typename std::decay_t<Stream>::range_type result{};
      auto&& lv = stream_.get();
      auto&& rv = range_.get();
      lv.reserve( lv.size() + rv.size() );
      auto&& first = lv.begin();
      auto&& middle = lv.end();
      lv.insert(middle, rv.begin(), rv.end());
      std::inplace_merge( first, middle, lv.end() );
      return std::forward<Stream>( stream_ );
    }

    decltype( auto ) release() { return std::move( range_ ); }

  private:
    Range range_;
  };

  template <
    typename Range
  >
  class Merge<
    Range,false,false
  >
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Merge() = default;
    Merge( Range range ) : range_{ std::forward<Range>( range ) } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      static_assert(
        std::is_constructible<
          E, element_type_of_t<Range>
        >::value,
        "" // TODO
      );
      typename std::decay_t<Stream>::range_type result{};
      using std::begin; using std::end;
      auto&& lv = stream_.get();
      lv.reserve( lv.size() + cranberries::size( range_ ) );
      auto&& first = lv.begin();
      auto&& middle = lv.end();
      lv.insert(middle, begin( range_ ), end( range_ ));
      std::inplace_merge( first, middle, lv.end() );
      return std::forward<Stream>( stream_ );
    }

    decltype( auto ) release() { return stream<element_type_of_t<Range>>{std::move( range_ )}; }
  private:
    Range range_;
  };

  template <
    typename Range
  >
  class Merge<
    Range,false,true
  >
    : private detail::StreamOperatorBase
  {
  public:
    Merge() = default;
    Merge( Range range ) : range_{ std::forward<Range>( range ) } {}

    decltype( auto ) release() { return std::move( range_ ); }

  private:
    Range range_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
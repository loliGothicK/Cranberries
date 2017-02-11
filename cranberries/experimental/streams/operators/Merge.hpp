#ifndef CRANBERRIES_STREAMS_OPERATORS_MERGE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MERGE_HPP
#include <string>
#include <iostream>
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {


  template <
    typename Range,
    bool IsFiniteStream = cranberries_magic::is_finite_stream_v<std::decay_t<Range>>,
    bool IsInfiniteStream = cranberries_magic::is_infinite_stream_v<std::decay_t<Range>>
  >
  class Merge
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    Merge() = default;
    Merge( Range range ) noexcept
      : range_{ std::forward<Range>( range ) }
    {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept
    {
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

    decltype( auto ) release() noexcept { return std::move( range_ ); }

  private:
    Range range_;
  };

  template <
    typename Range
  >
  class Merge<
    Range,false,false
  >
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    Merge() = default;
    Merge( Range range ) noexcept
      : range_{ std::forward<Range>( range ) }
    {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept
    {
      static_assert(
        std::is_constructible<
          E, element_type_of_t<Range>
        >::value,
        "" // TODO
      );
      typename std::decay_t<Stream>::range_type result{};
      using std::begin; using std::end;
      auto&& lv = stream_.get();
      auto mid = lv.size();
      lv.reserve( lv.size() + cranberries::size( range_ ) );
      lv.insert(lv.end(), begin( range_ ), end( range_ ));
      std::inplace_merge( lv.begin(), lv.begin()+mid, lv.end() );
      return std::forward<Stream>( stream_ );
    }

    decltype( auto ) release() noexcept { return stream<element_type_of_t<Range>>{std::move( range_ )}; }
  private:
    Range range_;
  };

  template <
    typename Range
  >
  class Merge<
    Range,false,true
  >
    : private cranberries_magic::StreamOperatorBase
  {
  public:
    Merge() = default;
    Merge( Range range ) noexcept
      : range_{ std::forward<Range>( range ) }
    {}

    decltype( auto ) release() noexcept { return std::move( range_ ); }

  private:
    Range range_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
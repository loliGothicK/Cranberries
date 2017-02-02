#ifndef CRANBERRIES_STREAMS_OPERATORS_SET_OPERATION_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SET_OPERATION_HPP
#include <utility>
#include <algorithm>
#include <iterator>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename Range
  >
  class SetUnion
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    SetUnion( Range range ) noexcept
      : set_{ std::forward<Range>( range ) }
    {}

    template <
      typename Stream,
      typename Result = typename std::decay_t<Stream>::range_type
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      using std::begin; using std::end; using cranberries::size;
      Result result{};
      auto&& src = stream_.get();
      std::set_union(
        src.begin(), src.end(),
        begin( set_ ), end( set_ ),
        std::back_inserter(result)
      );
      result.swap( src );
      return std::forward<Stream>( stream_ );
    }

  private:
    Range set_;
  };

  template <
    typename Range
  >
  class SetIntersection
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    SetIntersection( Range range ) noexcept
      : set_{ std::forward<Range>( range ) }
    {}

    template <
      typename Stream,
      typename Result = typename std::decay_t<Stream>::range_type
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      using std::begin; using std::end; using cranberries::size;
      Result result{};
      auto&& src = stream_.get();
      std::set_intersection(
        src.begin(), src.end(),
        begin( set_ ), end( set_ ),
        std::back_inserter(result)
      );
      result.swap( src );
      return std::forward<Stream>( stream_ );
    }

  private:
    Range set_;
  };

  template <
    typename Range
  >
  class SetDiff
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    SetDiff( Range range ) noexcept
      : set_{ std::forward<Range>( range ) }
    {}

    template <
      typename Stream,
      typename Result = typename std::decay_t<Stream>::range_type
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      using std::begin; using std::end; using cranberries::size;
      Result result{};
      auto&& src = stream_.get();
      std::set_difference(
        src.begin(), src.end(),
        begin( set_ ), end( set_ ),
        std::back_inserter(result)
      );
      result.swap( src );
      return std::forward<Stream>( stream_ );
    }

  private:
    Range set_;
  };

  template <
    typename Range
  >
  class SetSymmetricDiff
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    SetSymmetricDiff( Range range ) noexcept
      : set_{ std::forward<Range>( range ) }
    {}

    template <
      typename Stream,
      typename Result = typename std::decay_t<Stream>::range_type
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      using std::begin; using std::end; using cranberries::size;
      Result result{};
      auto&& src = stream_.get();
      std::set_symmetric_difference(
        src.begin(), src.end(),
        begin( set_ ), end( set_ ),
        std::back_inserter(result)
      );
      result.swap( src );
      return std::forward<Stream>( stream_ );
    }

  private:
    Range set_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
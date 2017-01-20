#ifndef CRANBERRIES_STREAMS_OPERATORS_REDUCERS_HPP
#define CRANBERRIES_STREAMS_OPERATORS_REDUCERS_HPP
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <utility>
#include <cstddef>
#include "../utility.hpp"
#include "../stat/stat.hpp"

namespace cranberries{
namespace streams{
namespace operators{

  template <
    typename InitialType,
    typename BinaryOp
  >
  class Accumulate
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Accumulate( InitialType ini, BinaryOp op ) noexcept
      : init_{ ini }, op_{ op }
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
      noexcept(false)
    {
      static_assert(
        is_callable_v<BinaryOp,InitialType&,E&>,
        "Invalid binary operator (or invalid initial type) designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::accumulate( stream_.begin(), stream_.end(), init_, op_ );
    }
  private:
    InitialType init_;
    BinaryOp op_;
  };

  template <
    typename InitialType
  >
  class Accumulate<
    InitialType,
    cranberries_magic::defaulted_t
  >
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Accumulate( InitialType ini ) : init_{ ini } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::accumulate( stream_.begin(), stream_.end(), init_);
    }

    InitialType init_;
  };

  template < >
  class Accumulate<
    cranberries_magic::defaulted_t,
    cranberries_magic::defaulted_t
  >
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Accumulate() = default;

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::accumulate( stream_.begin(), stream_.end(), E{} );
    }

  };


  class Product
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Product() = default;

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype( auto )
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::accumulate( stream_.begin(), stream_.end(), E{ 1 }, []( auto&& l, auto&& r ) { return l*r; } );
    }
  };

  template <
    typename Pred
  >
  class SumIf
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    SumIf( Pred pred ) noexcept
      : pred_{ std::forward<Pred>( pred ) }
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
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      E result{};
      for(auto&& e : stream_){
        if ( pred_( e ) ) result += e;
      }
      return result;
    }

    Pred pred_;
  };

  template <
    typename Target
  >
  class Count
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Count( Target val ) noexcept
      : value_{ val }
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
      noexcept(false)
    {
      static_assert(
        is_equality_comparable_to_v<E,Target>,
        "" // TODO
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::count(stream_.begin(), stream_.end(), value_);
    }

    Target value_;
  };

  template <
    typename Pred
  >
  class CountIf
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    CountIf( Pred pred ) noexcept
      : pred_{ std::forward<Pred>( pred ) }
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
      noexcept(false)
    {
      static_assert(
        is_callable_v<Pred,E>,
        "Invalid predicate dsignated."
      );
      static_assert(
        std::is_same<
          bool, std::result_of_t<Pred(E)>
        >::value,
        "predicate must be return bool."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::count_if(stream_.begin(), stream_.end(), pred_);
    }
  private:
    Pred pred_;
  };

  class Average
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Average() = default;

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      return std::accumulate(stream_.begin(), stream_.end(), 0.0) / stream_.size();
    }

  };

  class Median
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Median() = default;

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::sort( stream_.begin(), stream_.end() );
      auto len = stream_.size();
      return len % 2 == 1
        ? static_cast<double>(stream_[len / 2])
        : ( stream_[len / 2 - 1] + stream_[len / 2] ) / 2.0;
    }

  };

  class Mode
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    Mode() = default;

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
      std::sort( stream_.begin(), stream_.end() );
      auto mid = stream_.begin(), prev = stream_.begin();
      auto end = stream_.end();
      auto w = ( *prev - *(end-1) ) / 5.0;
      int n{};
      double result;
      for ( size_t i{ 1 }; i < 5; ++i ) {
        mid = std::upper_bound( stream_.begin(), stream_.end(), w*i );
        if ( n < std::distance( prev, mid ) ) result = cranberries::stat::median( prev, mid );
        prev = mid;
      }
      if ( n < std::distance( prev, end ) ) result = cranberries::stat::median( prev, end );
      return result;
    }

  };

} // ! namespace operators
} // ! namespace streams
} // ! namespace cranberries
#endif
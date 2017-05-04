#ifndef CRANBERRIES_STREAMS_OPERATORS_FLATMAP_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FLATMAP_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "../utility.hpp"
#include "../workaround.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename UnaryOp
  >
  class FlatMap
    : private cranberries_magic::LazyOperationModuleBase
  {
  public:
    FlatMap( UnaryOp op ) noexcept
      : op_{ std::forward<UnaryOp>( op ) }
    {}

    template <
      typename Stream,
      typename Tuple,
      size_t... I
    >
    void
    push_tuple(
      Stream&& stream_,
      Tuple&& t,
      std::index_sequence<I...>
    )
      noexcept
    {
      using swallow = std::initializer_list<int>;
      (void)swallow {
        (void(stream_.emplace_back(std::get<I>(t))), 0)...
      };
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
    )
      noexcept
    {
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
    )
      noexcept
    {
      workaround::front_emplacer<
        cranberries_magic::is_finite_stream_v<std::decay_t<Range>>
      >::invoke(std::forward<Stream>(stream_), std::forward<Range>(range_));
    }

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
      auto old = stream_.get();
      stream_.clear();
      for (auto&& e : old ) {
        push( stream_, op_( e ) );
      }
      return std::forward<Stream>( stream_ );
    }


    template <
      typename T
    >
    decltype(auto)
    operator[]
    (
      T&& a
    )
      noexcept
    {
      return op_(a);
    }

  private:
    UnaryOp op_;
  };

} // ! namespace operators
} // ! namespace streams
} // ! namespace cranberries


#endif
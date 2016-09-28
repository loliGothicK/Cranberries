#ifndef CRANBERRIES_STREAMS_OPERATORS_PARTITIONING_BY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PARTITIONING_BY_HPP
#include <utility>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <deque>
#include "..\utility.hpp"
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {


  template <
    opt Option,
    template <class T, class Allocator = std::allocator<T>> class Seeq,
    typename Pred
  >
  class PartitioningBy
    : private detail::TerminateStreamOperatorBase
  {
  public:
    PartitioningBy( Pred p ) : pred_{ std::forward<Pred>( p ) } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator ()
    (
      Stream&& stream_
    ) {
      static_assert(
        is_callable_v<Pred, E&>,
        "Invalid predicate designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      using Val = Seeq<E>;

      auto result = MapTraits<Option>:: template type<std::result_of_t<Pred( E )>, Val>{};

      auto middle = std::partition( stream_.begin(), stream_.end(), pred_ );

      result.emplace(
        std::piecewise_construct,
        std::forward_as_tuple( true ),
        std::forward_as_tuple( stream_.begin(), middle )
      );
      result.emplace(
        std::piecewise_construct,
        std::forward_as_tuple( false ),
        std::forward_as_tuple( middle, stream_.end() )
      );
      return result;
    }

  private:
    Pred pred_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
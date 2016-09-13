#ifndef CRANBERRIES_STREAMS_INIFINITE_OPERATIONS
#define CRANBERRIES_STREAMS_INIFINITE_OPERATIONS

#include <type_traits>
#include "detail\tag.hpp"
#include "forward.hpp"
#include "InfiniteStream.hpp"
#include "operators\TakeWhile.hpp"
#include "operators\Take.hpp"
#include "operators\Distinct.hpp"
#include "utility.hpp"

namespace cranberries {
namespace streams {


  template <
    typename Stream1, typename Stream2,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream1> || detail::is_infinite_stream_v<Stream2>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamMerger<Stream1, Stream2>
  operator >>
  (
    Stream1&& stream_,
    operators::Merge<Stream2>&& merge
  )
    noexcept
  {
    return{ std::forward<Stream1>(stream_), std::forward<Stream2>(merge.stream_) };
  }

  template <
    typename Stream, typename F,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamOperator<Stream,operators::Transform<F>>
  operator>>
  (
    Stream&& stream_,
    operators::Transform<F>&& trans_
  )
    noexcept
  {
    static_assert(
      is_callable_v<F,typename std::decay_t<Stream>::type>,
      "Not callable"
    );
    return{ std::forward<Stream>( stream_ ), std::move( trans_ ) };
  }

  template <
    typename Stream, typename F,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamOperator<Stream,operators::Peek<F>>
  operator>>
  (
    Stream&& stream_,
    operators::Peek<F>&& peek_
  )
    noexcept
  {
    static_assert(
      is_callable_v<F,typename std::decay_t<Stream>::type>,
      "Not callable"
    );
    return{ std::forward<Stream>( stream_ ), std::move( peek_ ) };
  }


  template <
    typename Stream, typename Pred,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamFilter<Stream, operators::Filter<Pred>>
  operator>>
  (
    Stream&& stream_,
    operators::Filter<Pred>&& filter
  )
    noexcept
  {
    static_assert(
      is_callable_v<Pred, typename std::decay_t<Stream>::type>,
      ""
    );
    static_assert(
      std::is_same<bool, std::result_of_t<Pred( typename std::decay_t<Stream>::type )>>::value,
      ""
    );

    return{ std::forward<Stream>( stream_ ), std::move( filter ) };
  }

  template <
    typename Stream, typename Pred,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamFilter<Stream, operators::DropWhile<Pred>>
  operator>>
  (
    Stream&& stream_,
    operators::DropWhile<Pred>&& dw
  )
    noexcept
  {
    static_assert(
      is_callable_v<Pred, typename std::decay_t<Stream>::type>,
      ""
    );
    static_assert(
      std::is_same<bool, std::result_of_t<Pred( typename std::decay_t<Stream>::type )>>::value,
      ""
    );

    return{ std::forward<Stream>( stream_ ), std::move( dw ) };
  }

  template <
    typename Stream,
    typename T = typename std::decay_t<Stream>::type,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamFilter<Stream, operators::Distinct<T>>
  operator>>
  (
    Stream&& stream_,
    operators::DistinctProxy&&
  )
    noexcept
  {
    return{ std::forward<Stream>( stream_ ), operators::Distinct<T>{} };
  }

  template <
    typename Stream,
    typename T = typename std::decay_t<Stream>::type,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamFilter<Stream, operators::Unique<T>>
  operator>>
  (
    Stream&& stream_,
    operators::UniqueProxy&&
  )
    noexcept
  {
    return{ std::forward<Stream>( stream_ ), operators::Unique<T>{} };
  }




  template <
    typename Stream,
    typename T = typename Stream::type,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  stream<T,Limited<Stream>>
  operator>>
  (
    Stream&& stream_,
    operators::Take&& take
  )
    noexcept
  {
    return stream<T, Limited<Stream>>{ std::vector<T>{}, Limited<Stream>{ take.lim, std::forward<Stream>( stream_ ) } };
  }

  template <
    typename Stream,
    typename Pred,
    typename T = typename Stream::type,
    std::enable_if_t<
      detail::is_infinite_stream_v<Stream>,
      std::nullptr_t
    > = nullptr
  >
  inline
  stream<T,LimitedWhile<Stream,Pred>>
  operator>>
  (
    Stream&& stream_,
    operators::TakeWhile<Pred>&& take_while_
  )
    noexcept
  {
    return stream<T, LimitedWhile<Stream,Pred>>{ std::vector<T>{}, LimitedWhile<Stream,Pred>{ std::move(take_while_), std::forward<Stream>( stream_ ) } };
  }


} // ! namespace stream
} // ! namespace cranberries

#endif
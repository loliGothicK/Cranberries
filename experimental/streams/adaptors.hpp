#ifndef CRANBERRIES_STREAMS_ADAPTORS_HPP
#define CRANBERRIES_STREAMS_ADAPTORS_HPP
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include "forward.hpp"
#include "utility.hpp"
#include "InfiniteStream.hpp"
#include "StreamConverter.hpp"

namespace cranberries{
namespace streams{



  template <
    typename Stream,
    typename Operator,
    std::enable_if_t<
       detail::is_finite_stream_v<std::decay_t<Stream>>
    && detail::is_intermidiate_v<std::decay_t<Operator>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  decltype(auto)
  operator>>
  (
    Stream&& stream_,
    Operator&& op_
  )
    noexcept
  {
    return stream_.lazy( std::forward<Operator>( op_ ) );
  }

  template <
    typename Stream,
    typename Operator,
    std::enable_if_t<
       detail::is_finite_stream_v<std::decay_t<Stream>>
    && detail::is_terminate_v<std::decay_t<Operator>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  decltype(auto)
  operator>>
  (
    Stream&& stream_,
    Operator&& op_
  )
    noexcept
  {
    return op_(stream_.eval());
  }

  template <
    typename Stream,
    typename Operator,
    std::enable_if_t<
       detail::is_infinite_stream_v<std::decay_t<Stream>>
    && detail::is_stream_filter_v<std::decay_t<Operator>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamFilter<Stream,Operator>
  operator>>
  (
    Stream&& stream_,
    Operator&& op_
  )
    noexcept
  {
    return{ std::forward<Stream>(stream_), std::forward<Operator>( op_ ) };
  }

  template <
    typename Stream,
    typename Operator,
    std::enable_if_t<
       detail::is_infinite_stream_v<std::decay_t<Stream>>
    && detail::is_stream_operator_v<std::decay_t<Operator>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamOperator<Stream,Operator>
  operator>>
  (
    Stream&& stream_,
    Operator&& op_
  )
    noexcept
  {
    return{ std::forward<Stream>(stream_), std::forward<Operator>( op_ ) };
  }




  template <
    typename Stream,
    typename UnaryOp,
    typename E = typename std::decay_t<Stream>::element_type,
    typename R = std::result_of_t<UnaryOp(E)>,
    std::enable_if_t<
      std::is_same<E, R>::value,
      std::nullptr_t
    > = nullptr,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::MapProxy<UnaryOp>&& proxy
  )
    noexcept
  {
    return stream_.lazy(operators::Transform<UnaryOp>{std::move(proxy.f)});
  }

  template <
    typename Stream,
    typename UnaryOp,
    typename E = typename std::decay_t<Stream>::element_type,
    typename R = std::result_of_t<UnaryOp(E)>,
    std::enable_if_t<
      !std::is_same<E, R>::value,
      std::nullptr_t
    > = nullptr,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::MapProxy<UnaryOp>&& proxy
  )
    noexcept
  {
    return stream<R, operators::Map<Stream, UnaryOp>>{ std::vector<R>{}, operators::Map<Stream, UnaryOp>{ std::move(stream_), std::move(proxy.f) } };
  }

  template <
    typename Stream,
    typename To,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
  >
  inline
  auto
  operator >>
  (
    Stream&& stream_,
    operators::MapToProxy<To>
  )
    noexcept
  {
    return stream<To, operators::MapTo<Stream, To>>{ std::vector<To>{}, operators::MapTo<Stream, To>{ std::forward<Stream>(stream_) } };
  }

  template <
    typename Stream1, typename Stream2
  >
  inline
  auto
  operator &
  (
    Stream1&& left,
    Stream2&& right
  )
    noexcept
  {
    static_assert(
      std::is_same<typename std::decay_t<Stream1>::element_type, typename std::decay_t<Stream2>::element_type>::value,
      "Call 'concat' with different element_type."
    );

    return left.lazy(concat(std::forward<Stream2>(right)));
  }

  template <
    typename Stream
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::Repetition&& ev
  )  {
    for (size_t i{}; i < ev.times; ++i) stream_.eval();
    return std::forward<Stream>(stream_);
  }

  template <
    typename Stream,
    typename E = typename std::decay_t<Stream>::elemtnet_type
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::Run
  ) {
    return stream<E>{ stream_.eval().get() }; // TODO
  }




  template <
    typename Stream,
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::UniqueProxy&&
  )
    noexcept
  {
    return stream_.lazy( operators::Unique<T>{} );
  }

  template <
    typename Stream,
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>,
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
    return stream<T, Limited<Stream>>{ std::vector<T>{}, Limited<Stream>{ take.lim, std::move( stream_ ) } };
  }



  template <
    typename Stream,
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>,
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
    return{ std::forward<Stream1>(stream_), std::forward<Stream2>(merge.release()) };
  }

  template <
    typename Stream1, typename Stream2,
    std::enable_if_t<
      detail::is_finite_stream_v<Stream1> && detail::is_infinite_stream_v<Stream2>,
      std::nullptr_t
    > = nullptr
  >
  inline
  StreamConcatenator<Stream1&&, Stream2>
  operator >>
  (
    Stream1&& stream_,
    operators::Concatenate<Stream2>&& concat_
  )
    noexcept
  {
    return{ std::forward<Stream1>(stream_), std::move(concat_.release()) };
  }

  template <
    typename Stream,
    typename Pred,
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>,
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
    return stream<T, LimitedWhile<Stream,Pred>>{ std::vector<T>{}, LimitedWhile<Stream,Pred>{ std::forward<Stream>( stream_ ), std::move(take_while_) } };
  }

  template <
    typename Stream,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>,
      std::nullptr_t
    > = nullptr
  >
  inline
  decltype(auto)
  operator>>
  (
    Stream&& stream_,
    operators::Slice const& slice_
  )
    noexcept
  {
    return std::move( stream_ )
      >> dropped( slice_.low() - 1 )
      >> taken( slice_.up() - slice_.low() + 1 );
  }

  template <
    typename Stream,
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>,
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
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<
      detail::is_infinite_stream_v<std::decay_t<Stream>>,
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
    typename F,
    typename T = typename std::decay_t<Stream>::element_type,
    std::enable_if_t<
    detail::is_infinite_stream_v<std::decay_t<Stream>>,
    std::nullptr_t
    > = nullptr
  >
  inline
  StreamFlatTransformer<Stream,operators::FlatMap<F>>
  operator >>
  (
    Stream&& stream_,
    operators::FlatMap<F>&& fm_
  )
    noexcept
  {
    return{ std::forward<Stream>(stream_), std::move(fm_) };
  }


  template <
    typename Stream
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    detail::ConvertAny
  ) 
    noexcept
  {
    return detail::ImplicitStreamConvertInvoker::invoke(std::forward<Stream>(stream_.eval()));
  }

  // Explicit Convert
  template <
    typename Stream,
    template<class, class> class Target
  >
  inline
  constexpr
  decltype(auto)
  operator >>
  (
    Stream&& stream,
    detail::ConvertTo<Target>
  )
    noexcept
  {
    return detail::ExplicitStreamConverter<
      typename detail::ConvertTo<Target>::template type<
      typename std::decay_t<Stream>::element_type
      >
    >::convert(std::forward<Stream>(stream.eval()));
  }


}
}

#endif
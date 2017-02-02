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
    enabler_t<conjunction_v<
      cranberries_magic::is_finite_stream<Stream>,
      cranberries_magic::is_lazy<Operator>
    >> = nullptr
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
    enabler_t<conjunction_v<
      cranberries_magic::is_finite_stream<Stream>,
      cranberries_magic::is_eager<Operator>
    >> = nullptr
  >
  inline
  decltype(auto)
  operator>>
  (
    Stream&& stream_,
    Operator&& op_
  ) {
    return op_(stream_.eval());
  }

  template <
    typename Stream,
    typename Operator,
    enabler_t<conjunction_v<
      cranberries_magic::is_infinite_stream<Stream>,
      cranberries_magic::is_stream_filter<Operator>
    >> = nullptr
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
    enabler_t<conjunction_v<
      cranberries_magic::is_infinite_stream<Stream>,
      cranberries_magic::is_stream_operator<Operator>
    >> = nullptr
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
    typename E = element_type_of_t<Stream>,
    enabler_t<cranberries_magic::is_finite_stream_v<Stream>> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::FlatProxy&&
  )
    noexcept
  {
    return stream<element_type_of_t<E>>{}.lazy(operators::Flat<Stream>{ std::forward<Stream>( stream_ ) });
  }

  template <
    typename Stream,
    enabler_t< cranberries_magic::is_finite_stream_v<Stream> > = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::FlatAllProxy&&
  )
    noexcept
  {
    return stream<root_element_type_of_t<element_type_of_t<Stream>>>{}.lazy( operators::FlatAll<Stream>{ std::forward<Stream>( stream_ ) } ); // TODO
  }


  template <
    typename Stream,
    typename UnaryOp,
    typename E = element_type_of_t<Stream>,
    typename R = std::result_of_t<UnaryOp(E)>,
    enabler_t<conjunction_v<
      std::is_same<E, R>,
      cranberries_magic::is_finite_stream<Stream>
    >> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::TransformProxy<UnaryOp>&& proxy
  )
    noexcept
  {
    return stream_.lazy(operators::Endomorphism<UnaryOp>{std::move(proxy.f)});
  }

  template <
    typename Stream,
    typename UnaryOp,
    typename E = element_type_of_t<Stream>,
    typename R = std::result_of_t<UnaryOp(E)>,
    enabler_t<conjunction_v<
      negation<std::is_same<E, R>>,
      cranberries_magic::is_finite_stream<Stream>
    >> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    operators::TransformProxy<UnaryOp>&& proxy
  )
    noexcept
  {
    return stream<R, operators::Transform<Stream, UnaryOp>>{ std::vector<R>{}, operators::Transform<Stream, UnaryOp>{ std::move(stream_), std::move(proxy.f) } };
  }

  template <
    typename Stream,
    typename To,
    enabler_t<cranberries_magic::is_finite_stream_v<Stream>> = nullptr
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
      std::is_same<element_type_of_t<Stream1>, element_type_of_t<Stream2>>::value,
      "Call 'joined' with different element_type."
    );

    return left.lazy(joined(std::forward<Stream2>(right)));
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
    typename E = element_type_of_t<Stream>
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
    typename T = element_type_of_t<Stream>,
    enabler_t<cranberries_magic::is_finite_stream_v<Stream>> = nullptr
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
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
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
    typename Stream1, typename Stream2,
    enabler_t<disjunction_v<
      cranberries_magic::is_infinite_stream<Stream1>,
      cranberries_magic::is_infinite_stream<Stream2>
    >> = nullptr
  >
  inline
  StreamMerger<
    Stream1,
    decltype(std::declval<operators::Merge<Stream2>>().release())
  >
  operator >>
  (
    Stream1&& stream_,
    operators::Merge<Stream2>&& merge
  )
    noexcept
  {
    return{ std::forward<Stream1>(stream_), merge.release() };
  }

  template <
    typename Stream1, typename Stream2,
    enabler_t<conjunction_v<
      cranberries_magic::is_finite_stream<Stream1>,
      cranberries_magic::is_infinite_stream<Stream2>
    >> = nullptr
  >
  inline
  StreamConcatenator<Stream1&&, Stream2>
  operator >>
  (
    Stream1&& stream_,
    operators::Join<Stream2>&& concat_
  )
    noexcept
  {
    return{ std::forward<Stream1>(stream_), std::move(concat_.release()) };
  }

  template <
    typename Stream,
    typename Pred,
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
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
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
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
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
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
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
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
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
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
    typename Stream,
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
  >
  inline
  StreamFlatter<Stream>
  operator >>
  (
    Stream&& stream_,
    operators::FlatProxy&&
  )
    noexcept
  {
    return{ std::forward<Stream>( stream_ ) };
  }


  template <
    typename Stream,
    typename T = element_type_of_t<Stream>,
    enabler_t< cranberries_magic::is_infinite_stream_v<Stream> > = nullptr
  >
  inline
  StreamAllFlatter<Stream>
  operator >>
  (
      Stream&& stream_,
      operators::FlatAllProxy&&
  )
    noexcept
  {
    return{ std::forward<Stream>( stream_ ) };
  }

  template <
    typename Stream
  >
  inline
  decltype(auto)
  operator >>
  (
    Stream&& stream_,
    ConvertAny
  ) {
    return ImplicitStreamConvertInvoker::invoke(std::forward<Stream>(stream_.eval()));
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
    ConvertTo<Target>
  ) {
    return ExplicitStreamConverter<
      typename ConvertTo<Target>::template type<
      element_type_of_t<Stream>
      >
    >::convert(std::forward<Stream>(stream.eval()));
  }

  template <
    typename Stream,
    typename T = element_type_of_t<Stream>
  >
  inline
  operators::SummaryStat<T>
  operator >>
  (
    Stream&& stream_,
    operators::SummaryStatProxy&&
  ) {
    static_assert(
      std::is_arithmetic<T>::value,
      "error : element_type must be arithmetic_type!"
    );
    return{ std::move(stream_.eval().get()) };
  }



} // ! namespace streams
} // ! namespace cranberries

#endif
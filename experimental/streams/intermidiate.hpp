#ifndef CRANBERRIES_STREAMS_INTERMIDIATE_HPP
#define CRANBERRIES_STREAMS_INTERMIDIATE_HPP
#include <utility>
#include <type_traits>
#include <string>
#include <vector>
#include "forward.hpp"
#include "detail/tag.hpp"

namespace cranberries {
namespace streams {

	template <
		typename STREAM, typename Pred,
		typename E = typename STREAM::element_type,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Filter<Pred>&& filter
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype(std::declval<Pred>()(std::declval<E>()))
			>::value,
			"Can not call Pred with an arg of stream::elemetnt_type or return type is not bool."
		);

		return stream_.lazy(filter);
	}

	template <
		typename STREAM, typename Pred,
		typename E = typename STREAM::element_type,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Sort<Pred>&& so
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype(std::declval<Pred>()(std::declval<E>(), std::declval<E>()))
			>::value,
			"Can not call Pred with 2 args of stream::elemetnt_type or return type is not bool."
		);

		return stream_.lazy(so);
	}

	template <
		typename STREAM,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Sort<>&& so
	)
		noexcept
	{
		return stream_.lazy(so);
	}

  template <
    typename STREAM,
    typename E = typename std::decay_t<STREAM>::element_type,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::DistinctProxy&&
	)
		noexcept
	{
    return stream_.lazy( operators::Distinct<E>{} );
	}

	template <
		typename STREAM,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Drop&& sk
	)
		noexcept
	{
		return stream_.lazy(sk);
	}

	template <
		typename STREAM,
    std::enable_if_t<detail::is_finite_stream_v<STREAM>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Take&& lim
	)
		noexcept
	{
		return stream_.lazy(lim);
	}

	template <
		typename STREAM1,
		typename STREAM2
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM1&& stream_,
		operators::Concatenate<STREAM2>&& concat
	)
		noexcept
	{

		static_assert(
			std::is_same<typename STREAM1::element_type, typename STREAM2::element_type>::value,
			"Call 'concat' with different element_type."
		);

		return std::forward<STREAM1>(stream_.lazy(std::move(concat)));
	}

	template <
		typename STREAM,
		typename T,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::PushBack<T>&& pb
	)
		noexcept
	{
		static_assert(
			std::is_constructible<typename STREAM::element_type, T>::value,
			"Can not construct element_type from T."
		);

		return stream_.lazy(pb);
	}

	template <
		typename STREAM,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::PopBack&& pb
	)
		noexcept
	{
		return stream_.lazy(pb);
	}

	template <
		typename STREAM,
		typename UnaryOp,
		typename E = typename STREAM::element_type,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Peek<UnaryOp>&& f
	)
		noexcept
	{
		static_assert(
			std::is_same<
				void,
				std::result_of_t<UnaryOp(E)>
			>::value,
			"Can not call UnaryOp with an arg of STREAM::element_type."
		);

		return stream_.lazy(f);
	}

  template <
    typename STREAM,
    typename UnaryOp,
    std::enable_if_t<detail::is_finite_stream_v<STREAM>,std::nullptr_t> = nullptr
  >
  inline
  decltype(auto)
  operator >>
  (
    STREAM&& stream_,
    operators::Transform<UnaryOp>&& transform
  )
    noexcept
  {
    return stream_.lazy( transform );
  }


	template <
		typename STREAM,
		typename UnaryOp,
		typename E = typename STREAM::element_type,
		typename R = std::result_of_t<UnaryOp(E)>,
		std::enable_if_t<
			std::is_same<E, R>::value,
			std::nullptr_t
		> = nullptr,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::MapProxy<UnaryOp>&& proxy
	)
		noexcept
	{
		return stream_.lazy(operators::Transform<UnaryOp>{std::move(proxy.f)});
	}

	template <
		typename STREAM,
		typename UnaryOp,
		typename E = typename STREAM::element_type,
		typename R = std::result_of_t<UnaryOp(E)>,
		std::enable_if_t<
			!std::is_same<E, R>::value,
			std::nullptr_t
		> = nullptr,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::MapProxy<UnaryOp>&& proxy
	)
		noexcept
	{
		return stream<R, operators::Map<STREAM, UnaryOp>>{ std::vector<R>{}, operators::Map<STREAM, UnaryOp>{ std::move(stream_), std::move(proxy.f) } };
	}

	template <
		typename STREAM,
		typename To,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	auto
	operator >>
	(
		STREAM&& stream_,
		operators::MapToProxy<To>
	)
		noexcept
	{
		return stream<To, operators::MapTo<STREAM, To>>{ std::vector<To>{}, operators::MapTo<STREAM, To>{ std::forward<STREAM>(stream_) } };
	}

	template <
		typename STREAM,
		typename BinaryOp,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::AdjacentForEach<BinaryOp> f
	)
		noexcept
	{
		static_assert(
			is_callable_v<BinaryOp,typename STREAM::element_type,typename STREAM::element_type>,
			"Can not call BinaryOp with 2 args of STREAM::element_type."
		);

		return stream_.lazy(f);
	}

	template <
		typename STREAM,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>, std::nullptr_t> = nullptr
  >
	inline
	operators::Concatenate<STREAM>
	concat(
		STREAM&& stream_
	)
		noexcept
	{
		return{ std::forward<STREAM>(stream_) };
	}

	template <
		typename STREAM1, typename STREAM2
	>
	inline
	auto
	operator &
	(
		STREAM1&& left,
		STREAM2&& right
	)
		noexcept
	{
		static_assert(
			std::is_same<typename STREAM1::element_type, typename STREAM2::element_type>::value,
			"Call 'concat' with different element_type."
		);

		return left.lazy(concat(std::forward<STREAM2>(right)));
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Repetition&& ev
	)	{
		for (size_t i{}; i < ev.times; ++i) stream_.eval();
		return std::forward<STREAM>(stream_);
	}

	template <
		typename STREAM,
		typename E = typename STREAM::elemtnet_type
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Run
	) {
		return stream<E>{ stream_.eval().get() }; // TODO
	}

	template <
		typename STREAM,
		typename Pred,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::DropWhile<Pred>&& dw
	)
		noexcept
	{
		return stream_.lazy(dw);
	}

	template <
		typename STREAM,
		typename Func,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::Iinvoker<Func>&& f
	)
		noexcept
	{
		static_assert(
			std::is_same<
				STREAM,
				decltype(std::declval<Func>()(std::declval<STREAM>()))
			>::value,
			"Can not call Func with an arg of type STREAM	or return type is not STREAM."
		);
		return stream_.lazy(f);
	}

  template <
		typename STREAM,
		typename Pred,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::TakeWhile<Pred>&& tw
	)
		noexcept
	{
		return stream_.lazy(tw);
	}

  template <
		typename STREAM,
    typename T = typename std::decay_t<STREAM>::element_type,
    std::enable_if_t<detail::is_finite_stream_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		operators::UniqueProxy&&
	)
		noexcept
	{
    return stream_.lazy( operators::Unique<T>{} );
	}

  template <
		typename Stream1,
    typename Stream2,
    std::enable_if_t<
      detail::is_finite_stream_v<std::decay_t<Stream1>>
      && detail::is_finite_stream_v<std::decay_t<Stream2>>,
      std::nullptr_t
    > = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		Stream1&& stream_,
		operators::Merge<Stream2>&& merge_
	)
		noexcept
	{
    return stream_.lazy( std::move(merge_) );
	}

} // ! namespace stream
} // ! namespace cranberries

#endif
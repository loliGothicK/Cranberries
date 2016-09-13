#ifndef CRANBERRIES_STREAMS_TERMINATION_HPP
#define CRANBERRIES_STREAMS_TERMINATION_HPP
#include <utility>
#include <type_traits>
#include "forward.hpp"
#include "detail/tag.hpp"
#include "StreamConverter.hpp"

namespace cranberries {
namespace streams {

	template <
		typename STREAM, typename UnaryOp,
		typename E = typename STREAM::element_type,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	void
	operator >>
	(
		STREAM&& stream_,
		operators::ForEach<UnaryOp>&& f
	)
		noexcept
	{
		static_assert(
			std::is_same<
				void,
				decltype(std::declval<UnaryOp>()(std::declval<E>()))
			>::value,
			"Can not call UnaryOp with an arg of STREAM::element_type."
		);

		f(stream_.eval());
		return;
	}

	template <
		typename STREAM,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	void
	operator >>
	(
		STREAM&& stream_,
		operators::Printer&& p
	)
		noexcept
	{
		p(stream_.eval());
		return;
	}

	template <
		typename STREAM,
		typename Pred
	>
	inline
	bool
	operator >>
	(
		STREAM&& stream_,
		operators::All<Pred> all
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype(std::declval<Pred>()(std::declval<typename STREAM::element_type>()))
			>::value,
			"Can not call Pred with an arg of type STREAM::element_type."
		);

		return all(stream_.eval());
	}

	template <
		typename STREAM,
		typename Pred
	>
	inline
	bool
	operator >>
	(
		STREAM&& stream_,
		operators::Any<Pred> any
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype(std::declval<Pred>()(std::declval<typename STREAM::element_type>()))
			>::value,
			"Can not call Pred with an arg of type STREAM::element_type."
		);

		any(return stream_.eval());
	}

	template <
		typename STREAM,
		typename Pred
	>
	inline
	bool
	operator >>
	(
		STREAM&& stream_,
		operators::None<Pred> none
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype(std::declval<Pred>()(std::declval<typename STREAM::element_type>()))
			>::value,
			"Can not call Pred with an arg of type STREAM::element_type."
		);

		return none(stream_.eval());
	}


	template <
		typename STREAM
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream_,
		detail::ConvertAny
	) 
		noexcept
	{
		return detail::ImplicitStreamConvertInvoker::invoke(std::forward<STREAM>(stream_.eval()));
	}

	template <
		typename STREAM, typename F,
		typename R = decltype(
			std::declval<F>()(std::declval<STREAM>())
			)
	>
	inline
	R
	operator >>
	(
		STREAM&& stream_,
		operators::Tinvoker<F>&& f
	) {
		return f(stream_.eval());
	}

} // ! namespace stream
} // ! namespace cranberries

#endif
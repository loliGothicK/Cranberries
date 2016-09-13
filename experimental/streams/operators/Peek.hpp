#ifndef CRANBERRIES_STREAMS_OPERATORS_PEEK_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PEEK_HPP
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	template <
		typename UnaryFunc
	>
	struct Peek
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM,
			std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream) f(e);
			return std::forward<STREAM>(stream);
		}

		template <
			typename T,
			std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
		>
		decltype(auto)
		operator()( T&& a ) {
			return f( a ), a;
		}

		// mapper

		UnaryFunc f;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
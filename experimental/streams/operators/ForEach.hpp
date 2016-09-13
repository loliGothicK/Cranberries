#ifndef CRANBERRIES_STREAMS_OPERATORS_FOREACH_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FOREACH_HPP
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	template <
		typename UnaryFunc
	>
	struct ForEach
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM
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

		// mapper

		UnaryFunc f;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
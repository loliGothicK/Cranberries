#ifndef CRANBERRIES_STREAMS_OPERATORS_PUSHBACK_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PUSHBACK_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	template <
		typename T
	>
	struct PushBack
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
			stream.push_back(v);
			return std::forward<STREAM>(stream);
		}

		// member

		T v;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
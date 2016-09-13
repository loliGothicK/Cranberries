#ifndef CRANBERRIES_STREAMS_OPERATORS_POPBACK_HPP
#define CRANBERRIES_STREAMS_OPERATORS_POPBACK_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	struct PopBack
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
			stream.pop_back();
			return std::forward<STREAM>(stream);
		}
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
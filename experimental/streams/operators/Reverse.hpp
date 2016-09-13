#ifndef CRANBERRIES_STREAMS_OPERATORS_REVERSE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_REVERSE_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	struct Reverse
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
			std::reverse(stream.begin(), stream.end());
			return std::forward<STREAM>(stream);
		}
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
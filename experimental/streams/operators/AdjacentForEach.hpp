#ifndef CRANBERRIES_STREAMS_OPERATORS_ADJACENTFOREACH_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ADJACENTFOREACH_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	template <
	typename BinaryFunc
	>
	struct AdjacentForEach
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
		// for ADL
		using std::begin;
		using std::end;

		auto&& first = begin(stream);
		auto&& last = end(stream);

		while (std::next(first) != last)
		{
			auto&& a = *first;
			++first;
			auto&& b = *first;
			f(a, b);
		}
		return std::forward<STREAM>(stream);
	}

	// member

	BinaryFunc f;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
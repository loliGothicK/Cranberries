#ifndef CRANBERRIES_STREAMS_OPERATORS_SORT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SORT_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	template <
		typename Pred
	>
	struct Sort
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
			std::sort(stream.begin(), stream.end(), pred);
			return std::forward<STREAM>(stream);
		}

		// member

		Pred pred;
	};

	// Intermidiate Operation
	template < >
	struct Sort <
		detail::defaulted // lookup operator < using ADL.
	>
	{
		using tree_tag = detail::not_tree;
		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()(
			STREAM&& stream
		) {
			std::sort(stream.begin(), stream.end());
			return std::forward<STREAM>(stream);
		}
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
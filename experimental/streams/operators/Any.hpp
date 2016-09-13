#ifndef CRANBERRIES_STREAMS_OPERATORS_ANY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ANY_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	template < typename Pred >
	struct Any
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator ()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)
				if (pred(e)) return true;
			return false;
		}

		// member

		Pred pred;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
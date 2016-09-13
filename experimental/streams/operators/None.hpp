#ifndef CRANBERRIES_STREAMS_OPERATORS_NONE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_NONE_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {
	
	template < typename Pred >
	struct None
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
				if (pred(e)) return false;
			return true;
		}

		// member

		Pred pred;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
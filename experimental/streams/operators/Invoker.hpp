#ifndef CRANBERRIES_STREAMS_OPERATORS_INVOKER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_INVOKER_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Adaptor for user defined operation and Stream operation interface.
	// Concept : Operator must call with arg of any Stream and return it.
	template <
		typename Operator // user defined operation
	>
	struct Tinvoker
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
			op(stream);
			return std::forward<STREAM>(stream);
		}

		// member

		Operator op;
	};

  template <
		typename Operator // user defined operation
	>
	struct Iinvoker
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
			op(stream);
			return std::forward<STREAM>(stream);
		}

		// member

		Operator op;
	};



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
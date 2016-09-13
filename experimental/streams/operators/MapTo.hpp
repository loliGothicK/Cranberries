#ifndef CRANBERRIES_STREAMS_OPERATORS_MAPTO_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MAPTO_HPP
#include <string>
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	struct ToString {};

	template < typename To >
	struct MapToProxy {};


	template <
		typename FromStream,
		typename To
	>
	struct MapTo
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
			from.eval();
			stream.get() = std::vector<To>{ from.begin(), from.end() };
			return std::forward<STREAM>(stream);
		}

		FromStream from;
	};

	template <
		typename FromStream
	>
	struct MapTo<
		FromStream,
		std::string
	>
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
			using std::to_string; // For ADL
			from.eval();
			for (auto&& e : from) stream.emplace_back(to_string(e));
			return std::forward<STREAM>(stream);
		}

		FromStream from;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
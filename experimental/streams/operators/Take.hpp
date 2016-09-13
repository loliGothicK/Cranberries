#ifndef CRANBERRIES_STREAMS_OPERATORS_LIMIT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_LIMIT_HPP
#include <utility>
#include "..\detail\tag.hpp"
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	struct Take
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM,
			typename Range = typename std::decay_t<STREAM>::range_type,
      std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			Range{ source.begin(), source.begin() + lim }.swap(source); // swap trick
			return std::forward<STREAM>(stream);
		}

		// member

		std::size_t lim;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
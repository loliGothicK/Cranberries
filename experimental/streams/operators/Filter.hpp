#ifndef CRANBERRIES_STREAMS_OPERATORS_FILTER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_FILTER_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	template <
		typename Pred
	>
	struct Filter
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM,
      std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			for (auto&& iter = source.begin(); iter != source.end(); ) {
				if (pred(*iter)) iter = source.erase(iter);
				else ++iter;
			}
			stream.shrink_to_fit();
			return std::forward<STREAM>(stream);
		}

    template <
      typename T,
      std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
    bool operator()( T&& a ) noexcept {
      return pred( a );
    }

		// member

		Pred pred;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
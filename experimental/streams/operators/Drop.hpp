#ifndef CRANBERRIES_STREAMS_OPERATORS_SKIP_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SKIP_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	// Intermidiate Operation
	struct Drop
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM,
      typename E = typename std::decay_t<STREAM>::element_type,
      std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
		>
		inline
		decltype(auto)
		operator()(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			Range{ source.begin() + n, source.end() }.swap(source); // swap trick
			return std::forward<STREAM>(stream);
		}

    template <
      typename T,
      std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
    bool operator()(T&&) noexcept
    {
      static size_t count = 0;
      if ( count < n ) {
        ++count;
        return true;
      }
      return false;
    }

		// member

		std::size_t n;
	};



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
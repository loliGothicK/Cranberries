#ifndef CRANBERRIES_STREAMS_OPERATORS_UNIQUE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_UNIQUE_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	struct UniqueProxy{};

	// Intermidiate Operation
	template < typename T >
	struct Unique
	{
		using tree_tag = detail::not_tree;

		Unique() = default;

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
			source.erase( std::unique( source.begin(), source.end() ), source.end() );
			return std::forward<STREAM>(stream);
		}

		template <
			typename T,
			std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
		>
		bool operator()(T&& arg) noexcept
		{
      once( arg );
			if ( arg == prev ) {
				return true;
			}
			prev = arg;
			return false;
		}

    void once( T const& a ) {
      prev = a + 1;
      once_flag = false;
    }

		T prev;
		bool once_flag = true;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
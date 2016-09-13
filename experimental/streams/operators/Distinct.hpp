#ifndef CRANBERRIES_STREAMS_OPERATORS_DISTINCT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_DISTINCT_HPP
#include <utility>
#include <unordered_map>
#include <type_traits>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	struct DistinctProxy{};

	// Intermidiate Operation
	template < typename T >
	struct Distinct
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
			STREAM&& stream_
		) {
			auto&& source = stream_.get();
			for ( auto iter = source.begin(); iter != source.end();)
			{
				if ( !is[*iter] )
				{
					is[*iter] = true;
					iter = source.erase(iter);
				}
				else{
					++iter;
				}
			}
			source.shorink_to_fit();
			return std::forward<STREAM>(stream_);
		}

		template <
			typename T,
			std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
		>
		bool operator()(T const& arg) noexcept
		{
			std::unordered_map<T, bool> is;
			if ( !is[arg] ) {
				is[arg] = true;
				return false;
			}
			return true;
		}

	 std::unordered_map<T, bool> is;
	};


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
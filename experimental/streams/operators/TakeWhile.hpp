#ifndef CRANBERRIES_STREAMS_OPERATORS_TAKEWHILE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_TAKEWHILE_HPP
#include <string>
#include <regex>
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	template <
		typename Pred
	>
	struct
	TakeWhile
	{
		using tree_tag = detail::not_tree;

    TakeWhile( Pred pred ) : pred_{ std::forward<Pred>(pred) } {}

		template <
			typename STREAM,
			typename E = typename std::decay_t<STREAM>::element_type,
			std::enable_if_t<detail::is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
		>
		decltype(auto)
		operator()
		(
			STREAM&& stream_
		)
			noexcept
		{
			auto&& resorce = stream_.get();
			auto&& iter = resorce.begin();
			// peel loop
			for ( ; iter != resorce.end() &&  pred_( *iter ); ++iter );
			Range{ resorce.begin(),iter }.swap( resorce );
			return std::forward<STREAM>( stream_ );
		}

		template <
			typename T,
			std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
		>
		bool operator()(T&& arg) noexcept
		{
			if ( flag && pred_(arg) ) {
				return true;
			}
			else if ( !flag ) {
				return false;
			}
			else {
				flag = false;
				return false;
			}
			return false;
		}

		Pred pred_;
		bool flag = true;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
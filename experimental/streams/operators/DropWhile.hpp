#ifndef CRANBERRIES_STREAMS_OPERATORS_DROPWHILE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_DROPWHILE_HPP
#include <type_traits>
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	template <
		typename Pred
	>
	struct DropWhile
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
			using Range = typename std::decay_t<STREAM>::range_type;
			// peel loop
			auto&& source = stream_.get();
			auto iter = source.begin();
			for (; iter != source.end() && pred(*iter); ++iter);
			Range{ iter,source.end() }.swap(source);
			return std::forward<STREAM>(stream_);
		}

		template <
      typename T,
      std::enable_if_t<!detail::is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
		bool operator()(T&& a) const noexcept {
			static bool flag = true;
			if (flag)	return flag = pred(a);
			return false;
		}


		Pred pred;
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
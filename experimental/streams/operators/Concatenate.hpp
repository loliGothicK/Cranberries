#ifndef CRANBERRIES_STREAMS_OPERATORS_CONCATENATE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_CONCATENATE_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {
	
	template <
		typename Branch
	>
	struct Concatenate
	{
		using tree_tag = detail::not_tree;

		// for evaluate Branch Stream only once
		void
		once()
		{
			static bool flag = true;
			if (flag) {
				branch.eval();
				flag = false;
			}
		}

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			once(); // perfectly call once
			auto&& lv = stream.get();
			auto&& rv = branch.get();
			if (lv.capacity() < lv.size() + rv.size())
				lv.reserve(lv.size() + rv.size());
			lv.insert(lv.end(), rv.begin(), rv.end());
			return std::forward<STREAM>(stream);
		}

		// member

		Branch branch; // Branch Stream
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
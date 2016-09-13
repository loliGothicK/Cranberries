#ifndef CRANBERRIES_STREAMS_OPERATORS_PRINTER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_PRINTER_HPP
#include <string>
#include <iostream>
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	struct Printer
	{
		using tree_tag = detail::not_tree;

		template <
			typename STREAM
		>
		inline
		void
		operator()
		(
			STREAM&& stream_
		)
			const noexcept
		{
			auto&& iter = stream_.begin();
			os << *iter;
			++iter;
			for (; iter != stream_.end(); ++iter)
				os << delim << *iter;
			os << std::endl;
		}

		// members

		std::ostream& os; // defalut is std::cout
		std::string delim; // defalut is ", "
	};

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
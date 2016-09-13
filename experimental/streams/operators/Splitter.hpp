#ifndef CRANBERRIES_STREAMS_OPERATORS_SPLITTER_HPP
#define CRANBERRIES_STREAMS_OPERATORS_SPLITTER_HPP
#include <string>
#include <regex>
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {

	struct Splitter
	{
		Splitter(std::string s) : r_{ s } {}

		// member

		std::regex r_{};
	};



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
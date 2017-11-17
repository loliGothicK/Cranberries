#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

class WriteLine
	: private tag::adaptor_tag
	, private tag::action_tag
{
	std::ostream& os;
public:
	WriteLine() : os(std::cout) {}
	WriteLine(std::ostream& os) : os(os) {}

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& range) {
		auto iter = std::begin(range);
		os << *iter; ++iter;
		for (; iter != std::end(range); ++iter)
			os << " " << *iter;
		os << std::endl;
		return std::forward<SentinelRange>(range);
	}
};

namespace operation {
	WriteLine write_line(std::ostream& os = std::cout)
	{ return {os}; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

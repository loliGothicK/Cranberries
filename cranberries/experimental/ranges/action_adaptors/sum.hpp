#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_SUM_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_SUM_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

class Sum
	: private tag::adaptor_tag
	, private tag::action_tag
{
public:
	Sum() = default;

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& range) {
		using std::begin;
		using std::end;
		auto iter = begin(range);
		auto last = end(range);
		auto ret = *iter;
		while (++iter != last)
		{
			ret += *iter;
		}
		return ret;
	}
};


namespace action {
	Sum sum() noexcept { return {}; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

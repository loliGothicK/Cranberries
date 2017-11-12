#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_ANY_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_ANY_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Pred >
class Any
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Pred pred;
public:
	template < class Pred_ >
	Any(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

	template < class SentinelRange >
	decltype(auto)
	operator()(SentinelRange&& range) {
		for (const auto& elem : range) {
			if (pred(elem)) return true;
		}
		return false;
	}
};

namespace action {
	template < class Pred >
	Any<Pred> any(Pred&& pred) { return { std::forward<Pred>(pred) }; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

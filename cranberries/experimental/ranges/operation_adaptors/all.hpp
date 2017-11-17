#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_ALL_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_ALL_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Pred >
class All
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Pred pred;
public:
	template < class Pred_ >
	All(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

	template < class SentinelRange >
	decltype(auto)
	operator()(SentinelRange&& range) {
		for (const auto& elem : range) {
			if (!pred(elem)) return false;
		}
		return true;
	}
};

namespace action {
	template < class Pred >
	All<Pred> all(Pred&& pred) { return { std::forward<Pred>(pred) }; }

	template < class Pred >
	auto none(Pred&& pred) { return all([pred = pred](auto&& e){ return !pred(std::forward<decltype(e)>(e)); }); }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

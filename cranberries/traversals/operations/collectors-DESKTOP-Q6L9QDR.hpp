#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_PERTITION_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_PERTITION_HPP
#pragma once
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include "../../common/concepts.hpp"
#include "../../functional.hpp"
#include <unordered_map>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class GroupType, class ValueType >
class collector {
	std::unordered_map<GroupType, std::vector<ValueType>> collection;
public:
	template < class Traversable, class Pred >
	collector(Traversable&& trv, Pred&& pred) : collection{}
	{
		for (auto const& item : trv) collection[pred(item)].emplace_back(item);
	}

	decltype(auto) operator[](GroupType g) const {
		return collection.at(g);
	}
};


class PertitioningBy
	: private tag::sentinel_traversal_tag
{
public:
	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;

	PertitioningBy() = default;

	template < class Traversable, class Pred,
		std::enable_if_t<is_callable_v<Pred(typename std::decay_t<Traversable>::value_type)>, std::nullptr_t> = nullptr >
	decltype(auto) operator()(Traversable&& trv, Pred&& pred) {
		return collector<
			decltype(pred(std::declval<decltype(*(::cranberries::back_magic::begin(trv)))>())),
			decltype(*(::cranberries::back_magic::begin(trv)))
		>{ std::forward<Traversable>( trv ), std::forward<Pred>( pred ) };
	}


private:
	Pred pred;
};



namespace view {
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

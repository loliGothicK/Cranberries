#ifndef CRANBERRIES_OPERATIONS_MATCH_HPP
#define CRANBERRIES_OPERATIONS_MATCH_HPP
#pragma once

#include "../detail/tags.hpp"
#include "../../common/concepts.hpp"
#include <utility>
#include <regex>

namespace cranberries {
inline namespace experimental {
namespace traversals {

enum class match_mode {
	all, any, none, regex,
};

template < class Pred, match_mode = match_mode::all >
class Match
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Pred pred;
public:
	template < class Pred_ >
	Match(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;

	template < class SentinelRange >
	decltype(auto)
	operator()(SentinelRange&& traversal) {
		for (const auto& elem : traversal) {
			if (!pred(elem)) return false;
		}
		return true;
	}
};

template < class Pred >
class Match<Pred, match_mode::none>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Pred pred;
public:
	template < class Pred_ >
	Match(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& traversal) {
		for (const auto& elem : traversal) {
			if (pred(elem)) return true;
		}
		return false;
	}
};

template < class Pred >
class Match<Pred, match_mode::any>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Pred pred;
public:
	template < class Pred_ >
	Match(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;


	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& traversal) {
		for (const auto& elem : traversal) {
			if (pred(elem)) return true;
		}
		return false;
	}
};
template < >
class Match<void, match_mode::regex>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	std::string regexp;
public:
	Match(std::string regexp) : regexp{ regexp } {}

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<conjunction_v<
			concepts::required<Range, concepts::iterable>,
			is_specialize_of<std::basic_string, ElemType>
		>>;

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& traversal) {
		std::regex regexp_(regexp);
		for (const auto& elem : traversal) {
			if (std::regex_match(elem, regexp_)) return true;
		}
		return false;
	}
};


namespace operation {
	template < class Pred >
	Match<Pred, match_mode::all> all_match(Pred&& pred) { return { std::forward<Pred>(pred) }; }

	template < class Pred >
	Match<Pred, match_mode::none> none_match(Pred&& pred) { return { std::forward<Pred>(pred) }; }

	template < class Pred >
	Match<Pred, match_mode::any> any_match(Pred&& pred) { return { std::forward<Pred>(pred) }; }

	inline Match<void, match_mode::regex> regex_match(std::string regexp) { return { regexp }; }
}



}}}


#endif
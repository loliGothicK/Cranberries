#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_FIND_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_FIND_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include <type_traits>
#include <iostream>
#include "../../optional.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class T >
class Find
	: private tag::adaptor_tag
	, private tag::action_tag
{
	T value;
public:
	template < class T_ >
	Find(T_&& value) : value{ std::forward<T_>(value) } {}

	template < class SentinelRange >
	optional<typename std::decay_t<SentinelRange>::value_type>
		operator()(SentinelRange&& traversal) {
		for (auto const& e : traversal) {
			if (e == value) return some(e);
		}
		return nullopt;
	}
};

template < class Pred >
class FindIf
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Pred pred;
public:
	template < class Pred_ >
	FindIf(Pred_&& pred) : pred{ std::forward<Pred_>(pred) } {}

	template < class SentinelRange >
	optional<typename std::decay_t<SentinelRange>::value_type>
		operator()(SentinelRange&& traversal) {
		for (auto const& e : traversal) {
			if (pred(e)) return some(e);
		}
		return nullopt;
	}
};


namespace collect {
	template < class T >
	Find<T> find(T&& value)
	{
		return { std::forward<T>(value) };
	}

	template < class Pred >
	Find<Pred> find_if(Pred&& pred)
	{
		return { std::forward<Pred>(pred) };
	}
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_DECONSTRUCT_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_DECONSTRUCT_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>
#include <tuple>

namespace cranberries {
namespace experimental {
namespace ranges {

template < size_t N >
class Deconstruct
	: private tag::adaptor_tag
	, private tag::action_tag
{
	template < size_t, class T >
	using Ex = T;

	template < class Iter, size_t... I >
	std::tuple<typename std::decay_t<Iter>::value_type, Ex<I,typename std::decay_t<Iter>::value_type>...>
		invoke(Iter&& iter, std::index_sequence<I...>)
	{
		auto forward = [](auto& it,auto) { return ++it, *it; };
		return {*iter, forward(iter,I)...};
	}

public:
	Deconstruct() = default;

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& range) {
		using std::begin;
		return invoke(begin(range), std::make_index_sequence<N-1>{});
	}
};

namespace action {
	template < size_t N >
	Deconstruct<N> deconstruct()
		{ return {}; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

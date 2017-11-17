#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_CONVERTER_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_CONVERTER_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

template < template <class T, class A = std::allocator<T>> class Container >
class Converter
	: private tag::adaptor_tag
	, private tag::action_tag
{
public:
	Converter() = default;

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& range) {
		Container<typename std::decay_t<SentinelRange>::value_type> ret{};
		for (auto const& e: range) ret.emplace_back(e);
		return ret;
	}
};



namespace operation {
	template < template < class, class > class Target >
	Converter<Target> convert() noexcept { return {}; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

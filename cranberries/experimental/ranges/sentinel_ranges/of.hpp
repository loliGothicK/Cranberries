#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_OF_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_OF_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include <fstream>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Of
	: private tag::sentinel_range_tag
{
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = magic_arts::default_forward_sentinel<typename std::decay_t<Range>::value_type>;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<typename std::decay_t<Range>::value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Of(const Range& range) : range( range )  {}

	iterator begin() { return { std::make_unique<sentinel>(range) }; }
	iterator end() { return {}; }
	reverse_iterator rbegin() { return { std::make_unique<reverse_sentinel>(range) }; }
	reverse_iterator rend() { return {}; }

private:
	Range range;
};


namespace create {
	template < class Range >
	Of<Range> of(Range&& range) { return { std::forward<Range>(range) }; }

	template < class T >
	Of<std::initializer_list<T>> of(std::initializer_list<T> il) { return { il }; }
}

}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP

#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_OF_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_OF_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include <fstream>
#include <iostream>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Of
	: private tag::sentinel_traversal_tag
{
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = magic_arts::default_forward_sentinel<typename std::decay_t<Range>::value_type>;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<typename std::decay_t<Range>::value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Of(const Range& traversal) : traversal( traversal )  {}

	iterator begin() { return { std::make_unique<sentinel>(traversal) }; }
	iterator end() { return {}; }
	reverse_iterator rbegin() { return { std::make_unique<reverse_sentinel>(traversal) }; }
	reverse_iterator rend() { return {}; }

private:
	Range traversal;
};


namespace create {
	template < class Range >
	Of<Range> of(Range&& traversal) { return { std::forward<Range>(traversal) }; }

	template < class T >
	Of<std::initializer_list<T>> of(std::initializer_list<T> il) { return { il }; }
}

}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP

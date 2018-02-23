#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Take
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
		Range traversal_;
		long long int n_;
		typename std::decay_t<Range>::iterator iter;
	public:
		using value_type = typename std::decay_t<Range>::value_type;

		sentinel_impl(Range traversal, long long int n)
			: traversal_{ traversal }
			, n_(n)
			, iter{ cranberries::back_magic::begin(traversal_) }
		{}

		auto get() { return *iter; }
		bool next() { return ++iter, --n_ < 0; }
		bool is_end() { return n_ <= 0 || iter == cranberries::back_magic::end(traversal_); }
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Take(Range traversal, size_t n)
		: traversal{ traversal }
		, n(n) {}

	iterator begin() const { return { std::make_unique<sentinel>(traversal, n) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	long long int n;
};



namespace view {
	auto take(size_t n) { return make_proxy<Take>::forward(n); }
}

}}}
#endif
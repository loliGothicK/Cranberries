#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_SHUFFLE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_SHUFFLE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <algorithm>
#include "../../randomize.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Shuffle
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		sentinel_impl(Range traversal)
			: traversal_{}
			, iter{}
			, last{}
		{
			using std::begin; using std::end;
			for (auto const& e : traversal) traversal_.emplace_back(e);
			std::shuffle(begin(traversal_), end(traversal_), gen_mt_64(std::random_device{}));
			iter = begin(traversal_);
			last = end(traversal_);
		}

		auto get() { return *iter; }
		void next() { ++iter; }
		bool is_end() { return iter == last; }
	private:
		std::vector<value_type> traversal_;
		typename std::vector<value_type>::iterator iter;
		typename std::vector<value_type>::iterator last;
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < class Range_ >
	Shuffle(Range_&& traversal)
		: traversal{ std::forward<Range_>(traversal) }
	{}

	iterator begin() const { return { std::make_unique<sentinel>(traversal) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
};

namespace view {
	auto shuffle() { return make_proxy<Shuffle>::forward(); }
}

}}}
#endif
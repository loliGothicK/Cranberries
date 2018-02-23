#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_REVERSE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_REVERSE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../../detection_toolkit.hpp"
#include "../../common/concepts.hpp"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Reverse
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		forward_sentinel(Range traversal)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::rbegin(traversal_) }
			, last{ cranberries::back_magic::rend(traversal_) }
		{}

		auto get() { return *iter; }
		void next() { ++iter; }
		bool is_end() { return iter == last; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::reverse_iterator iter;
		typename std::decay_t<Range>::reverse_iterator last;
	};

	class reverse_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		reverse_sentinel(Range traversal)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::begin(traversal_) }
			, last{ cranberries::back_magic::end(traversal_) }
		{}

		auto get() { return *iter; }
		void next() { ++iter; }
		bool is_end() { return iter == last; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::iterator iter;
		typename std::decay_t<Range>::iterator last;
	};

public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = reverse_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;
	using value_type = typename std::decay_t<Range>::value_type;

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using constraints
		= std::enable_if_t<conjunction_v<
		concepts::required<Range, concepts::reverse_iterable>,
		concepts::required<ElemType, concepts::regular_type>
		>>;


	Reverse(Range traversal)
		: traversal{ traversal } {}

	iterator begin() const { return { std::make_unique<forward_sentinel>(traversal) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(traversal) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
};

namespace view {
	auto reverse() { return make_proxy<Reverse>::forward(); }
}


}}}
#endif
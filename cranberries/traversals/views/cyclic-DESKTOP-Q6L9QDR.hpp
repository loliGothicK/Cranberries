#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {


template < class Range >
class Cyclic
	: private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		Range traversal_;
		typename std::decay_t<Range>::iterator iter;
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		forward_sentinel(Range traversal) : traversal_{ traversal }, iter{ ::cranberries::back_magic::begin(traversal_) } {}
		forward_sentinel(const forward_sentinel&) = default;

		auto get() { return *iter; }
		void next() { if (++iter == ::cranberries::back_magic::end(traversal_)) iter = ::cranberries::back_magic::begin(traversal_); }
		std::false_type is_end() { return {}; }
	};

	class reverse_sentinel {
		Range traversal_;
		typename std::decay_t<Range>::reverse_iterator iter;
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		reverse_sentinel(Range traversal) : traversal_{ traversal }, iter{ ::cranberries::back_magic::rbegin(traversal_) } {}
		reverse_sentinel(const reverse_sentinel&) = default;

		auto get() { return *iter; }
		void next() { if (++iter == ::cranberries::back_magic::rend(traversal_) ) iter = ::cranberries::back_magic::rbegin(traversal_); }
		std::false_type is_end() { return {}; }
	};

public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = reverse_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;
	using value_type = typename std::decay_t<Range>::value_type;

	Cyclic(Range traversal) : traversal{ traversal } {}

	iterator begin() const { return { std::make_unique<forward_sentinel>(traversal) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(traversal) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
};

namespace view {
	template < typename Range >
	Cyclic<Range> cyclic(Range traversal) { return {std::forward<Range>(traversal)}; }

	inline auto cyclic() noexcept { return make_proxy<Cyclic>::piecewise()(); }
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

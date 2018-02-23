#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_FILTER_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_FILTER_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range, class Pred >
class Filter
	: private tag::sentinel_traversal_tag
	, private tag::view_tag
{
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		forward_sentinel(Range traversal, Pred pred)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::begin(traversal_) }
			, last{ cranberries::back_magic::end(traversal_) }
			, pred(pred)
		{
			if (!pred(*iter)) next();
		}

		auto get() { return *iter; }
		void next() {
			++iter;
			while (!pred(*iter) && iter != last) ++iter;
		}
		bool is_end() { return iter == last; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::iterator iter;
		typename std::decay_t<Range>::iterator last;
		Pred pred;
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = forward_sentinel;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < class Range_, class Pred_ >
	Filter(Range_&& traversal, Pred_&& pred)
		: traversal{ std::forward<Range_>(traversal) }
		, pred{ std::forward<Pred_>(pred) }
	{}

	iterator begin() const { return { std::make_unique<sentinel>(traversal, pred) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	Pred pred;
};

namespace view {
	template < typename Pred >
	auto filter(Pred&& pred) { return make_proxy<Filter>::embedded(std::forward<Pred>(pred)); }
}

}}}
#endif
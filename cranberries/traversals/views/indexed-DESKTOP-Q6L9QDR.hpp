#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../common/concepts.hpp"
#include "../detail/default_sentinel.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range >
class Indexed
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
	public:
		using value_type = std::tuple<size_t, typename std::decay_t<Range>::value_type>;
		sentinel_impl(Range traversal, size_t origin)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::begin(traversal_) }
			, last{ cranberries::back_magic::end(traversal_) }
			, idx{ origin }
		{
		}

		auto get() { return std::make_tuple(idx, *iter); }
		void next() { ++idx; ++iter; }
		bool is_end() { return iter == last ; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::iterator iter;
		typename std::decay_t<Range>::iterator last;
		size_t idx;
	};
public:
	using value_type = std::tuple<size_t, typename std::decay_t<Range>::value_type>;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < class Range_ >
	Indexed(Range_&& traversal, size_t origin)
		: traversal{ std::forward<Range_>(traversal) } 
		, origin{ origin }
	{}

	iterator begin() const { return { std::make_unique<sentinel>(traversal, origin) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	size_t origin;
};

namespace view {
	inline auto indexed(size_t origin = 0) { return make_proxy<Indexed>::forward(origin); }
}

}}}
#endif
#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <tuple>

namespace cranberries {
namespace experimental {
namespace ranges {

template < class Range >
class Indexed
	: private tag::sentinel_range_tag
{
	class sentinel_impl {
	public:
		using value_type = std::tuple<size_t, typename std::decay_t<Range>::value_type>;
		sentinel_impl(Range range, size_t origin)
			: range_{ range }
			, iter{ cranberries::back_magic::begin(range_) }
			, last{ cranberries::back_magic::end(range_) }
			, idx{ origin }
		{
		}

		auto get() { return std::make_tuple(idx, *iter); }
		void next() { ++idx; ++iter; }
		bool is_end() { return iter == last ; }
	private:
		Range range_;
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
	Indexed(Range_&& range, size_t origin)
		: range{ std::forward<Range_>(range) } 
		, origin{ origin }
	{}

	iterator begin() const { return { std::make_unique<sentinel>(range, origin) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range range;
	size_t origin;
};

class IndexedProxy
	: private tag::adaptor_proxy_tag
{
	size_t origin;
public:
	IndexedProxy(size_t origin) : origin{ origin } {};

	template < class Range >
	Indexed<Range> operator()(Range&& range) {
		return { std::forward<Range>(range), origin };
	}
};

namespace view {
	IndexedProxy indexed(size_t origin = 0) { return {origin}; }
}

}}}
#endif
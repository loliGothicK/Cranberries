#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_WITH_INDEX_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TRANFORM_WITH_INDEX_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range, class F >
class TransformWithIndex
	: private tag::adaptor_tag
	, private tag::sentinel_traversal_tag
{
	class forward_sentinel {
		Range traversal_;
		F f_;
		size_t index;
		typename std::decay_t<Range>::iterator iter;
		typename std::decay_t<Range>::iterator last;
	public:
		using value_type = std::result_of_t<F(size_t, typename std::decay_t<Range>::value_type)>;
		forward_sentinel(Range traversal, F f, size_t init)
			: traversal_{ traversal }
			, f_(f)
			, index{ init }
			, iter{ cranberries::back_magic::begin(traversal_) }
			, last{ cranberries::back_magic::end(traversal_) } {}

		auto get() { return f_(index++, *iter); }
		void next() { ++iter; }
		bool is_end() { return iter == last; }
	};

public:
	using value_type = std::result_of_t<F(size_t,typename std::decay_t<Range>::value_type)>;
	using forward_sentinel = forward_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	TransformWithIndex(Range traversal, F f, size_t init)
		: traversal{ traversal }
		, f(f)
		, init{ init } {}

	iterator begin() const { return { std::make_unique<forward_sentinel>(traversal, f, init) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	F f;
	size_t init;
};

namespace view {
	template < typename F >
	auto transform_with_index(F&& f, size_t init = 0) noexcept
		{ return make_proxy<TransformWithIndex>::piecewise( std::forward<F>(f) )( init ); }
}

}}}
#endif
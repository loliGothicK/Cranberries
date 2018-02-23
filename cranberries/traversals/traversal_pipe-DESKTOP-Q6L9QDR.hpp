#ifndef CRANBERRIES_RANGES_RANGE_PIPE_HPP
#define CRANBERRIES_RANGES_RANGE_PIPE_HPP
#include "../common/concepts.hpp"
#include "../common/macros.hpp"
#include "views/multi_demensional.hpp"
#include "views/filter.hpp"
namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range
				 , class Adaptor
				 , enabler_t<conjunction_v<
						concepts::required<Range, concepts::iterable>,
						is_adaptor<std::decay_t<Adaptor>>
				 >> = nullptr >
decltype(auto) operator<= (Range&& traversal, Adaptor&& adaptor)
CRANERRIES_AUTO_NOEXCEPT_SPECIFIED_BODY(std::forward<Adaptor>(adaptor)(std::forward<Range>(traversal)))

template < class Target, class Range,
					 concepts::concept_required<Range, concepts::iterable> = nullptr,
					 std::enable_if_t<std::is_constructible<typename std::decay_t<Range>::value_type, typename std::decay_t<Target>::value_type>::value, std::nullptr_t> = nullptr >
decltype(auto) operator|=(Target&& target, Range&& traversal) {
	target.clear();
	target.reserve(traversal.size());
	for (const auto& e : traversal) target.emplace_back(e);
	return std::forward<Target>(target);
}

template < class Traversal1, class Traversal2,
					 concepts::concept_required<Traversal1, concepts::iterable> = nullptr,
					 concepts::concept_required<Traversal2, concepts::iterable> = nullptr>
decltype(auto) operator& (Traversal1&& lhs, Traversal2&& rhs)
{
	return view::multi_demensional(std::forward<Traversal2>(rhs))(std::forward<Traversal1>(lhs));
}

template < class Traversal, class Pred,
	concepts::concept_required<Traversal, concepts::iterable> = nullptr,
	enabler_t<is_callable_v<Pred(typename std::decay_t<Traversal>::value_type)>,bool> = nullptr>
decltype(auto) operator/ (Traversal&& trv, Pred&& pred)
{
	return view::filter(std::forward<Pred>(pred))(std::forward<Traversal>(trv));
}



}}}
#endif
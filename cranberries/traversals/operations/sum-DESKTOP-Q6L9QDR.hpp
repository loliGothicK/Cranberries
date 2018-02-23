#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_SUM_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_SUM_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../../common/concepts.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
inline namespace experimental {
namespace traversals {

class Sum
	: private tag::adaptor_tag
	, private tag::action_tag
{
public:
	Sum() = default;

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<conjunction_v<
			concepts::required<Range, concepts::iterable>,
			concepts::required<ElemType, concepts::regular_type>,
			always_t<decltype(std::declval<ElemType&>() += std::declval<const ElemType&>())>
		>>;

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& traversal) {
		using std::begin;
		using std::end;
		auto iter = begin(traversal);
		auto last = end(traversal);
		auto ret = *iter;
		while (++iter != last)
		{
			ret += *iter;
		}
		return ret;
	}
};


namespace operation {
	inline Sum sum() noexcept { return {}; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

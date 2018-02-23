#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_FOLD_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_FOLD_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
inline namespace experimental {
namespace traversals {

namespace detail_ {
	enum class fold_mode {
		left,
		right,
	};
}

template < detail_::fold_mode, class > class Fold;

template < class Op >
class Fold<detail_::fold_mode::left, Op>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Op op;
public:
	template < class Op_ >
	Fold(Op_&& op) : op{ std::forward<Op_>(op) } {}

	template < class Traversable, typename ElemType = typename std::decay_t<Traversable>::value_type >
	using requires
		= std::enable_if_t<conjunction_v<
			concepts::required<Traversable, concepts::iterable>,
			is_callable<Op(ElemType, std::result_of_t<Op(ElemType, ElemType)>)>
		>>;


	template < class Traversable >
	auto
	operator()(Traversable&& traversal)
		-> decltype(op(*(::cranberries::back_magic::begin(traversal)), *(::cranberries::back_magic::begin(traversal))))
	{
		auto iter = ::cranberries::back_magic::begin(traversal);
		auto last = ::cranberries::back_magic::end(traversal);
		auto ret = *iter; ++iter;
		if (iter == last) return {};
		do {
			ret = op(ret, *iter);
		} while (++iter != last);
		return ret;
	}
};

template < class Op >
class Fold<detail_::fold_mode::right, Op>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Op op;
public:
	template < class Op_ >
	Fold(Op_&& op) : op{ std::forward<Op_>(op) } {}

	template < class Traversable, typename ElemType = typename std::decay_t<Traversable>::value_type >
	using requires
		= std::enable_if_t<conjunction_v<
			concepts::required<Traversable, concepts::reverse_iterable>,
			is_callable<Op(ElemType,std::result_of_t<Op(ElemType,ElemType)>)>
		>>;

	template < class Traversable >
	auto
	operator()(Traversable&& traversal)
		-> decltype(op(*(::cranberries::back_magic::rbegin(traversal)), *(::cranberries::back_magic::rbegin(traversal))))
	{
		auto iter = ::cranberries::back_magic::rbegin(traversal);
		auto last = ::cranberries::back_magic::rend(traversal);
		auto ret = *iter; ++iter;
		if (iter == last) return {};
		do {
			ret = op(ret, *iter);
		} while (++iter != last);
		return ret;
	}
};

namespace collectors {
	template < class Op >
	Fold<detail_::fold_mode::left, Op> foldl(Op&& op) { return { std::forward<Op>(op) }; }
	
	template < class Op >
	Fold<detail_::fold_mode::right, Op> foldr(Op&& op) { return { std::forward<Op>(op) }; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

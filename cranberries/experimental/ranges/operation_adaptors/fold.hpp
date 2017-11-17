#ifndef CRANBERRIES_RANGES_ACTION_ADAPTORS_FOLD_HPP
#define CRANBERRIES_RANGES_ACTION_ADAPTORS_FOLD_HPP
#include "../ranges_tag.hpp"
#include "../sentinel_iterator.hpp"
#include "../../../utility/utility.hpp"
#include <type_traits>
#include <iostream>

namespace cranberries {
namespace experimental {
namespace ranges {

namespace detail_ {
	enum class fold_t {
		left,
		right,
	};
}

template < detail_::fold_t, class > class Fold;

template < class Op >
class Fold<detail_::fold_t::left, Op>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Op op;
public:
	template < class Op_ >
	Fold(Op_&& op) : op{ std::forward<Op_>(op) } {}

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& range) {
		using std::begin;
		using std::end;
		auto iter = begin(range);
		auto last = end(range);
		auto a = *iter; ++iter;
		auto b = *iter;
		auto ret = op(a, b);
		while (++iter != last)
		{
			ret = op(ret, *iter);
		}
		return ret;
	}
};

template < class Op >
class Fold<detail_::fold_t::right, Op>
	: private tag::adaptor_tag
	, private tag::action_tag
{
	Op op;
public:
	template < class Op_ >
	Fold(Op_&& op) : op{ std::forward<Op_>(op) } {}

	template < class SentinelRange >
	decltype(auto)
		operator()(SentinelRange&& range) {
		using std::rbegin;
		using std::rend;
		auto iter = rbegin(range);
		auto last = rend(range);
		auto a = *iter; ++iter;
		auto b = *iter;
		auto ret = op(a, b);
		while (++iter != last)
		{
			ret = op(ret, *iter);
		}
		return ret;
	}
};

namespace action {
	template < class Op >
	Fold<detail_::fold_t::left, Op> foldl(Op&& op) { return { std::forward<Op>(op) }; }
	
	template < class Op >
	Fold<detail_::fold_t::right, Op> foldr(Op&& op) { return { std::forward<Op>(op) }; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

#ifndef CRANBERRIES_STREAMS_FORWARD_HPP
#define CRANBERRIES_STREAMS_FORWARD_HPP

#include "detail\tag.hpp"
#include "operators\Identity.hpp"

namespace cranberries {
namespace streams {
namespace operators {


	template < typename BinaryFunc >
	struct AdjacentForEach;

	template < typename Pred >
	struct All;

	template < typename Pred >
	struct Any;

	template < typename Branch >
	struct Concatenate;

	struct DistinctProxy;

	template < typename T >
	struct Distinct;

	template < typename Pred >
	struct Filter;

	template < typename F >
	struct Tinvoker;

	template < typename F >
	struct Iinvoker;

	struct Take;

	struct Taker;

	template < typename OldStream, typename UnaryFunc	>
	struct Map;

	template < typename FromStream,	typename To	>
	struct MapTo;

	template < typename Pred >
	struct None;

	template < typename F >
	struct MapProxy;

	template < typename UnaryFunc	>
	struct Transform;

	template < typename T >
	struct PushBack;

	struct PopBack;

	struct Repetition;

	struct Reverse;

	struct Identity;

	struct Drop;

	template < typename Pred = detail::defaulted >
	struct Sort;

	struct Splitter;

	template < typename To >
	struct MapToProxy;

	struct Run;

	struct Printer;

	template < typename Pred >
	struct TakeWhile;

	template < typename UnaryOp >
	struct ForEach;

	template < typename UnaryOp >
	struct Peek;

	template < typename Pred >
	struct DropWhile;

	template < typename Pred >
	struct DropperWhile;

	template < typename Stream >
	struct MergeProxy;

	template < typename Stream >
	struct Merge;

	struct UniqueProxy;

	template < typename T >
	struct Unique;

} // ! namesapce operators
} // ! namespace stream
} // ! namespace cranberries

#endif
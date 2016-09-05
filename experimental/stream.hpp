#ifndef CRANBERRIES_STREAM_HPP
#define CRANBERRIES_STREAM_HPP
// Stream
#include <iostream>
#include <sstream>
#include <fstream>
// String
#include <string>
#include <regex>
// Sequence container
#include <array>
#include <vector>
#include <forward_list>
#include <list>
#include <deque>
// Container adaptor
#include <queue>
#include <stack>
// Associative container
#include <set>
#include <map>
// Unordered associative container
#include <unordered_set>
#include <unordered_map>
// Others
#include <type_traits>
#include <algorithm>
#include <utility>
#include <functional>
#include <future>
// exception
#include "stream_error.hpp"

namespace cranberries {
namespace stream {

	//--------//
	// detail //
	//--------//

	// Type Transfer
	template < typename Target >
	struct ConvertMarker {};

	// Defaluted Tag
	struct defaulted {};

	struct is_tree{};
	struct not_tree{};

	struct chainable{};
	struct connectable{};
	struct unconectable{};
	struct generator {};

	// Unary Operator Callable Checker Implementation
	struct isAvailableUnaryOpImpl
	{
		template <
			typename T,
			typename UnaryFunc
		>
		static constexpr
		auto check
		(
			T&& a,
			UnaryFunc&& lambda
		)
			->decltype(lambda(a), std::true_type{});

		template <
			typename T,
			typename UnaryFunc
		>
		static constexpr
		auto
		check(...)->std::false_type;
	};

	// Unary Operator Callable Checker
	template < typename T, typename UnaryOp >
	struct isAvailableUnaryOp
		: decltype(
			isAvailableUnaryOpImpl::check<T, UnaryOp>( std::declval<T>(), std::declval<UnaryOp>()	)
	) {};

	// Binary Operator Callable Checker Implementation
	struct isAvailableBinaryOpImpl
	{
		template <
			typename T,
			typename BinaryFunc
		>
		static constexpr
		auto check
		(
			T&& a,
			BinaryFunc&& lambda
		)
			->decltype(lambda(a,a), std::true_type{});

		template <
			typename T,
			typename BinaryFunc
		>
		static constexpr
		auto check(...)->std::false_type;
	};

	// Binary Operator Callable Checker
	template <
		typename T,
		typename BinaryOp
	>
	struct isAvailableBinaryOp
		: decltype(
			isAvailableBinaryOpImpl::check<T, BinaryOp>( std::declval<T>(), std::declval<BinaryOp>() )
	) {};

	template < typename Chainable >
	struct Chain
	{
		using connect_tag = chainable;
		using tree_tag = not_tree;
		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for ( auto&& e : stream )	e = f(e);
			return std::forward<STREAM>(stream);
		}
		Chainable f;
	};

	template < typename Connectable >
	struct Connection
	{
		using connect_tag = connectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)	f(e);
			return std::forward<STREAM>(stream);
		}
		Connectable f;
	};

	template < typename Pred >
	struct ConnectAll
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)	if (!pred(e)) return false;
			return true;
		}
		Pred pred;
	};

	template < typename Pred >
	struct ConnectAny
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)	if (pred(e)) return true;
			return false;
		}
		Pred pred;
	};

	template < typename Pred >
	struct ConnectNone
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)	if (pred(e)) return false;
			return true;
		}
		Pred pred;
	};


	template < template<class> class Connect, typename F >
	inline auto make_connect(F&& f)
	{
		return Connect<F>{ std::move(f) };
	}

	//-----------------------------//
	// Intermidiate Operation Tree //
	//-----------------------------//


	template <
		typename Op1, // evaluate first
		typename Op2  // evaluate next
	>
	struct OperationTree
	{
		using connect_tag = typename Op2::connect_tag;
		using tree_tag = is_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()(
			STREAM&& stream
		) {
			return op2(op1(std::forward<STREAM>(stream)));
		}

		// members
		Op1 op1;
		Op2 op2;
	};



	//----------------------------//
	// Terminate Operation Root   //
	//----------------------------//

	struct RootOperation
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		)
			noexcept
		{
			return std::forward<STREAM>(stream);
		}
	};

	template < typename Op1, typename Op2 >
	struct MakeOpTreeTraits {
		static constexpr auto invoke(Op1 op1, Op2 op2) {
			return OperationTree<Op1, Op2>{ std::move(op1), std::move(op2) };
		}
	};
	template < typename Op2 >
	struct MakeOpTreeTraits<RootOperation, Op2> {
		static constexpr auto invoke(RootOperation, Op2 op2) {
			return std::forward<Op2>(op2);
		}
	};

	template < typename Pred >
	struct All;
	template < typename Pred >
	struct Any;
	template < typename Pred >
	struct None;

	template < typename IsTree >
	struct ConnectTraits{};

	template < >
	struct ConnectTraits<not_tree>
	{
		template < typename Op1, typename Op2 >
		static constexpr auto chaining(Op1&& op1, Op2&& op2)
		{
			return make_connect<Chain>([f1 = std::move(op1.f), f2 = std::move(op2.f)](auto&& arg){ return f2(f1(arg)); });
		}
		template < typename Op1, typename Op2 >
		static constexpr auto chain_connect(Op1&& op1, Op2&& op2)
		{
			return make_connect<Connection>([f1 = std::move(op1.f), f2 = std::move(op2.f)](auto&& arg){ f2(f1(arg)); });
		}
		template < typename Op1, typename Op2 >
		static constexpr auto connect_chain(Op1&& op1, Op2&& op2)
		{
			return make_connect<Chain>([f1 = std::move(op1.f), f2 = std::move(op2.f)](auto&& arg){ return f1(arg),f2(arg); });
		}
		template < typename Op1, typename Op2 >
		static constexpr auto connecting(Op1&& op1, Op2&& op2)
		{
			return make_connect<Connection>([f1 = std::move(op1.f), f2 = std::move(op2.f)](auto&& arg){ f1(arg),f2(arg); });
		}
		template < typename Op1, typename Pred >
		static constexpr auto connect_all(Op1&& op1, Pred&& pred)
		{
			return make_connect<ConnectAll>([f1 = std::move(op1.f), f2 = std::move(pred)](auto&& arg){ return f1(arg), f2(arg); });
		}
		template < typename Op1, typename Pred >
		static constexpr auto chain_all(Op1&& op1, Pred&& pred)
		{
			return make_connect<ConnectAll>([f1 = std::move(op1.f), f2 = std::move(pred)](auto&& arg){ return f2(f1(arg)); });
		}
		template < typename Op1, typename Pred >
		static constexpr auto connect_any(Op1&& op1, Pred&& pred)
		{
			return make_connect<ConnectAny>([f1 = std::move(op1.f), f2 = std::move(pred)](auto&& arg){ return f1(arg), f2(arg); });
		}
		template < typename Op1, typename Pred >
		static constexpr auto chain_any(Op1&& op1, Pred&& pred)
		{
			return make_connect<ConnectAny>([f1 = std::move(op1.f), f2 = std::move(pred)](auto&& arg){ return f2(f1(arg)); });
		}
		template < typename Op1, typename Pred >
		static constexpr auto connect_none(Op1&& op1, Pred&& pred)
		{
			return make_connect<ConnectNone>([f1 = std::move(op1.f), f2 = std::move(pred)](auto&& arg){ return f1(arg), f2(arg); });
		}
		template < typename Op1, typename Pred >
		static constexpr auto chain_none(Op1&& op1, Pred&& pred)
		{
			return make_connect<ConnectNone>([f1 = std::move(op1.f), f2 = std::move(pred)](auto&& arg){ return f2(f1(arg)); });
		}
	};

	template < >
	struct ConnectTraits<is_tree>
	{
		template < typename Op1, typename Op2 >
		static constexpr auto chaining(Op1&& op1, Op2&& op2)
		{
			return make_op_tree(
				std::move(op1.op1),
				make_connect<Chain>([f1 = std::move(op1.op2.f), f2 = std::move(op2.f)](auto&& arg){ return f2(f1(arg)); })
			);
		}
		template < typename Op1, typename Op2 >
		static constexpr auto chain_connect(Op1&& op1, Op2&& op2)
		{
			return make_op_tree(
				std::move(op1.op1),
				make_connect<Connection>([f1 = std::move(op1.op2.f), f2 = std::move(op2.f)](auto&& arg){ f2(f1(arg)); })
			);
		}
		template < typename Op1, typename Op2 >
		static constexpr auto connect_chain(Op1&& op1, Op2&& op2)
		{
			return make_op_tree(
				std::move(op1.op1),
				make_connect<Chain>([f1 = std::move(op1.op2.f), f2 = std::move(op2.f)](auto&& arg){ return f1(arg), f2(arg); })
			);
		}
		template < typename Op1, typename Op2 >
		static constexpr auto connecting(Op1&& op1, Op2&& op2)
		{
			return make_op_tree(
				std::move(op1.op1),
				make_connect<Connection>([f1 = std::move(op1.op2.f), f2 = std::move(op2.f)](auto&& arg){ f1(arg), f2(arg); })
			);
		}
		template < typename Op1, typename Pred >
		static constexpr auto connect_all(Op1&& op1, Pred&& pred)
		{
			return make_op_tree(
				std::move(op1.op1),
				make_connect<ConnectAll>([f1 = std::move(op1.op2.f), f2 = std::move(pred)](auto&& arg){ f1(arg), f2(arg); })
			);
		}
		template < typename Op1, typename Pred >
		static constexpr auto chain_all(Op1&& op1, Pred&& pred)
		{
			return make_op_tree(
				std::move(op1.op1),
				make_connect<ConnectAll>([f1 = std::move(op1.op2.f), f2 = std::move(pred)](auto&& arg){ f2(f1(arg)); })
			);
		}
	};


	template < typename Op1, typename Op2 >
	inline constexpr auto make_op_tree(Op1&& op1, Op2&& op2, ...)
	{
		return MakeOpTreeTraits<Op1, Op2>::invoke(std::forward<Op1>(op1), std::forward<Op2>(op2));
	}
	template < typename Op1, typename Op2 >
	inline constexpr auto make_op_tree(Op1&& op1, Op2&& op2, chainable, chainable)
	{
		return ConnectTraits<typename Op1::tree_tag>::chaining(std::forward<Op1>(op1), std::forward<Op2>(op2));
	}
	template < typename Op1, typename Op2 >
	inline constexpr auto make_op_tree(Op1&& op1, Op2&& op2, chainable, connectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::chain_connect(std::forward<Op1>(op1), std::forward<Op2>(op2));
	}
	template < typename Op1, typename Op2 >
	inline constexpr auto make_op_tree(Op1&& op1, Op2&& op2, connectable, chainable)
	{
		return ConnectTraits<typename Op1::tree_tag>::connect_chain(std::forward<Op1>(op1), std::forward<Op2>(op2));
	}
	template < typename Op1, typename Op2 >
	inline constexpr auto make_op_tree(Op1&& op1, Op2&& op2, connectable, connectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::connecting(std::forward<Op1>(op1), std::forward<Op2>(op2));
	}
	template < typename Op1, typename Pred >
	inline constexpr auto make_op_tree(Op1&& op1, All<Pred>&& all, connectable, unconectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::connect_all(std::forward<Op1>(op1), std::move(all.pred));
	}
	template < typename Op1, typename Pred >
	inline constexpr auto make_op_tree(Op1&& op1, All<Pred>&& all, chainable, unconectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::chain_all(std::forward<Op1>(op1), std::move(all.pred));
	}
	template < typename Op1, typename Pred >
	inline constexpr auto make_op_tree(Op1&& op1, Any<Pred>&& any, connectable, unconectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::connect_any(std::forward<Op1>(op1), std::move(any.pred));
	}
	template < typename Op1, typename Pred >
	inline constexpr auto make_op_tree(Op1&& op1, Any<Pred>&& any, chainable, unconectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::chain_any(std::forward<Op1>(op1), std::move(any.pred));
	}
	template < typename Op1, typename Pred >
	inline constexpr auto make_op_tree(Op1&& op1, None<Pred>&& none, connectable, unconectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::connect_none(std::forward<Op1>(op1), std::move(none.pred));
	}
	template < typename Op1, typename Pred >
	inline constexpr auto make_op_tree(Op1&& op1, None<Pred>&& none, chainable, unconectable)
	{
		return ConnectTraits<typename Op1::tree_tag>::chain_none(std::forward<Op1>(op1), std::move(none.pred));
	}

	template < typename F >
	struct MapProxy {
		F f;
	};

	//------------//
	// Operations //
	//------------//


	//-------------------//
	// Stream Generators //
	//-------------------//

	// Stream Create Operation
	// Create stream from the type that convertible to std::string.
	// Regex split and generate stream.
	struct Splitter
	{
		Splitter(std::string s) : r_{ s } {}

		// member

		std::regex r_{};
	};

	struct Creator {};

	// Terminate Operation
	// Print each element of range stream has.
	struct Printer
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		void
		operator()
		(
			STREAM&& stream
		)
			const noexcept
		{
			auto&& iter = stream.begin();
			os << *iter;
			++iter;
			for (; iter != stream.end(); ++iter)
				os << delim << *iter;
			os << std::endl;
		}

		// members

		std::ostream& os; // defalut is std::cout
		std::string delim; // defalut is ", "
	};

	// Intermidiate / Terminate Operation
	// Applying UnaryFunc each element of range stream has.
	template <
		typename UnaryFunc
	>
	struct ForEach
	{
		using connect_tag = connectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream) f(e);
			return std::forward<STREAM>(stream);
		}

		// mapper

		UnaryFunc f;
	};

	template <
		typename UnaryFunc
	>
	struct Map
	{
		using connect_tag = chainable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream) e = f(e);
			return std::forward<STREAM>(stream);
		}

		// mapper

		UnaryFunc f;
	};

	template <
		typename OldStream,
		typename UnaryFunc
	>
	struct Endomorphism
	{
		using connect_tag = chainable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			stream.reserve(old.size());
			for (auto&& e : old) stream.emplace_back(f(e));
			return std::forward<STREAM>(stream);
		}

		// member
		OldStream old;
		UnaryFunc f;
	};


	// Intermidiate Operation
	template <
		typename Pred
	>
	struct Filter
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			for (auto&& iter = source.begin(); iter != source.end(); ) {
				if (pred(*iter)) iter = source.erase(iter);
				else ++iter;
			}
			stream.shrink_to_fit();
			return std::forward<STREAM>( stream );
		}

		// member

		Pred pred;
	};

	// Intermidiate Operation
	template <
		typename Pred = defaulted
	>
	struct Sort
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			std::sort(stream.begin(), stream.end(), pred);
			return std::forward<STREAM>( stream );
		}

		// member

		Pred pred;
	};

	// Intermidiate Operation
	template < >
	struct Sort <
		defaulted // lookup operator < using ADL.
	>
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;
		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()(
			STREAM&& stream
		) {
			std::sort(stream.begin(), stream.end());
			return std::forward<STREAM>( stream );
		}
	};

	// Intermidiate Operation
	struct Distinct
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			std::sort(source.begin(), source.end());
			source.erase(std::unique(source.begin(), source.end()), source.end());
			return std::forward<STREAM>( stream );
		}
	};

	// Intermidiate Operation
	struct Skip
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM,
			typename Range = typename std::decay_t<STREAM>::range_type
		>
		inline
		decltype(auto)
		operator()(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			Range{ source.begin() + n, source.end() }.swap(source); // swap trick
			return std::forward<STREAM>( stream );
		}

		// member

		std::size_t n;
	};

	// Intermidiate Operation
	struct Limit
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM,
			typename Range = typename std::decay_t<STREAM>::range_type
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& source = stream.get();
			Range{ source.begin(), source.begin() + n }.swap(source); // swap trick
			return std::forward<STREAM>( stream );
		}

		// member

		std::size_t n;
	};

	// Intermidiate Operation
	template <
		typename T
	>
	struct PushBack
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			stream.push_back(v);
			return std::forward<STREAM>( stream );
		}

		// member

		T v;
	};

	// Intermidiate Operation
	struct PopBack
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			stream.pop_back();
			return std::forward<STREAM>( stream );
		}
	};

	// Intermidiate Operation
	struct Reverse
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			std::reverse(stream.begin(), stream.end());
			return std::forward<STREAM>( stream );
		}
	};

	// Intermidiate Operation
	template <
		typename BinaryFunc
	>
	struct AdjacentForEach
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			// for ADL
			using std::begin;
			using std::end;

			auto&& first = begin(stream);
			auto&& last = end(stream);

			while (std::next(first) != last)
			{
				auto&& a = *first;
				++first;
				auto&& b = *first;
				f(a, b);
			}
			return std::forward<STREAM>( stream );
		}

		// member

		BinaryFunc f;
	};

	template <
		typename Branch
	>
	struct Concatenate
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		// for evaluate Branch Stream only once
		void
		once()
		{
			static bool flag = true;
			if (flag) {
				branch.eval();
				flag = false;
			}
		}

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			once(); // perfectly call once
			auto&& lv = stream.get();
			auto&& rv = branch.get();
			if (lv.capacity() < lv.size() + rv.size())
				lv.reserve(lv.size() + rv.size());
			lv.insert(lv.end(), rv.begin(), rv.end());
			return std::forward<STREAM>( stream );
		}

		// member

		Branch branch; // Branch Stream
	};

	template < typename To >
	struct MapToProxy{};

	struct ToString{};

	template <
		typename FromStream,
		typename To
	>
	struct MapTo
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			from.eval();
			stream.get() = std::vector<To>{ from.begin(), from.end() };
			return std::forward<STREAM>(stream);
		}

		FromStream from;
	};

	template <
		typename FromStream
	>
	struct MapTo<
		FromStream,
		std::string
	>
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		)	{
			using std::to_string; // For ADL
			from.eval();
			for (auto&& e : from) stream.emplace_back( to_string(e) );
			return std::forward<STREAM>(stream);
		}

		FromStream from;
	};

	template < typename Pred >
	struct All
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator ()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)
				if (!pred(e)) return false;
			return true;
		}

		// member

		Pred pred;
	};

	template < typename Pred >
	struct Any
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator ()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)
				if (pred(e)) return true;
			return false;
		}

		// member

		Pred pred;
	};

	template < typename Pred >
	struct None
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator ()
		(
			STREAM&& stream
		) {
			for (auto&& e : stream)
				if (pred(e)) return false;
			return true;
		}

		// member

		Pred pred;
	};


	//------------------------------//
	// Just Time Evaluate Operation //
	//------------------------------//

	// Just Time Evaluate Operation
	struct Repeat { size_t times; };

	// Just Time Evaluate Operation
	struct Run {};

	//----------------------//
	// Operation Assistants //
	//----------------------//


	// Adaptor for user defined operation and Stream operation interface.
	// Concept : Operator must call with arg of any Stream and return it.
	template <
		typename Operator // user defined operation
	>
	struct Invoker
	{
		using connect_tag = unconectable;
		using tree_tag = not_tree;

		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			op(stream);
			return std::forward<STREAM>(stream);
		}

		// member

		Operator op;
	};


	//-------------//
	// Stream Body //
	//-------------//

	/*

	[ Note : Stream process is one of iterate process.
	Abstract iterate process as stream.
	Stream is abstract process that continuously operating to treat range object through pipeline.

	<< Stream Create Phase >>
	Stream<T> is able to construct from Range<T>, iterator pair has element_type as T and initializer_list<T>.
	Template parameter T is source type. In Stream, source manageed as vector<T>.
	Template parameter Operation is Operation Tree.
	Root Operation is default setting at creating Stream<T> .

	<< Pipeline Construction Phase >>
	You can chain Intermidiate Operation.
	Intermidiate Operation is lazy evaluate.
	The return type Intermidiate Operation is temporary proxy stream object.
	Operation is reccuresively construct like 'Operation<A,Operation<B,C>>'.
	To bind temporary proxy stream, you can use 'auto' keyword.

	<< Termination Phase >>
	You can execute Terminate Operation or convert stream to container.
	Execute Terminate Operation or onvert to container, evaluating Intermidiate Operation Pipeline.

	Let 's' is a Stream and 'A', 'B' are Intermidiate Querys, and 'C' is Terminate Operation.
	Stream Intermidiate Querys are combine with operator >> and Terminate Operation using operator || .

	[ Example :
	s >> A >> B || C;
	- end example ]

	When convert stream to container, use convert kryword( constexpr variable ).
	Implicit convert to any container from iterator pair.

	[ Example :
	std::vector<int> v = s >> A >> B || convert;
	- end example ]


	- end note ]

	*/

	class StreamBase {};

	template <
		typename T,
		typename Operation = RootOperation
	>
	class Stream
		: StreamBase
	{
	public:
		typedef T element_type;
		typedef std::vector<T> range_type;
		typedef typename std::vector<T>::reference reference;
		typedef typename std::vector<T>::const_reference const_reference;
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::size_type size_type;
		typedef typename std::vector<T>::difference_type difference_type;
		typedef typename std::vector<T>::reverse_iterator reverse_iterator;
		typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

		template <
			typename Iterator
		>
		Stream
		(
			Iterator first,
			Iterator last,
			Operation q
		)
			: source{ first,last }
			, operation{ q }
		{}

		Stream
		(
			std::initializer_list<T> init_list,
			Operation q
		)
			: source{ init_list }
			, operation{ q }
		{}

		template <
			typename Range
		>
		Stream
		(
			Range&& range,
			Operation&& q
		)
			: source{ range.begin(), range.end() }
			, operation{ q }
		{}

		template <
			typename Iterator
		>
		Stream
		(
			Iterator first,
			Iterator last
		)
			: source{ first,last }
			, operation{ RootOperation{} }
		{}

		Stream
		(
			std::initializer_list<T> init_list
		)
			: source{ init_list }
			, operation{ RootOperation{} }
		{}

		template <
			typename Range
		>
		Stream
		(
			Range&& range
		)
			: source{ range.begin(), range.end() }
			, operation{ RootOperation{} }
		{}

		Stream()
			: source{}
			, operation{ RootOperation{} }
		{}

		Stream(Stream const&) = default;
		Stream(Stream&&) = default;
		~Stream() = default;
		Stream& operator=(Stream const&) = default;
		Stream& operator=(Stream&&) = default;

		// Stream to Range converter
		template <
			typename TargetRange // Concept : Range must be construct from iterator pair.
		>
		inline
		TargetRange
		convert
		(
			ConvertMarker<TargetRange>
		)
			const
		{
			return TargetRange{ source.begin(), source.end() };
		}

		// Range wrappers

		inline decltype(auto) begin() { return source.begin(); }

		inline decltype(auto) end() { return source.end(); }

		inline decltype(auto) begin() const { return source.cbegin(); }

		inline decltype(auto) end() const { return source.cend(); }

		inline decltype(auto) cbegin() const { return source.cbegin(); }

		inline decltype(auto) cend() const { return source.cend(); }

		inline decltype(auto) rbegin() { return source.rbegin(); }

		inline decltype(auto) rend() { return source.rend(); }

		inline decltype(auto) crbegin() const { return source.crbegin(); }

		inline decltype(auto) crend() const { return source.crend(); }

		inline decltype(auto) size() { return source.size(); }

		inline decltype(auto) max_size() { return source.max_size(); }

		inline decltype(auto) resize(size_t n) { return source.resize(n); }

		inline decltype(auto) capacity() { return source.capacity(); }

		inline bool empty() { return source.empty(); }

		inline decltype(auto) reserve(size_t n) { return source.reserve(n); }

		inline decltype(auto) insert(const_iterator pos, T const& v) { return source.insert(pos, v); }

		inline decltype(auto) insert(const_iterator pos, T&& v) { return source.insert(pos, std::move(v)); }

		inline decltype(auto) insert(const_iterator pos,size_type n, T const& v) { return source.insert(pos, n, v) }

		template <class InputIterator>
		decltype(auto) insert(
			const_iterator pos,
			InputIterator first,
			InputIterator last
		) {
			return source.insert( pos, first, last );
		}

		inline decltype(auto) insert(
			const_iterator pos,
			std::initializer_list<T> il
		) {
			return source.insert( pos, il );
		}

		template < typename U >
		inline void push_back( U&& v ) { source.push_back(v); }

		inline void pop_back() { source.pop_back(); }

		template < typename U >
		inline void emplace_back( U&& v ) { source.emplace_back(v); }

		inline void shrink_to_fit() { source.shrink_to_fit(); }

		// resource getter
		inline auto& get() { return source; }


		// Operator Registration
		template <
			typename Operator
		>
		inline
		auto
		lazy(
			Operator&& op
		) {
			return make_stream_with_op(
				std::move(source), // move resource
				make_op_tree(
					std::move(operation),
					std::move(op),
					typename Operation::connect_tag{},
					typename std::decay_t<Operator>::connect_tag{}
				) // make operation tree
			);
		}

		// execute lazy evaluation
		inline
		decltype(auto)
		eval()
		{
			return operation(*this); // evaluate operation tree
		}

		// members
	private:

		// source source
		std::vector<T> source{};

		// Operation Tree
		Operation operation{};
	};


	//----------------//
	// Stream Utility //
	//----------------//

	template < typename T, typename Tree >
	inline auto make_stream_with_op( std::vector<T>&& range, Tree&& tree)
	{
		return Stream<T, Tree>{ std::move(range), std::move(tree) };
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	begin
	(
		STREAM&& stream
	) {
		return stream.begin();
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	end
	(
		STREAM&& stream
	) {
		return stream.end();
	}

	template < typename T >
	struct RangeStream {

		template <
			typename STREAM
		>
		decltype(auto)
		operator ()
		(
			STREAM&& stream
		) {
			for (; first < last; first+=step)
				stream.emplace_back(first);
			return std::forward<STREAM>(stream);
		}


		T first;
		T last;
		T step;
	};

	template < typename Op1, typename Op2 >
	struct OperatingExpression {
		template < typename T >
		decltype(auto) operator()(T&& arg) { return op2(op1(arg)); }
		Op1 op1;
		Op2 op2;
	};


	struct Identity {
		template < typename T >
		T operator () (T a) {
			return a;
		}
	};

	template < typename InfiniteStream >
	struct Limited{
		using connect_tag = generator;
		using tree_tag = not_tree;
		using E = typename InfiniteStream::element_type;

		template < typename STREAM >
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			stream.reserve(lim);
			std::pair<bool, E> tmp;
			for (size_t count{}; count < lim;) {
				tmp = inf.get();
				if (tmp.first) {
					stream.emplace_back(tmp.second);
					++count;
				}
				inf.advance();
			}
			return std::forward<STREAM>(stream);
		}
		InfiniteStream inf;
		size_t lim;
	};

	template <
		typename InfiniteStream,
		typename Pred
	>
	struct Taken {
		using connect_tag = generator;
		using tree_tag = not_tree;
		using E = typename InfiniteStream::element_type;

		template < typename STREAM >
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			std::pair<bool, E> tmp;
			do {
				tmp = inf.get();
				if (tmp.first) stream.emplace_back(tmp.second);
				inf.advance();
			}
			while ( tmp.first ? pred(tmp.second) : true );
			return std::forward<STREAM>(stream);
		}
		InfiniteStream inf;
		Pred pred;
	};


	template <
		typename T,
		typename Func,
		typename Operation = Identity
	>
	struct IterateStream {
		using element_type = std::result_of_t<Operation(T)>;

		std::pair<bool,element_type> get() try {
			return std::make_pair(true,op(init));
		}
		catch (except::filtered_except const&) {
			return std::make_pair(false, element_type{});
		}

		void advance() { init = f(init); }

		template < typename F >
		IterateStream<T, Func, OperatingExpression<Operation, F>> push_expr(F&& func) {
			return{ std::move(init), std::move(f), OperatingExpression<Operation, F>{std::move(op),std::move(func)} };
		}

		T init;
		Func f;
		Operation op;
	};

	template < typename Pred >
	struct TakeWhile {
		Pred pred;
	};

	template < typename Pred >
	inline TakeWhile<Pred> takeWhile(Pred&& pred) noexcept{
		return{ std::forward<Pred>(pred) };
	}

	template <
		typename T,
		typename F,
		typename Op,
		typename Pred,
		typename E = typename IterateStream<T, F, Op>::element_type
	>
	inline
	auto
	operator >>
	(
		IterateStream<T, F, Op>&& iter,
		TakeWhile<Pred>&& tw
	) {
		using Inf = IterateStream<T, F, Op>;
		return Stream<E, Taken<Inf, Pred>>{ std::vector<E>{}, Taken<Inf, Pred>{ std::move(iter), std::move(tw.pred) } };
	}


	template <
		typename T,
		typename F,
		typename Op,
		typename E = typename IterateStream<T, F, Op>::element_type
	>
	inline
	auto
	operator >>
	(
		IterateStream<T, F, Op>&& iter,
		Limit&& lim
	) {
		using Inf = IterateStream<T, F, Op>;
		return Stream<E, Limited<Inf>>{ std::vector<E>{}, Limited<Inf>{ std::move(iter), std::move(lim.n) } };
	}

	template <
		typename T,
		typename F,
		typename Op,
		typename M
	>
	inline
	auto
	operator >>
	(
		IterateStream<T, F, Op>&& iter,
		MapProxy<M>&& proxy
	) {
		return iter.push_expr(proxy.f);
	}

	template <
		typename T,
		typename F,
		typename Op,
		typename To
	>
	inline
	auto
	operator >>
	(
		IterateStream<T, F, Op>&& iter,
		MapToProxy<To>&& proxy
	) {
		iter.push_expr([](auto&& a){ return static_cast<To>(a); });
	}

	template <
		typename T,
		typename F,
		typename Op,
		typename UnaryOp
	>
	inline
	auto
	operator >>
	(
		IterateStream<T, F, Op>&& iter,
		ForEach<UnaryOp>&& proxy
	) {
		iter.push_expr([f = proxy.f](auto&& a) { return f(a),a; });
	}

	//------------------//
	// Stream Generator //
	//------------------//

	struct makeStream {
		template <
			typename Iterator,
			typename T = typename std::decay_t<Iterator>::value_type
		>
		static
		Stream<T>
		of
		(
			Iterator first,
			Iterator last
		)
			noexcept
		{
			return Stream<T>{ first, last };
		}

		template <
			typename T
		>
		static
		Stream<T>
		of
		(
			std::initializer_list<T> il
		)
			noexcept
		{
			return Stream<T>{ il };
		}


		template <
			typename Range,
			typename T = typename std::decay_t<Range>::value_type
		>
		static
		auto
		from
		(
			Range&& range
		)
			noexcept -> decltype(range.begin(), range.end(), Stream<T>{})
		{
			return Stream<T>{ range.begin(), range.end() };
		}

		template <
			typename T
		>
		static
		Stream<T,RangeStream<T>>
		range
		(
			T first,
			T last,
			T step = 1
		) {
			return Stream<T, RangeStream<T>>{ std::vector<T>{}, RangeStream<T>{first,last,step} };
		}

		template <
			typename T,
			typename F
		>
		static
		auto
		iterate
		(
			T&& init,
			F&& f
		) {
			return IterateStream<T, F>{ std::forward<T>(init), std::forward<F>(f), Identity{} };
		}

		template <
			typename Iterable
		>
		static
		auto
		counter
		(
			Iterable init
		) {
			auto next = [](auto&& a) { return a + 1; };
			return IterateStream<Iterable, decltype(next)>{ init, std::move(next), Identity{} };
		}

		template <
			typename T
		>
		static
		auto
		repeat
		(
			T val
		) {
			return IterateStream<T, Identity>{ val, Identity{}, Identity{} };
		}

		template <
			typename T
		>
		static
		auto
		repeat
		(
			T val,
			size_t lim
		) {
			return Stream<T>{std::vector<T>(lim,val)};
		}


	};


	template <
		typename STR
	>
	inline
	Stream<std::string>
	operator /
	(
		STR&& s,
		Splitter&& sp
	)
		noexcept
	{
		static_assert(
			std::is_constructible<std::string, STR>::value,
			"Can not construct std::string from STR."
		);
		auto&& str = std::string{ s };
		return{ std::sregex_token_iterator(str.begin(),str.end(), sp.r_, -1), std::sregex_token_iterator{} };
	}

	//----------------------//
	// Intermediate process //
	//----------------------//


	inline Splitter split(std::string s) noexcept { return{ s }; }

	template <
		typename Pred
	>
	inline
	Filter<Pred>
	filtered
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}

	inline Sort<> sorted() noexcept { return{}; }

	template <
		typename Pred
	>
	inline
	Sort<Pred>
	sorted
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}

	inline Distinct distinct() noexcept	{	return{};	}

	inline Skip skip(std::size_t n)	noexcept {	return{ n }; }

	inline Limit limit(std::size_t n) noexcept	{ return{ n }; }

	template < typename T >
	inline PushBack<std::decay_t<T>> push_back(T&& v) noexcept { return{ v };	}

	inline PopBack pop_back()	noexcept {	return{};	}

	template < typename To >
	inline MapToProxy<To> map_to() noexcept { return{}; }

	inline MapToProxy<std::string> to_string() noexcept { return{}; }

	inline Repeat repeat(size_t times = 1) noexcept { return{ times };	}

	inline Run run() noexcept { return{}; }

	template <
		typename BinaryFunc
	>
	inline
	AdjacentForEach<BinaryFunc>
	adjacent_for_each
	(
		BinaryFunc&& f
	)
		noexcept
	{
		return{ std::forward<BinaryFunc>(f) };
	}

	template <
		typename STREAM, typename Pred,
		typename E = typename STREAM::element_type
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Filter<Pred>&& filter
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype( std::declval<Pred>()( std::declval<E>() ) )
			>::value,
			"Can not call Pred with an arg of Stream::elemetnt_type or return type is not bool."
		);

		return stream.lazy(filter);
	}

	template <
		typename STREAM, typename Pred,
		typename E = typename STREAM::element_type
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Sort<Pred>&& so
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype( std::declval<Pred>()( std::declval<E>(), std::declval<E>() ) )
			>::value,
			"Can not call Pred with 2 args of Stream::elemetnt_type or return type is not bool."
		);

		return stream.lazy(so);
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Sort<>&& so
	)
		noexcept
	{
		return stream.lazy(so);
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Distinct&& di
	)
		noexcept
	{
		return stream.lazy(di);
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Skip&& sk
	)
		noexcept
	{
		return stream.lazy(sk);
	}

	template <
		typename STREAM
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Limit&& lim
	)
		noexcept
	{
		return stream.lazy(lim);
	}

	template <
		typename STREAM1,
		typename STREAM2
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM1&& stream,
		Concatenate<STREAM2>&& concat
	)
		noexcept
	{

		static_assert(
			std::is_same<typename STREAM1::element_type, typename STREAM2::element_type>::value
			"Call 'concat' with different element_type."
		);

		return std::forward<STREAM1>( stream.lazy(std::move(concat)) );
	}

	template <
		typename STREAM,
		typename T
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		PushBack<T>&& pb
	)
		noexcept
	{
		static_assert(
			std::is_constructible<typename STREAM::element_type, T>::value,
			"Can not construct element_type from T."
		);

		return stream.lazy(pb);
	}

	template <
		typename STREAM,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		PopBack&& pb
	)
		noexcept
	{
		return stream.lazy(pb);
	}

	//------------------------------------//
	// Intermidiate / Termination Process //
	//------------------------------------//

	template <
		typename UnaryFunc
	>
	inline
	ForEach<UnaryFunc>
	for_each
	(
		UnaryFunc&& func
	)
		noexcept
	{
		return{ std::forward<UnaryFunc>(func) };
	}

	template <
		typename UnaryFunc
	>
	inline
	MapProxy<UnaryFunc>
	map_
	(
		UnaryFunc&& func
	)
		noexcept
	{
		return{ std::forward<UnaryFunc>(func) };
	}



	template <
		typename STREAM,
		typename UnaryOp,
		typename E = typename STREAM::element_type
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		ForEach<UnaryOp>&& f
	)
		noexcept
	{
		static_assert(
			std::is_same<
				void,
				std::result_of_t<UnaryOp(E)>
			>::value,
			"Can not call UnaryOp with an arg of STREAM::element_type."
		);

		return stream.lazy(f);
	}


	template <
		typename STREAM,
		typename UnaryOp,
		typename E = typename STREAM::element_type,
		typename R = std::result_of_t<UnaryOp(E)>,
		std::enable_if_t<
			std::is_same<E,R>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		MapProxy<UnaryOp>&& proxy
	)
		noexcept
	{
		return stream.lazy(Map<UnaryOp>{proxy.f});
	}

	template <
		typename STREAM,
		typename UnaryOp,
		typename E = typename STREAM::element_type,
		typename R = std::result_of_t<UnaryOp(E)>,
		std::enable_if_t<
			!std::is_same<E, R>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		MapProxy<UnaryOp>&& proxy
	)
		noexcept
	{
		return Stream<R, Endomorphism<STREAM, UnaryOp>>{ std::vector<R>{}, Endomorphism<STREAM, UnaryOp>{ std::move(stream), std::move(proxy.f) } };
	}

	template <
		typename STREAM,
		typename To
	>
	inline
	auto
	operator >>
	(
		STREAM&& stream,
		MapToProxy<To>
	)
		noexcept
	{
		return Stream<To, MapTo<STREAM, To>>{ std::vector<To>{}, MapTo<STREAM, To>{ std::forward<STREAM>(stream) } };
	}

	template <
		typename STREAM,
		typename BinaryOp
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		AdjacentForEach<BinaryOp> f
	)
		noexcept
	{
		static_assert(
			isAvailableBinaryOp<typename STREAM::element_type, BinaryOp>::value,
			"Can not call BinaryOp with 2 args of STREAM::element_type."
		);

		return stream.lazy(f);
	}

	template <
		typename STREAM,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	Concatenate<STREAM>
	concat(
		STREAM&& stream
	)
		noexcept
	{
		return{ std::forward<STREAM>(stream) };
	}

	template <
		typename STREAM1, typename STREAM2,
		std::enable_if_t<
			!std::is_const<STREAM1>::value && !std::is_const<STREAM2>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	auto
	operator &
	(
		STREAM1&& left,
		STREAM2&& right
	)
		noexcept
	{
		static_assert(
			std::is_same<typename STREAM1::element_type, typename STREAM2::element_type>::value,
			"Call 'concat' with different element_type."
			);

		return left.lazy( concat(std::forward<STREAM2>(right) ) );
	}


	//---------------------//
	// Termination Process //
	//---------------------//

	inline
	Printer
	print_to
	(
		std::ostream& os = std::cout, // default output std::cout
		std::string delim = ", " // default delimiter ", "
	)	{
		return{ os, delim };
	}

	template <
		typename Pred
	>
	inline
	All<Pred>
	all
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>( pred ) };
	}

	template <
		typename Pred
	>
	inline
	Any<Pred>
	any
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}

	template <
		typename Pred
	>
	inline
	None<Pred>
	none
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}


	template <
		typename STREAM, typename UnaryOp,
		typename E = typename STREAM::element_type,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	void
	operator ||
	(
			STREAM&& stream,
			ForEach<UnaryOp>&& f
	)
		noexcept
	{
		static_assert(
			std::is_same<
				void,
				decltype(std::declval<UnaryOp>()(std::declval<E>()))
			>::value,
			"Can not call UnaryOp with an arg of STREAM::element_type."
		);

		f(stream.eval());
		return;
	}

	template <
		typename STREAM,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	void
	operator ||
	(
		STREAM&& stream,
		Printer&& p
	)
	{
		p(stream.eval());
		return;
	}

	template <
		typename STREAM,
		typename Pred
	>
	inline
	bool
	operator ||
	(
		STREAM&& stream,
		All<Pred> all
	)
		noexcept
	{
		static_assert(
			std::is_same<
				bool,
				decltype( std::declval<Pred>()( std::declval<typename STREAM::element_type>() ) )
			>::value,
			"Can not call Pred with an arg of type STREAM::element_type."
		);

		return stream.lazy(all).eval();
	}

	template <
		typename STREAM,
		typename Pred
	>
	inline
	bool
	operator ||
	(
		STREAM&& stream,
		Any<Pred> any
	)
		noexcept
	{
		static_assert(
			std::is_same<
			bool,
			decltype(std::declval<Pred>()(std::declval<typename STREAM::element_type>()))
			>::value,
			"Can not call Pred with an arg of type STREAM::element_type."
			);

		return stream.lazy(any).eval();
	}

	template <
		typename STREAM,
		typename Pred
	>
	inline
	bool
	operator ||
	(
		STREAM&& stream,
		None<Pred> none
	)
		noexcept
	{
		static_assert(
			std::is_same<
			bool,
			decltype(std::declval<Pred>()(std::declval<typename STREAM::element_type>()))
			>::value,
			"Can not call Pred with an arg of type STREAM::element_type."
			);

		return stream.lazy(none).eval();
	}


	//----------------------------//
	// Just Time Evaluate Process //
	//----------------------------//

	template <
		typename STREAM,
		std::enable_if_t<
			!std::is_const<STREAM>::value,
			std::nullptr_t
		> = nullptr
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Repeat&& ev
	)
	{
		for (size_t i{}; i < ev.times; ++i) stream.eval();
		return std::forward<STREAM>( stream );
	}

	template <
		typename STREAM,
		typename E = typename STREAM:: elemtnet_type
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Run
	)
	{
		return Stream<E>{ stream.eval().get() }; // TODO
	}


	//----------------------------//
	// Stream to Range Converters //
	//----------------------------//


	// Converter temporary proxy object
	// Convert stream to any container
	template < class STREAM >
	class ImplicitStreamConverter
	{
		STREAM stream_;
	public:
		ImplicitStreamConverter(STREAM stream) : stream_{ std::forward<STREAM>(stream) } {}

		template < typename Target >
		inline operator Target() const {
			return stream_.convert(ConvertMarker<Target>{});
		}
	};

	// Implicit convert traits
	struct ImplicitStreamConvertInvoker {
		template < typename STREAM >
		static constexpr auto invoke(STREAM&& stream) {
			return ImplicitStreamConverter<STREAM>(std::forward<STREAM>(stream));
		}
	};

	// Explicit convert traits
	template < class Target >
	struct ExplicitStreamConverter {
		template < typename STREAM >
		static constexpr auto convert(STREAM&& stream) {
			return Target{ stream.begin(), stream.end() };
		}
	};


	struct ConvertAny {};

	template < template<class T, class Allocator = std::allocator<T>> class Cont >
	struct ConvertTo {
		template < typename T >
		using type = Cont<T>;
	};

	constexpr ConvertAny convert{};

	template < template<class, class> class Target >
	constexpr auto convert_to = ConvertTo<Target>{};

	inline constexpr ConvertAny converter() noexcept { return{}; }

	template < template<class, class> class Target >
	inline constexpr ConvertTo<Target> converter() noexcept { return{}; }

	// Convert Terminate Operation
	template <
		typename STREAM,
		template<class, class> class Target
	>
		inline constexpr decltype(auto) operator || (STREAM&& stream, ConvertTo<Target>) noexcept {
		return ExplicitStreamConverter<
			typename ConvertTo<Target>::template type<
				  typename std::decay_t<STREAM>::element_type
			>
		>::convert(std::forward<STREAM>(stream.eval()));
	}

	template < typename STREAM >
	inline decltype(auto) operator || (STREAM&& stream, ConvertAny) noexcept {
		return ImplicitStreamConvertInvoker::invoke( std::forward<STREAM>(stream.eval()) );
	}


	//------------------//
	// Operation Assistants //
	//------------------//

	template < typename Func >
	inline decltype(auto) invoker(Func&& f) {
		return Invoker<Func>{ std::forward<Func>(f) };
	}

	template <
		typename STREAM,
		typename Func
	>
	inline
	decltype(auto)
	operator >>
	(
		STREAM&& stream,
		Invoker<Func>&& f
	)
		noexcept
	{
		static_assert(
			std::is_same<
				STREAM,
				decltype( std::declval<Func>()( std::declval<STREAM>() ) )
			>::value,
			"Can not call Func with an arg of type STREAM	or return type is not STREAM."
		);
		return stream.lazy(f);
	}

	template <
		typename STREAM, typename F,
		typename R = decltype(
				std::declval<F>()( std::declval<STREAM>() )
			)
	>
	inline
	R
	operator ||
	(
		STREAM&& stream,
		Invoker<F>&& f
	)	{
		return f(stream.eval());
	}


} // ! namespace stream
} // ! namespace cranberries
#endif

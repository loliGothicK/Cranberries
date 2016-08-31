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


namespace cranberries {
namespace stream_api {
	//--------//
	// detail //
	//--------//

	// Type Transfer
	template < typename Target >
	struct ConvertMarker {};

	// Defaluted Tag
	struct defaulted {};

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


	//-----------------------------//
	// Intermidiate Operation Tree //
	//-----------------------------//


	template <
		typename Op1, // evaluate first
		typename Op2  // evaluate next
	>
	struct OperationTree
	{
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
	// Terminate Operation Root //
	//----------------------------//

	struct RootOperation
	{
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

	// Intermidiate Operation
	template <
		typename Pred
	>
	struct Filter 
	{
		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& data = stream.get();
			for (auto&& iter = data.begin(); iter != data.end(); ) {
				if (pred(*iter)) iter = data.erase(iter);
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
		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			auto&& data = stream.get();
			std::sort(data.begin(), data.end());
			data.erase(std::unique(data.begin(), data.end()), data.end());
			return std::forward<STREAM>( stream );
		}
	};

	// Intermidiate Operation
	struct Skip
	{
		template <
			typename STREAM,
			typename Range = typename std::decay_t<STREAM>::range_type
		>
		inline
		decltype(auto)
		operator()(
			STREAM&& stream
		) {
			auto&& data = stream.get();
			Range{ data.begin() + n, data.end() }.swap(data); // swap trick
			return std::forward<STREAM>( stream );
		}

		// member

		std::size_t n;
	};

	// Intermidiate Operation
	struct Limit
	{
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
			auto&& data = stream.get();
			Range{ data.begin(), data.begin() + n }.swap(data); // swap trick
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
			STREAM&& left
		) {
			once(); // perfectly call once
			auto&& lv = left.get();
			auto&& rv = branch.get();
			if (lv.capacity() < lv.size() + rv.size())
				lv.reserve(lv.size() + rv.size());
			lv.insert(lv.end(), rv.begin(), rv.end());
			return std::forward<STREAM>(left);
		}

		// member

		Branch branch; // Branch Stream
	};

	template < typename Pred >
	struct All
	{
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
		template <
			typename STREAM
		>
		inline
		decltype(auto)
		operator()
		(
			STREAM&& stream
		) {
			return op(stream);
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
	Template parameter T is data type. In Stream, data manageed as vector<T>.
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

		template <
			typename Iterator
		>
		Stream
		(
			Iterator first,
			Iterator last,
			Operation q
		)
			: data{ first,last }
			, operation{ q }
		{}

		Stream
		(
			std::initializer_list<T> init_list,
			Operation q
		)
			: data{ init_list }
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
			: data{ range.begin(), range.end() }
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
			: data{ first,last }
			, operation{ RootOperation{} }
		{}

		Stream
		(
			std::initializer_list<T> init_list
		)
			: data{ init_list }
			, operation{ RootOperation{} }
		{}

		template <
			typename Range
		>
		Stream
		(
			Range&& range
		) 
			: data{ range.begin(), range.end() }
			, operation{ RootOperation{} }
		{}

		Stream() 
			: data{}
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
			return TargetRange{ data.begin(), data.end() };
		}

		// Range wrappers

		inline decltype(auto) begin() { return data.begin(); }

		inline decltype(auto) end() { return data.end(); }

		inline decltype(auto) begin() const { return data.cbegin(); }

		inline decltype(auto) end() const { return data.cend(); }

		inline auto size() { return data.size(); }

		inline void push_back(T const& v) { data.push_back(v); }

		inline void pop_back() { data.pop_back(); }

		inline void shrink_to_fit() { data.shrink_to_fit(); }

		// resource getter
		inline auto& get() { return data; }

		// Operator Registration
		template <
			typename Operator
		>
		inline
		auto
		lazy(
			Operator&& op
		) {
			// make another Stream
			return Stream<
				T, OperationTree<Operation, Operator> 
			>{
				std::move(data), // move resource
			  OperationTree<Operation, Operator>{ operation, op } // make operation tree
			};
		}

		// execute lazy evaluation
		inline
		Stream&
		eval()
		{
			operation(*this); // evaluate operation tree
			return *this;
		}

		// members
	private:

		// data source
		std::vector<T> data{};

		// Operation Tree
		Operation operation{};
	};


	//----------------//
	// Stream Utility //
	//----------------//

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


	//-----------------------//
	// Stream Create Process //
	//-----------------------//

	template <
		typename Iterator,
		typename T = typename Iterator::value_type
	>
	inline
	Stream<T>
	makeStream
	(
		Iterator first,
		Iterator last
	)
		noexcept
	{
		return Stream<T>{ first, last };
	}

	template <
		typename Range,
		typename T = typename Range::value_type
	>
	inline
	Stream<T>
	makeStream
	(
		Range&& range
	)
		noexcept
	{
		return Stream<T>{ range.begin(), range.end() };
	}

	template <
		typename T
	>
	inline
	Stream<T>
	makeStream
	(
		std::initializer_list<T> li
	)
		noexcept
	{
		return Stream<T>{ li };
	}

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

	static constexpr Creator to_stream{};

	template <
		typename T
	>
	inline
	auto
	operator /
	(
		T&& v,
		Creator
	)
		noexcept
	{
		return makeStream(std::forward<T>(v));
	}

	template <
		typename T
	>
	inline
	auto
	operator /
	(
		std::initializer_list<T> li,
		Creator
	)
		noexcept
	{
		return makeStream( std::move(li) );
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

	inline Skip skip(std::size_t n)	{	return{ n }; }

	inline Limit limit(std::size_t n)	{ return{ n }; }

	template < typename T >
	inline PushBack<std::decay_t<T>> push_back(T&& v) noexcept { return{ v };	}


	inline PopBack pop_back()	noexcept {	return{};	}

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
		typename STREAM,
		typename UnaryOp
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
			isAvailableUnaryOp<typename STREAM::element_type, UnaryOp>::value,
			"Can not call UnaryOp with an arg of STREAM::element_type."
		);

		return stream.lazy(f);
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
			std::is_same<typename STREAM1::element_type, typename STREAM2::element_type>::value
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
	{
		return{ std::forward<Pred>( pred ) };
	}

	template <
		typename STREAM, typename UnaryOp,
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
				decltype(std::declval<UnaryOp>()(std::declval<typename STREAM::element_type>()))
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

		return all( stream.eval() );
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
		ImplicitStreamConverter(STREAM stream) : stream_{ stream } {}

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
		>::convert(stream.eval());
	}

	template < typename STREAM >
	inline decltype(auto) operator || (STREAM&& stream, ConvertAny) noexcept {
		return ImplicitStreamConvertInvoker::invoke( stream.eval() );
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
	inline decltype(auto) operator >>
	(
		STREAM&& stream,
		Invoker<Func>&& f
	) noexcept 
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
	)
		noexcept
	{
		return f(stream.eval());
	}


} // ! namespace stream
} // ! namespace cranberries
#endif
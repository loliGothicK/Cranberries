#ifndef CRANBERRIES_STREAMS_OPERATIONS_HPP
#define CRANBERRIES_STREAMS_OPERATIONS_HPP
#include <utility>
#include <string>
#include <type_traits>
#include "detail/tag.hpp"


namespace cranberries {
namespace streams {

	//----------------------//
	// Intermediate process //
	//----------------------//


	inline operators::Splitter split(std::string s) noexcept { return{ s }; }

	template <
		typename Pred
	>
	inline
	operators::Filter<Pred>
	filtered
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}

	inline operators::Sort<> sorted() noexcept { return{}; }

	template <
		typename Pred
	>
	inline
	operators::Sort<Pred>
	sorted
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}

	inline operators::DistinctProxy distinctly() noexcept { return{}; }

	inline operators::Drop dropped(std::size_t n)	noexcept { return{ n }; }

	inline operators::Take taken(std::size_t n) noexcept { return{ n }; }

	template < typename T >
	inline operators::PushBack<std::decay_t<T>> push_back(T&& v) noexcept { return{ v }; }

	inline operators::PopBack pop_back()	noexcept { return{}; }

	template < typename To >
	inline operators::MapToProxy<To> map_to() noexcept { return{}; }

	inline operators::MapToProxy<std::string> to_string() noexcept { return{}; }

	inline operators::Repetition repeat(size_t times = 1) noexcept { return{ times }; }

	inline operators::Run run() noexcept { return{}; }

	template <
		typename BinaryFunc
	>
	inline
	operators::AdjacentForEach<BinaryFunc>
	adjacent_for_each
	(
		BinaryFunc&& f
	)
		noexcept
	{
		return{ std::forward<BinaryFunc>(f) };
	}

	template <
		typename UnaryFunc
	>
	inline
	operators::ForEach<UnaryFunc>
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
	operators::Peek<UnaryFunc>
	peeked
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
	operators::MapProxy<UnaryFunc>
	mapped
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
	operators::Transform<UnaryFunc>
	transformed
	(
		UnaryFunc&& func
	)
		noexcept
	{
		return{ std::forward<UnaryFunc>(func) };
	}

	inline operators::UniqueProxy uniquely() noexcept { return{}; }
	
	template <
		typename Stream,
		std::enable_if_t<
			detail::is_infinite_stream_v<Stream>
			|| detail::is_finite_stream_v<Stream>,
			std::nullptr_t
		> = nullptr
	>
	inline
	operators::Merge<Stream>
	merged
	(
		Stream&& stream_
	)
		noexcept
	{
		return{ std::forward<Stream>(stream_) };
	}



	inline
	operators::Printer
	print_to
	(
		std::ostream& os = std::cout, // default output std::cout
		std::string delim = ", " // default delimiter ", "
	) {
		return{ os, delim };
	}

	template <
		typename Pred
	>
	inline
	operators::All<Pred>
	all
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
	operators::Any<Pred>
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
	operators::None<Pred>
	none
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
	operators::TakeWhile<Pred>
	taken_while
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
	operators::DropWhile<Pred>
	dropped_while
	(
		Pred&& pred
	)
		noexcept
	{
		return{ std::forward<Pred>(pred) };
	}

	//----------------------//
	// Operation Assistants //
	//----------------------//

	template <
		typename Func
	>
	inline
	decltype(auto)
	invoked(
		Func&& f
	)
		noexcept
	{
		return operators::Iinvoker<Func>{ std::forward<Func>(f) };
	}

	template <
		typename Func
	>
	inline
	decltype(auto)
	invoker(
		Func&& f
	)
		noexcept
	{
		return operators::Tinvoker<Func>{ std::forward<Func>(f) };
	}




} // ! namespace stream
} // ! namespace cranberries

#endif
#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_CHUNK_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_CHUNK_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <array>
#include "../../common/concepts.hpp"
#include "../../span.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < size_t, class, class = void >
class Chunk;

template < size_t N, class Range >
class Chunk<N, Range, void>
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		sentinel_impl(Range traversal)
			: traversal_{ traversal }
			, iter{ cranberries::back_magic::begin(traversal_) }
			, last{ cranberries::back_magic::end(traversal_) }
			, chunk{}
		{
			chunk[0] = *iter;
			for (size_t i{ 1 }; i < N; ++i) {
				if(++iter != last)
					{ chunk[i] = *iter; }
				else break;
			}
		}

		auto get() { return chunk; }
		void next() { 
			for (auto& e : chunk) {
				if(++iter != last)
					{ e = *iter; }
				else break;
				}
		}
		bool is_end() { return iter == last ; }
	private:
		Range traversal_;
		typename std::decay_t<Range>::iterator iter;
		typename std::decay_t<Range>::iterator last;
		std::array<value_type, N> chunk;
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Chunk(Range traversal)
		: traversal{ traversal } {}

	iterator begin() const { return { std::make_unique<sentinel>(traversal) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
};



template < class Range >
class Chunk<-1, Range, bool_pack<true>>
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		template < class Traversal_ >
		sentinel_impl(Traversal_&& traversal, size_t n)
			: traversal_{ std::forward<Traversal_>(traversal) }
			, n_{n}
		{
			span = ::cranberries::experimental::readonly_span(traversal_, 0, n);
;		}

		auto get() { return span; }
		void next() {
			span.next_segment(n_);
		}
		bool is_end() { return !span.has_next_segment(); }
	private:
		Range traversal_;
		size_t n_;
		readonly_span_t<Range> span;
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Chunk(Range traversal, size_t n)
		: traversal{ traversal }
		, n(n)
	{}

	iterator begin() const { return { std::make_unique<sentinel>(traversal, n) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	size_t n;
};

template < class Range >
class Chunk<-1, Range, bool_pack<false, true>>
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		template < class Traversal_ >
		sentinel_impl(Traversal_&& traversal, size_t n)
			: seq( ::cranberries::back_magic::size(traversal) )
			, n_{ n }
			, span{}
		{
			::cranberries::index_for_each(traversal,
				[&](size_t i, auto&& item) { seq[i] = item; }
			);
			span = ::cranberries::experimental::readonly_span(seq, 0, n);
		}

		auto get() { return span; }
		void next() {
			span.next_segment(n_);
		}
		bool is_end() { return !span.has_next_segment(); }
	private:
		std::vector<value_type> seq;
		size_t n_;
		readonly_span_t<std::vector<value_type>> span;
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Chunk(Range traversal, size_t n)
		: traversal{ traversal }
		, n(n)
	{}

	iterator begin() const { return { std::make_unique<sentinel>(traversal, n) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	size_t n;
};

template < class Range >
class Chunk<-1, Range, bool_pack<false, false>>
	: private tag::sentinel_traversal_tag
{
	class sentinel_impl {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
		template < class Traversal_ >
		sentinel_impl(Traversal_&& traversal, size_t n)
			: seq{}
			, n_{ n }
			, span{}
		{
			for (auto&& e : traversal) seq.emplace_back(std::forward<decltype(e)>(e));
			span = ::cranberries::experimental::readonly_span(seq, 0, n);
		}

		auto get() { return span; }
		void next() {
			span.next_segment(n_);
		}
		bool is_end() { return !span.has_next_segment(); }
	private:
		std::vector<value_type> seq;
		size_t n_;
		readonly_span_t<std::vector<value_type>> span;
	};
public:
	using value_type = typename std::decay_t<Range>::value_type;
	using sentinel = sentinel_impl;
	using iterator = sentinel_iterator<sentinel>;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	Chunk(Range traversal, size_t n)
		: traversal{ traversal }
		, n(n)
	{}

	iterator begin() const { return { std::make_unique<sentinel>(traversal, n) }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const { return {}; }

private:
	Range traversal;
	size_t n;
};



template < size_t N >
class ChunkProxy
	: private tag::adaptor_proxy_tag
{
public:
	ChunkProxy() = default;

	template < class Traversable >
	using requires
		= typename concepts::iterable::template requires<Traversable>;

	template < class Range >
	Chunk<N, Range> operator()(Range&& traversal) {
		return { std::forward<Range>(traversal) };
	}
};

template < >
class ChunkProxy<-1>
	: private tag::adaptor_proxy_tag
{
	size_t n;
public:
	ChunkProxy(size_t n) : n{ n } {}

	template < class Traversable >
	using requires
		= typename concepts::iterable::template requires<Traversable>;

	template < class T >
	using enable_span = decltype(::cranberries::experimental::span(std::declval<T&>()));

	template < class Range,
		std::enable_if_t<is_satisfied_v<enable_span, Range>, std::nullptr_t> = nullptr >
	Chunk<-1, Range, bool_pack<true>> operator()(Range&& traversal) {
		return { std::forward<Range>(traversal), n };
	}

	template < class Range,
		std::enable_if_t<!is_satisfied_v<enable_span, Range> && concepts::required_v<Range, concepts::sized>, std::nullptr_t> = nullptr >
		Chunk<-1, Range, bool_pack<false, true>> operator()(Range&& traversal) {
		return { std::forward<Range>(traversal), n };
	}

	template < class Range,
		std::enable_if_t<!is_satisfied_v<enable_span, Range> && !concepts::required_v<Range, concepts::sized>, std::nullptr_t> = nullptr >
		Chunk<-1, Range, bool_pack<false, false>> operator()(Range&& traversal) {
		return { std::forward<Range>(traversal), n };
	}

};


namespace view {
	template < size_t N >
	ChunkProxy<N> chunk() { return {}; }

	inline ChunkProxy<-1> chunk(size_t n) { return { n }; }
}

}}}
#endif
#ifndef CRANBERRIES_SPAN_HPP
#define CRANBERRIES_SPAN_HPP
//#pragma once
#include "./common/concepts.hpp"
#include "./common/exception.hpp"

namespace cranberries {
inline namespace experimental {

	enum class permission {
		read_write, read_only,
	};

	class span_tag{};

	namespace segment {

		template < class Sequence, permission = permission::read_write >
		struct segment_traits {
			using value_type = decltype(*(::cranberries::back_magic::begin(std::declval<Sequence>())));
			using reference_type = value_type&;
			using traversable_type = decltype(::cranberries::back_magic::begin(std::declval<Sequence>()));
		};

		template < class Sequence >
		struct segment_traits<Sequence, permission::read_only> {
			using value_type = std::add_const_t<std::add_lvalue_reference_t<remove_cvr_t<decltype(*(::cranberries::back_magic::begin(std::declval<Sequence>())))>>>;
			using reference_type = const value_type&;
			using traversable_type = decltype(::cranberries::back_magic::cbegin(std::declval<Sequence>()));
		};

		template < class T >
		struct segment_traits<T*, permission::read_write> {
			using value_type = T;
			using reference_type = T&;
			using traversable_type = T*;
		};

		template < class T >
		struct segment_traits<T*, permission::read_only> {
			using value_type = const T;
			using reference_type = const T&;
			using traversable_type = const T*;
		};


	
		template < class Sequence, permission Permission = permission::read_write >
		struct segment_t
		{
			// types
			using value_type = typename segment_traits<Sequence, Permission>::value_type;
			using traversable_type = typename segment_traits<Sequence, Permission>::traversable_type;
			using const_traversable_type = typename segment_traits<Sequence, permission::read_only>::traversable_type;
			using reference_type = typename segment_traits<Sequence, Permission>::reference_type;

			// members
			traversable_type traversable_;
			size_t count_;

			//// index access
			//reference_type operator[](size_t n) const {
			//	return count_ > n ? *std::next(traversable_, n) : CRANBERRIES_OUT_OF_RANGE_THROW_WITH_MSG("Span out of segment");
			//}

			// range access
			traversable_type begin() { return traversable_; }
			traversable_type end() { return std::next(traversable_, count_); }
			const_traversable_type cbegin() const { return traversable_; }
			const_traversable_type cend() const { return std::next(traversable_, count_); }

		};


	}
	template < class, permission = permission::read_write > class span_t;

	template < class Sequence, permission Per >
	class span_t : private span_tag {
		segment::segment_t<Sequence, Per> segment_;
		size_t offset, upper_bound;
	public:
		using value_type = typename segment::segment_t<Sequence, Per>::value_type;

		span_t() = default;
		span_t(span_t const&) = default;
		span_t& operator=(span_t const&) = default;

		template < class Traversable >
		span_t(Traversable& source, size_t offset, size_t count) noexcept
			: segment_{ std::next(::cranberries::back_magic::begin(source), offset), count }
			, offset{ offset }
			, upper_bound{ ::cranberries::back_magic::size(source) }
		{}

		auto begin() noexcept { return segment_.begin(); }
		auto end() noexcept { return segment_.end(); }
		auto begin() const noexcept { return segment_.cbegin(); }
		auto end() const noexcept { return segment_.cend(); }

		size_t size() const noexcept { return segment_.count_; }

		bool has_next_segment() noexcept {
			return offset < upper_bound;
		}

		operator bool() noexcept { return has_next_segment(); }

		auto next_segment(size_t n) {
			offset += segment_.count_;
			std::advance(segment_.traversable_, segment_.count_);
			segment_.count_ = n < upper_bound - offset ? n : upper_bound - offset;
		}

	};

	template < class T, permission Per >
	class span_t<T*, Per> : private span_tag {
		segment::segment_t<T*, Per> segment_;
		size_t offset;
	public:
		using value_type = typename segment::segment_t<T*, Per>::value_type;

		span_t() = default;
		span_t(span_t const&) = default;
		span_t& operator=(span_t const&) = default;

		span_t(T* source, size_t offset, size_t count) noexcept
			: segment_{ std::next(source, offset), count }
			, offset{ offset }
		{}

		decltype(auto) begin() noexcept { return segment_.begin(); }
		decltype(auto) end() noexcept { return segment_.end(); }
		size_t size() const noexcept { return segment_.count_; }

		bool has_next_segment() noexcept {
			return false;
		}

		operator bool() noexcept { return false; }

	};

	template < class T >
	using readonly_span_t = span_t<T, permission::read_only>;


	template < class Span1, class Span2 >
	std::enable_if_t<conjunction_v<
		std::is_base_of<span_tag, Span1>,
		std::is_base_of<span_tag, Span2>,
		::cranberries::is_equality_comparable_to<
		typename std::decay_t<Span1>::value_type,
		typename std::decay_t<Span2>::value_type>>,
	bool>
	operator==(Span1 const& lhs, Span2 const& rhs) {
		if (lhs.size() != rhs.size()) return false;

		auto iter1 = ::cranberries::back_magic::begin(lhs);
		auto iter2 = ::cranberries::back_magic::begin(rhs);
		auto last1 = ::cranberries::back_magic::end(lhs);
		auto last2 = ::cranberries::back_magic::end(rhs);
		for(;iter1 != last1 && iter2 != last2; ++iter1, ++iter2)
			if ( !(*iter1 == *iter2) ) return false;
		return true;
	}

	template < class Span1, class Span2 >
	std::enable_if_t<conjunction_v<
		std::is_base_of<span_tag, Span1>,
		std::is_base_of<span_tag, Span2>,
		::cranberries::is_equality_comparable_to<
		typename std::decay_t<Span1>::value_type,
		typename std::decay_t<Span2>::value_type>>,
	bool>
	operator!=(Span1 const& lhs, Span2 const& rhs) {
		if (lhs.size() != rhs.size()) return true;

		auto iter1 = lhs.begin();
		auto iter2 = ::cranberries::back_magic::begin(rhs);
		auto last1 = ::cranberries::back_magic::end(lhs);
		auto last2 = ::cranberries::back_magic::end(rhs);
		for (; iter1 != last1 && iter2 != last2; ++iter1, ++iter2)
			if (*iter1 == *iter2) return false;
		return true;
	}


	template < class Traversable,
		::cranberries::concepts::concept_required<Traversable, ::cranberries::concepts::iterable> = nullptr,
		::cranberries::concepts::concept_required<decltype(*(::cranberries::back_magic::begin(std::declval<Traversable>()))), ::cranberries::concepts::writable> = nullptr >
	auto span(Traversable& trv, size_t offset, size_t length) {
		return span_t<Traversable>{ trv, offset, length };
	}

	template < class Traversable,
		::cranberries::concepts::concept_required<Traversable, ::cranberries::concepts::iterable> = nullptr,
		::cranberries::concepts::concept_required_not<decltype(*(::cranberries::back_magic::begin(std::declval<Traversable>()))), ::cranberries::concepts::writable> = nullptr >
	auto span(Traversable& trv, size_t offset, size_t length) {
		return readonly_span_t<Traversable>{ trv, offset, length };
	}


	template < class Traversable,
		::cranberries::concepts::concept_required<Traversable, ::cranberries::concepts::iterable> = nullptr >
	auto readonly_span(Traversable& trv, size_t offset, size_t length) {
		return readonly_span_t<Traversable>{ trv, offset, length };
	}


}
}


#endif

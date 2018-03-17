#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_OF_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_OF_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include <fstream>
#include <iostream>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Range, class = void >
class RangeWrapper
	: private tag::sentinel_traversal_tag
	, public reversable_traversal
{
	friend struct view_get;
public:
	using value_type = typename remove_cvr_t<Range>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using finite_ = std::nullptr_t;
	using immutable_ = std::nullptr_t;

	explicit RangeWrapper(Range traversal): traversal( traversal ) {}
private:
	class forward_view final : public polymorphic_view<value_type>
	{
		decltype(::cranberries::back_magic::cbegin(std::declval<const Range&>())) iter;
		decltype(::cranberries::back_magic::cend(std::declval<const Range&>())) last;
	public:
		template < class Iterator >
		forward_view(Iterator first, Iterator last)
			: iter{ first }
			, last{ last }
		{}
		forward_view(const forward_view&) = default;

		virtual value_type get() const override final { return *iter; }
		virtual void next() override final { ++iter; }
		virtual bool is_end() const override final { return iter == last; }
	};
	class reverse_view final : public polymorphic_view<value_type>
	{
		decltype(::cranberries::back_magic::crbegin(std::declval<const Range&>())) iter;
		decltype(::cranberries::back_magic::crend(std::declval<const Range&>())) last;
	public:
		template < class Iterator >
		reverse_view(Iterator first, Iterator last)
			: iter{ first }
			, last{ last }
		{}
		reverse_view(const reverse_view&) = default;

		virtual value_type get() const override final { return *iter; }
		virtual void next() override final { ++iter; }
		virtual bool is_end() const override final { return iter == last; }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<forward_view>(
			::cranberries::back_magic::cbegin(traversal),
			::cranberries::back_magic::cend(traversal)
			);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<reverse_view>(
			::cranberries::back_magic::crbegin(traversal),
			::cranberries::back_magic::crend(traversal)
			);
	}
public:
	iterator begin() { return { this->fwd() }; }
	iterator end() { return {}; }
	iterator rbegin() { return { this->rev() }; }
	iterator rend() { return {}; }
private:
	Range traversal;
};

template < class Range >
class RangeWrapper<Range, void_t<decltype(back_magic::crbegin(std::declval<Range>()))>>
	: private tag::sentinel_traversal_tag
	, public forward_traversal
{
	friend struct view_get;
public:
	using value_type = typename remove_cvr_t<Range>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using finite_ = std::nullptr_t;
	using immutable_ = std::nullptr_t;

	explicit RangeWrapper(Range traversal): traversal(traversal) {}
private:
	class forward_view final : public polymorphic_view<value_type>
	{
		typename remove_cvr_t<Range>::const_iterator iter;
		typename remove_cvr_t<Range>::const_iterator last;
	public:
		template < class Iterator >
		forward_view(Iterator first, Iterator last)
			: iter{ first }
			, last{ last }
		{}
		forward_view(const forward_view&) = default;

		virtual value_type get() const override final { return *iter; }
		virtual void next() override final { ++iter; }
		virtual bool is_end() const override final { return iter == last; }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<forward_view>(
			::cranberries::back_magic::cbegin(traversal),
			::cranberries::back_magic::cend(traversal)
			);
	}
public:
	iterator begin() { return { this->fwd() }; }
	iterator end() { return {}; }
private:
	Range traversal;
};


namespace create {
	template < class Range >
	auto of(Range&& traversal) { return RangeWrapper<Range>{ std::forward<Range>(traversal) }; }

	template < class T >
	auto of(std::initializer_list<T> il) { return RangeWrapper<std::initializer_list<T>>{ il }; }
}

}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_GENERATE_HPP

#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_REPLACE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_REPLACE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include "../../common/concepts.hpp"
#include "../../functional.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class, class >
class Replaced;

template < class Range >
class Replaced<Range, bool_pack<true>>
	: private tag::sentinel_traversal_tag
{
public:
	using value_type = typename std::decay_t<Range>::value_type;
private:
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
	private:
		Range traversal_;
		value_type from, to;
		typename std::decay_t<Range>::iterator iter, last;
	public:
		forward_sentinel(Range traversal, value_type from, value_type to) noexcept
			: traversal_{ traversal }
			, from{ from }
			, to{ to }
			, iter{ ::cranberries::back_magic::begin(traversal_) }
			, last{ ::cranberries::back_magic::end(traversal_) }
		{}

		forward_sentinel(const forward_sentinel&) noexcept {}

		auto get() noexcept { return *iter != from ? *iter : to; }
		void next() noexcept { ++iter; }
		bool is_end() const noexcept { return iter == last; }
	};

public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using constraints
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;


	Replaced(Range traversal, value_type from, value_type to) noexcept
		: traversal { traversal  }
		, from{ from }
		, to{ to  }
	{}

	iterator begin() const noexcept { return { std::make_unique<forward_sentinel>(traversal, from, to) }; }
	iterator end() const noexcept { return {}; }
	reverse_iterator rbegin() const noexcept { return { std::make_unique<reverse_sentinel>(*this) }; }
	reverse_iterator rend() const noexcept { return {}; }

private:
	Range traversal;
	value_type from, to;
};


template < class Range >
class Replaced<Range, bool_pack<false>>
	: private tag::sentinel_traversal_tag
{
	using value_type = typename std::decay_t<Range>::value_type;
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
	private:
		Range traversal_;
		value_type from, to;
		typename std::decay_t<Range>::iterator iter, last;
	public:
		forward_sentinel(Range traversal, value_type from, value_type to) noexcept
			: traversal_{ traversal }
			, from{ from }
			, to{ to }
			, iter{ ::cranberries::back_magic::begin(traversal_) }
			, last{ ::cranberries::back_magic::end(traversal_) }
		{}

		forward_sentinel(const forward_sentinel&) noexcept {}

		auto get() noexcept { return *iter != from ? *iter : to; }
		void next() noexcept { ++iter; }
		bool is_end() const noexcept { return iter == last; }
	};

public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;


	Replaced(Range traversal, value_type from, value_type to) noexcept
		: traversal { traversal  }
		, from { from }
		, to { to }
	{}

	iterator begin() const noexcept { return { std::make_unique<forward_sentinel>(traversal, from, to) }; }
	iterator end() const noexcept { return {}; }
	reverse_iterator rbegin() const noexcept { return std::make_unique<reverse_iterator>(*this); }
	reverse_iterator rend() const noexcept { return {}; }

private:
	Range traversal;
	value_type from, to;
};


template < class, class, class >
class ReplacedIf;


template < class Range, class Pred >
class ReplacedIf<Range, bool_pack<true>, Pred>
	: private tag::sentinel_traversal_tag
{
public:
	using value_type = typename std::decay_t<Range>::value_type;
private:
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
	private:
		Range traversal_;
		Pred pred;
		value_type to;
		typename std::decay_t<Range>::iterator iter, last;
	public:
		forward_sentinel(Range traversal, Pred pred, value_type to) noexcept
			: traversal_{ traversal }
			, pred{ pred }
			, to{ to }
			, iter{ ::cranberries::back_magic::begin(traversal_) }
			, last{ ::cranberries::back_magic::end(traversal_) }
		{}

		forward_sentinel(const forward_sentinel&) noexcept {}

		auto get() const noexcept { return pred(*iter) ? to : *iter; }
		void next() noexcept { ++iter; }
		bool is_end() const noexcept { return iter == last; }
	};

	class reverse_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
	private:
		Range traversal_;
		Pred pred;
		value_type to;
		typename std::decay_t<Range>::iterator iter, last;
	public:
		reverse_sentinel(Range traversal, Pred pred, value_type to) noexcept
			: traversal_{ traversal }
			, pred{ pred }
			, to{ to }
			, iter{ ::cranberries::back_magic::rbegin(traversal_) }
			, last{ ::cranberries::back_magic::rend(traversal_) }
		{}

		reverse_sentinel(const reverse_sentinel&) noexcept {}

		auto get() const noexcept { return pred(*iter) ? to : *iter; }
		void next() noexcept { ++iter; }
		bool is_end() const noexcept { return iter == last; }
	};


public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = reverse_sentinel;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < typename Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using constraints
		= std::enable_if_t<
		concepts::required_v<Range, concepts::iterable>
		>;


	ReplacedIf(Range traversal, Pred pred, value_type to) noexcept
		: traversal{ traversal }
		, pred{ pred }
		, to{ to }
	{}

	iterator begin() const noexcept { return { std::make_unique<forward_sentinel>(traversal, pred, to) }; }
	iterator end() const noexcept { return {}; }
	reverse_iterator rbegin() const noexcept { return { std::make_unique<reverse_sentinel>(traversal, pred, to) }; }
	reverse_iterator rend() const noexcept { return {}; }

private:
	Range traversal;
	Pred pred;
	value_type to;
};


template < class Range, class Pred >
class ReplacedIf<Range, bool_pack<false>, Pred>
	: private tag::sentinel_traversal_tag
{
	using value_type = typename std::decay_t<Range>::value_type;
	class forward_sentinel {
	public:
		using value_type = typename std::decay_t<Range>::value_type;
	private:
		Range traversal_;
		Pred pred;
		value_type to;
		typename std::decay_t<Range>::iterator iter, last;
	public:
		forward_sentinel(Range traversal, Pred pred, value_type to) noexcept
			: traversal_{ traversal }
			, pred{ pred }
			, to{ to }
			, iter{ ::cranberries::back_magic::begin(traversal_) }
			, last{ ::cranberries::back_magic::end(traversal_) }
		{}

		forward_sentinel(const forward_sentinel&) noexcept {}

		auto get() const noexcept { return pred(*iter) ? to : *iter; }
		void next() noexcept { ++iter; }
		bool is_end() const noexcept { return iter == last; }
	};

public:
	using forward_sentinel = forward_sentinel;
	using reverse_sentinel = magic_arts::default_reverse_sentinel<value_type>;
	using iterator = sentinel_iterator<forward_sentinel>;
	using reverse_iterator = sentinel_iterator<reverse_sentinel>;

	template < class Range, typename ElemType = typename std::decay_t<Range>::value_type >
	using requires
		= std::enable_if_t<conjunction_v<
			concepts::required<Range, concepts::iterable>,
			is_callable<Pred(ElemType), bool>,
			std::is_convertible<value_type, ElemType>
		>>;

	ReplacedIf(Range traversal, Pred pred, value_type to) noexcept
		: traversal{ traversal }
		, pred{ pred }
		, to{ to }
	{}

	iterator begin() const noexcept { return { std::make_unique<forward_sentinel>(traversal, preds, to) }; }
	iterator end() const noexcept { return {}; }
	reverse_iterator rbegin() const noexcept { return std::make_unique<reverse_iterator>(*this); }
	reverse_iterator rend() const noexcept { return {}; }

private:
	Range traversal;
	Pred pred;
	value_type to;
};

namespace view {
	template < typename Range >
	Replaced<Range, bool_pack<concepts::required_v<Range, concepts::reverse_iterable>>>
		replace(Range traversal, typename std::decay_t<Range>::value_type const& from, typename std::decay_t<Range>::value_type const& to) noexcept
	{
		return { std::forward<Range>(traversal), from, to };
	}

	template < class T >
	auto replace(T from, T to) noexcept
	{ 
		return make_proxy<Replaced, pred_sequence<bind_1st<concepts::required, concepts::reverse_iterable>::template expr>>::forward( from, to );
	}


	template < typename Range, class Pred >
	ReplacedIf<Range, bool_pack<concepts::required_v<Range, concepts::reverse_iterable>>, Pred>
		replace_if(Range traversal, Pred&& pred, typename std::decay_t<Range>::value_type const& to) noexcept
	{
		return { std::forward<Range>(traversal), std::forward<Pred>(pred), to };
	}

	template < class Pred, class T >
	auto replace_if(Pred&& pred, T to) noexcept
	{
		return make_proxy<ReplacedIf, pred_sequence<bind_1st<concepts::required, concepts::reverse_iterable>::template expr>>::piecewise(std::forward<Pred>(pred))(to);
	}
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

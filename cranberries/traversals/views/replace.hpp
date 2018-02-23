#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_REPLACE_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_REPLACE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/default_sentinel.hpp"
#include "../../common/concepts.hpp"
#include "../../functional/relational.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class, bool = true >
class Replaced;

template < class Traversal >
class Replaced<Traversal, true>
	: private tag::sentinel_traversal_tag
	, private reversable_traversal
	, public _detail::size_impl_injection< Replaced<Traversal, true>, Traversal >
{
	friend struct view_get;
	template <class, class, class> friend struct _detail::size_impl_injection;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	Replaced(Traversal traversal, value_type from, value_type to) noexcept
		: traversal { traversal  }
		, from{ from }
		, to{ to  }
	{}
	Replaced(Replaced const&) = delete;
	Replaced& operator=(Replaced const&) = delete;
	Replaced(Replaced&&) = default;
	Replaced& operator=(Replaced&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		value_type from;
		value_type to;
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view, value_type from, value_type to)
			: view{ std::move(_view) }
			, from{ from }
			, to{ to }
		{}
		value_type get() const override final { return view->get() == from ? to: view->get(); }
		void next() override final { view->next(); }
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), from, to);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<view_>(view_get::rev(traversal), from, to);
	}

public:
	iterator begin() const noexcept { return { this->fwd() }; }
	iterator end() const noexcept { return {}; }
	reverse_iterator rbegin() const noexcept { return { this->rev() }; }
	reverse_iterator rend() const noexcept { return {}; }
private:
	Traversal traversal;
	value_type from, to;
};


template < class Traversal >
class Replaced<Traversal, false>
	: private tag::sentinel_traversal_tag
	, private forward_traversal
{
	friend struct view_get;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	Replaced(Traversal traversal, value_type from, value_type to) noexcept
		: traversal { traversal  }
		, from{ from }
		, to{ to  }
	{}
	Replaced(Replaced const&) = delete;
	Replaced& operator=(Replaced const&) = delete;
	Replaced(Replaced&&) = default;
	Replaced& operator=(Replaced&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		value_type from;
		value_type to;
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view, value_type from, value_type to)
			: view{ std::move(_view) }
			, from{ from }
			, to{ to }
		{}
		value_type get() const override final { return view->get() == from ? to: view->get(); }
		void next() override final { view->next(); }
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), from, to);
	}

public:
	iterator begin() const noexcept { return { this->fwd() }; }
	iterator end() const noexcept { return {}; }
private:
	Traversal traversal;
	value_type from, to;
};


template < class, class, bool = true >
class ReplacedIf;

template < class Traversal, class Pred >
class ReplacedIf<Traversal, Pred, true>
	: private tag::sentinel_traversal_tag
	, private reversable_traversal
	, public _detail::size_impl_injection< ReplacedIf<Traversal, Pred, true>, Traversal >
{
	friend struct view_get;
	template <class, class, class> friend struct _detail::size_impl_injection;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	ReplacedIf(Traversal traversal, Pred pred, value_type to) noexcept
		: traversal { traversal  }
		, pred{ pred }
		, to{ to  }
	{}
	ReplacedIf(ReplacedIf const&) = delete;
	ReplacedIf& operator=(ReplacedIf const&) = delete;
	ReplacedIf(ReplacedIf&&) = default;
	ReplacedIf& operator=(ReplacedIf&&) = default;
private:
	class view_ : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		Pred pred;
		value_type to;
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view, Pred pred, value_type to)
			: view{ std::move(_view) }
			, pred{ pred }
			, to{ to }
		{}
		value_type get() const override final { return pred( view->get() ) ? to: view->get(); }
		void next() override final { view->next(); }
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), pred, to);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<view_>(view_get::rev(traversal), pred, to);
	}

public:
	iterator begin() const noexcept { return { this->fwd() }; }
	iterator end() const noexcept { return {}; }
	reverse_iterator rbegin() const noexcept { return { this->rev() }; }
	reverse_iterator rend() const noexcept { return {}; }
private:
	Traversal traversal;
	Pred pred;
	value_type to;
};


template < class Traversal, class Pred >
class ReplacedIf<Traversal, Pred, false>
	: private tag::sentinel_traversal_tag
	, private forward_traversal
{
	friend struct view_get;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	ReplacedIf(Traversal traversal, Pred pred, value_type to) noexcept
		: traversal { traversal  }
		, pred{ pred }
		, to{ to  }
	{}
	ReplacedIf(ReplacedIf const&) = delete;
	ReplacedIf& operator=(ReplacedIf const&) = delete;
	ReplacedIf(ReplacedIf&&) = default;
	ReplacedIf& operator=(ReplacedIf&&) = default;
private:
	class view_ : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		Pred pred;
		value_type to;
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view, Pred pred, value_type to)
			: view{ std::move(_view) }
			, pred{ pred }
			, to{ to }
		{}
		value_type get() const override final { return pred( view->get() ) ? to: view->get(); }
		void next() override final { view->next(); }
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), pred, to);
	}
public:
	iterator begin() const noexcept { return { this->fwd() }; }
	iterator end() const noexcept { return {}; }
private:
	Traversal traversal;
	Pred pred;
	value_type to;
};

namespace _proxy {
	template < class T >
	struct ReplaceX: tag::adaptor_tag {
		T from;
		T to;
	public:
		ReplaceX(T from, T to) : from{ from }, to{to} {}

		template < class Trav >
		auto operator()(Trav&& trav) { return Replaced<Trav, std::is_base_of<reversable_traversal, Trav>::value>{
			std::forward<Trav>(trav), from, to
		}; }
	};

	template < class Pred, class T >
	struct ReplaceIfX: tag::adaptor_tag {
		Pred pred;
		T to;
	public:
		template < class Pred_ >
		ReplaceIfX(Pred_&& pred, T to) : pred{ std::forward<Pred_>(pred) }, to{to} {}

		template < class Trav >
		auto operator()(Trav&& trav) { return ReplacedIf<Trav, Pred, std::is_base_of<reversable_traversal, Trav>::value>{
			std::forward<Trav>(trav), pred, to
		}; }
	};
}

namespace view {
	template < typename T >
	auto replace(T const& from, T const& to) noexcept
	{
		return _proxy::ReplaceX<T>{ from, to };
	}

	template < class Pred, class T >
	auto replace_if(Pred&& pred, T to) noexcept
	{
		return _proxy::ReplaceIfX<Pred, T>{ std::forward<Pred>(pred), to };
	}
}



}}}
#endif // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

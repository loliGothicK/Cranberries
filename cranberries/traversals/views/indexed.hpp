#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_INDEXED_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../common/concepts.hpp"
#include "../detail/default_sentinel.hpp"

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Traversal, bool = true >
class Indexed
	: private tag::sentinel_traversal_tag
	, private reversable_traversal
	, public _detail::size_impl_injection<Indexed<Traversal,true>, Traversal>
{
	friend struct view_get;
	template <class, class> friend struct _detail::size_impl_injection_for_drop;
public:
	using value_type = typename std::tuple<size_t, typename remove_cvr_t<Traversal>::value_type>;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	Indexed(Traversal traversal, size_t n)
		: traversal{ traversal }
		, index(n) {}
	Indexed(Indexed const&) = delete;
	Indexed& operator=(Indexed const&) = delete;
	Indexed(Indexed&&) = default;
	Indexed& operator=(Indexed&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> view;
		size_t index;
	public:
		view_(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> _view, size_t n)
			: view{ std::move(_view) }
			, index{n}
		{}
		value_type get() const override final { return std::make_tuple(index, view->get()); }
		void next() override final { view->next(), index++; }
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), index);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<default_rev_view<value_type>>(this->fwd());
	}

public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { this->rev() }; }
	reverse_iterator rend() const { return {}; }
private:
	Traversal traversal;
	size_t index;
};

template < class Traversal >
class Indexed<Traversal, false>
	: private tag::sentinel_traversal_tag
	, private forward_traversal
{
	friend struct view_get;
public:
	using value_type = typename std::tuple<size_t, typename remove_cvr_t<Traversal>::value_type>;
	using iterator = sentinel_iterator<value_type>;

	Indexed(Traversal traversal, size_t n)
		: traversal{ traversal }
		, index(n) {}
	Indexed(Indexed const&) = delete;
	Indexed& operator=(Indexed const&) = delete;
	Indexed(Indexed&&) = default;
	Indexed& operator=(Indexed&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> view;
		size_t index;
	public:
		view_(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> _view, size_t n)
			: view{ std::move(_view) }
			, index{ n }
		{}
		value_type get() const override final { return std::make_tuple(index, view->get()); }
		void next() override final { view->next(), index++; }
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), index);
	}
public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
private:
	Traversal traversal;
	size_t index;
};

namespace _proxy {
	struct IndexedX: tag::adaptor_tag {
		size_t n;
	public:
		IndexedX(size_t n): n{n} {}

		template < class Trav >
		auto operator()(Trav&& trav) { return Indexed<Trav, std::is_base_of<reversable_traversal, Trav>::value>{
			std::forward<Trav>(trav), n
		}; }
	};
}
namespace view {
	inline auto indexed(size_t n=0) noexcept { return _proxy::IndexedX{n}; }
}

}}}
#endif
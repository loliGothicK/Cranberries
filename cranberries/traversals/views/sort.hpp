#ifndef CRANBERRIES_RANGES_VIEW_SORT_HPP
#define CRANBERRIES_RANGES_VIEW_SORT_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../../common/defaulted_type.hpp"
#include <type_traits>
#include <iostream>
#include <algorithm>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Traversable, class Pred >
class Sorted
	: private tag::sentinel_traversal_tag
	, private reversable_traversal
	, public _detail::size_impl_injection<Sorted<Traversable, Pred>, Traversable>
{
	friend struct view_get;
	template <class, class, class> friend struct _detail::size_impl_injection;
public:
	using value_type = typename std::decay_t<Traversable>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	template < class Trav_, class Pred_ >
	Sorted(Trav_&& traversal, Pred_&& pred)
		: traversal{ std::forward<Trav_>(traversal) }, pred{ std::forward<Pred_>(pred) } {}
	Sorted(Sorted const&) = delete;
	Sorted& operator=(Sorted const&) = delete;
	Sorted(Sorted&&) = default;
	Sorted& operator=(Sorted&&) = default;
private:
	class fwd_view final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		std::vector<value_type> seq;
		typename std::vector<value_type>::const_iterator iter;
	public:
		fwd_view(std::unique_ptr<polymorphic_view<value_type>> _view, Pred pred)
			: view{ std::move(_view) }
			, seq{}
		{
			for (; !view->is_end(); view->next()) seq.emplace_back(view->get());
			std::sort(seq.begin(), seq.end(), pred );
			iter = seq.cbegin();
		}
		value_type get() const override final { return *iter; }
		void next() override final { ++iter;  }
		bool is_end() const override final { return iter == seq.cend(); }
	};

	class rev_view final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		std::vector<value_type> seq;
		typename std::vector<value_type>::const_reverse_iterator iter;
	public:
		rev_view(std::unique_ptr<polymorphic_view<value_type>> _view, Pred pred)
			: view{ std::move(_view) }
			, seq{}
		{
			for (; !view->is_end(); view->next()) seq.emplace_back(view->get());
			std::sort(seq.begin(), seq.end(), pred);
			iter = seq.crbegin();
		}
		value_type get() const override final { return *iter; }
		void next() override final { ++iter; }
		bool is_end() const override final { return iter == seq.crend(); }
	};


	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<fwd_view>(view_get::fwd(traversal), pred);
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<rev_view>(view_get::fwd(traversal), pred);
	}

public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
	reverse_iterator rbegin() const { return { this->rev() }; }
	reverse_iterator rend() const { return {}; }
private:
	Traversable traversal;
	Pred pred;
};


namespace _proxy {
	template < class Pred = cranberries::less<> >
	struct SortX: tag::adaptor_tag {
		mutable Pred pred;
	public:
		template < class Pred_ >
		constexpr SortX(Pred_&& pred): pred{ std::forward<Pred_>(pred) } {}
		constexpr SortX() : pred{} {}

		template < class Trav >
		auto operator()(Trav&& trav) const {
			return Sorted<Trav, Pred>{ std::forward<Trav>(trav), (pred) };
		}
	};
}

namespace view {
	inline auto sort() { return _proxy::SortX<>{}; }

	template < class Pred >
	auto sort(Pred&& pred) { return _proxy::SortX<Pred>{ std::forward<Pred>(pred) }; }
}

}}}
#endif // !CRANBERRIES_RANGES_ACTION_ADAPTORS_WRITELINE_HPP

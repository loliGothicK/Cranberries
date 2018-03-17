#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_WHILE_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_TAKE_WHILE_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Traversal, class Pred >
class TakeWhile
	: private tag::sentinel_traversal_tag
	, public reversable_traversal
	, public _detail::conjunction_bound<Traversal>
{
	friend struct view_get;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	TakeWhile(Traversal traversal, Pred pred)
		: traversal{ traversal }
		, pred(pred)
	{}
	TakeWhile(TakeWhile const&) = delete;
	TakeWhile& operator=(TakeWhile const&) = delete;
	TakeWhile(TakeWhile&&) = default;
	TakeWhile& operator=(TakeWhile&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		mutable Pred pred;
		bool end{};
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view, Pred _pred)
			: view{ std::move(_view) }
			, pred{ _pred }
		{
			if (!pred(view->get())) end = true;
		}
		value_type get() const override final { return view->get(); }
		void next() override final { view->next(); if (!pred(view->get())) end = true; }
		bool is_end() const override final { return end || view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal), pred);
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
	Pred pred;
};

namespace _proxy {
	template < class Pred >
	class TakeWhileX: tag::adaptor_tag {
		Pred pred;
	public:
		template < class Pred_ >
		TakeWhileX(Pred_&& pred): pred{ std::forward<Pred_>(pred) } {}

		template < class Trav >
		auto operator()(Trav&& trav) {
			return TakeWhile<Trav, Pred>{ std::forward<Trav>(trav), pred };
		}
	};
}

namespace view {
	template < class Pred >
	auto take_while(Pred&& pred) { return _proxy::TakeWhileX<Pred>{ std::forward<Pred>(pred) }; }
}

}}}
#endif
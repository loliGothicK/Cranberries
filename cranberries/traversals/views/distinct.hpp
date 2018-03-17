#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_DISTINCT_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_DISTINCT_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Traversal, bool = true >
class Distinct
	: private tag::sentinel_traversal_tag
	, private reversable_traversal
{
	friend struct view_get;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	template < class Trav >
	Distinct(Trav&& traversal)
		: traversal{ std::forward<Trav>(traversal) }
	{}
	Distinct(Distinct const&) = delete;
	Distinct& operator=(Distinct const&) = delete;
	Distinct(Distinct&&) = default;
	Distinct& operator=(Distinct&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		std::unordered_map<value_type, bool> hash;
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view)
			: view{ std::move(_view) }
			, hash{}
		{
			hash[view->get()] = true;
		}
		value_type get() const override final { return view->get(); }
		void next() override final {
			view->next();
			while (hash[view->get()] && !view->is_end()) view->next();
			hash[view->get()] = true;
		}
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal));
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
};

template < class Traversal >
class Distinct<Traversal, false>
	: private tag::sentinel_traversal_tag
	, private forward_traversal
{
	friend struct view_get;
public:
	using value_type = typename std::decay_t<Traversal>::value_type;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;

	template < class Trav >
	Distinct(Trav&& traversal)
		: traversal{ std::forward<Trav>(traversal) }
	{}
	Distinct(Distinct const&) = delete;
	Distinct& operator=(Distinct const&) = delete;
	Distinct(Distinct&&) = default;
	Distinct& operator=(Distinct&&) = default;
private:
	class view_ : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<value_type>> view;
		std::unordered_map<value_type, bool> hash;
	public:
		view_(std::unique_ptr<polymorphic_view<value_type>> _view)
			: view{ std::move(_view) }
			, hash{}
		{
			hash[view->get()] = true;
		}
		value_type get() const override final { return view->get(); }
		void next() override final {
			view->next();
			while (hash[view->get()] || view->is_end()) view->next();
			hash[view->get()] = true;
		}
		bool is_end() const override final { return view->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(traversal));
	}

public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
private:
	Traversal traversal;
};

namespace _proxy {
	struct DistinctX: tag::adaptor_tag {
		template < class Trav >
		auto operator()(Trav&& trav) const {
			return Distinct<Trav, std::is_base_of<reversable_traversal, Trav>::value>{
				std::forward<Trav>(trav)
			};
		}
	};
}


namespace view {
	constexpr _proxy::DistinctX distinct{};
}

}}}
#endif
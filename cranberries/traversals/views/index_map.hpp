#ifndef CRANBERRIES_VIEW_MODULE_INDEX_MAP_HPP
#define CRANBERRIES_VIEW_MODULE_INDEX_MAP_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include "../../functional/relational.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class Traversal,
					 class F,
					 typename IndexType,
					 class Next,
					 bool = true >
class IndexMap
	: private tag::sentinel_traversal_tag
	, public reversable_traversal
{
	friend struct view_get;
public:
	using value_type = cranberries::invoke_result_t<F, IndexType, typename remove_cvr_t<Traversal>::value_type>;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using finite_ = std::nullptr_t;
	using immutable_ = std::nullptr_t;

	IndexMap(Traversal traversal, F f)
		: traversal{ std::forward<Traversal>(traversal) }
		, f(std::forward<F>(f))
		, ini{}
		, incr{}
	{}

	IndexMap(Traversal traversal, F f, const IndexType& ini)
		: traversal{ std::forward<Traversal>(traversal) }
		, f(std::forward<F>(f))
		, ini{ ini }
		, incr{}
	{}

	IndexMap(Traversal traversal, F f, const IndexType& ini, Next next)
		: traversal{ std::forward<Traversal>(traversal) }
		, f(std::forward<F>(f))
		, ini{ ini }
		, incr{ std::forward<Next>(next) }
	{}

	IndexMap(IndexMap const&) = delete;
	IndexMap& operator=(IndexMap const&) = delete;
	IndexMap(IndexMap&&) = default;
	IndexMap& operator=(IndexMap&&) = default;
private:
	class _view final : public polymorphic_view<value_type>
	{
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> view_;
		IndexType index;
		const F& f;
		Next _next;
	public:
		_view(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> p, const IndexType& init, const F& f, const Next& next)
			: view_{ std::move(p) }, index{ init }, f{ f }, _next{ next }
		{}
		_view(const _view&) = default;
	protected:
		value_type get() const override final { return f( index, view_->get() ); }
		void next() override final { view_->next(), _next(index); }
		bool is_end() const override final { return view_->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<_view>(view_get::fwd(traversal), ini, f, incr);
	}
	template < class, class = void >
	struct rev_impl {
		template < class Trav, class... Args >
		static constexpr auto invoke(Trav&& trav, Args&&... args) {
			return std::make_unique<_view>(view_get::fwd(std::forward<Trav>(trav)), std::forward<Args>(args)...);
		}
	};
	template < class T >
	struct rev_impl<T, cranberries::void_t<decltype(std::declval<T const&>().size())>> {
		template < class Trav, class F_, class Next_ >
		static constexpr auto invoke(Trav&& trav, IndexType index, F&& f, Next&& incr) {
			for (size_t i{}; i < trav.size(); ++i) index = incr(index);
			return std::make_unique<default_rev_view<value_type>>(
				view_get::rev(std::forward<Trav>(trav)),
				index,
				std::forward<F_>(f),
				std::forward<Next_>(incr)
				);
		}
	};

	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return rev_impl<remove_cvr_t<Traversal>>::invoke(
			std::forward<Traversal>(traversal), ini, std::forward<F>(f), std::forward<Next>(incr)
		);
	}
public:
	auto begin() const  { return iterator{ this->fwd() }; }
	auto end() const    { return iterator{}; }
	auto rbegin() const { return reverse_iterator{ this->rev() }; }
	auto rend() const   { return reverse_iterator{}; }
private:
	Traversal traversal;
	F f;
	IndexType ini;
	Next incr;
};

template < class Traversal,
					 class F,
					 typename IndexType,
					 class Next >
class IndexMap<Traversal, F, IndexType, Next, false>
	: private tag::sentinel_traversal_tag
	, public forward_traversal
{
	friend struct view_get;
public:
	using value_type = cranberries::invoke_result_t<F, IndexType, typename remove_cvr_t<Traversal>::value_type>;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using infinite_ = std::nullptr_t;
	using immutable_ = std::nullptr_t;

	IndexMap(Traversal traversal, F f)
		: traversal{ std::forward<Traversal>(traversal) }
		, f(std::forward<F>(f))
		, ini{}
		, incr{}
	{}

	IndexMap(Traversal traversal, F f, const IndexType& ini)
		: traversal{ std::forward<Traversal>(traversal) }
		, f(std::forward<F>(f))
		, ini{ ini }
		, incr{}
	{}

	IndexMap(Traversal traversal, F f, const IndexType& ini, const Next& next)
		: traversal{ std::forward<Traversal>(traversal) }
		, f(std::forward<F>(f))
		, ini{ ini }
		, incr{ next }
	{}

	IndexMap(IndexMap const&) = delete;
	IndexMap& operator=(IndexMap const&) = delete;
	IndexMap(IndexMap&&) = default;
	IndexMap& operator=(IndexMap&&) = default;
private:
	class _view final : public polymorphic_view<value_type>
	{
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> view_;
		const F& f;
		IndexType index;
		Next _next;
	public:
		_view(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Traversal>::value_type>> p, const F& f, const IndexType& init, const Next& next)
			: view_{ std::move(p) }, f{ f }, index{ init }, _next{ next }
		{}
		_view(const _view&) = default;
	protected:
		value_type get() const override final { return f( index, view_->get() ); }
		void next() override final { view_->next(), _next(index); }
		bool is_end() const override final { return view_->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<_view>(view_get::fwd(traversal), f, ini, incr);
	}
public:
	auto begin() const  { return iterator{ this->fwd() }; }
	auto end() const    { return iterator{}; }
private:
	Traversal traversal;
	F f;
	IndexType ini;
	Next incr;
};

namespace _proxy {

	template < class F, class IndexType = void, class Next = void >
	class IndexMapX: tag::adaptor_tag {
		F f;
		IndexType ini;
		Next next;
	public:
		template < class F_, class Next_ >
		IndexMapX(F_&& f, IndexType ini, Next_&& next)
			: f{ std::forward<F_>(f) }, ini{ ini }, next{ std::forward<Next_>(next) } {}

		template < class Traversable >
		auto operator()(Traversable&& trav) {
			return IndexMap< Traversable, F, IndexType, Next, std::is_base_of<reversable_traversal, remove_cvr_t<Traversable>>::value >{
					std::forward<Traversable>(trav)
				, std::forward<F>(f)
				, ini
				, next
			};
		}
	};

	template < class F, class IndexType >
	class IndexMapX<F, IndexType, void>: tag::adaptor_tag {
		F f;
		IndexType ini;
	public:
		template < class F_ >
		IndexMapX(F_&& f, IndexType ini) : f{ std::forward<F_>(f) }, ini{ini} {}

		template < class Traversable >
		auto operator()(Traversable&& trav) {
			return IndexMap< Traversable, F, IndexType, cranberries::pre_increment<>, std::is_base_of<reversable_traversal, remove_cvr_t<Traversable>>::value >{
					std::forward<Traversable>(trav)
				, std::forward<F>(f)
				, ini
			};
		}
	};

	template < class F >
	class IndexMapX<F,void,void>: tag::adaptor_tag {
		F f;
	public:
		template < class F_ >
		explicit IndexMapX(F_&& f) : f{ std::forward<F_>(f) } {}

		template < class Traversable >
		auto operator()(Traversable&& trav) {
			return IndexMap< Traversable, F, size_t, cranberries::pre_increment<>, std::is_base_of<reversable_traversal, remove_cvr_t<Traversable>>::value >{
				std::forward<Traversable>(trav), std::forward<F>(f)
			};
		}
	};


}

namespace view {
	template < typename F >
	auto index_map(F&& f) { return _proxy::IndexMapX<F&&>{ std::forward<F>(f) }; }

	template < typename F, class IndexType >
	auto index_map(F&& f, const IndexType& ini) { return _proxy::IndexMapX<F&&, const IndexType&>{ std::forward<F>(f), ini }; }

	template < typename F, class IndexType, class Next >
	auto index_map(F&& f, const IndexType& ini, Next&& next) { return _proxy::IndexMapX<F&&, const IndexType, Next&&>{ std::forward<F>(f), ini, std::forward<Next>(next) }; }
}

}}}
#endif
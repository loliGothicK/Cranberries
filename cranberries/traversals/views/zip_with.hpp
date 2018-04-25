#ifndef CRANBERRIES_RANGES_VIEW_ADAPTORS_ZIP_WITH_HPP
#define CRANBERRIES_RANGES_VIEW_ADAPTORS_ZIP_WITH_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>
#include <memory>

namespace cranberries {
inline namespace experimental {
namespace traversals {

template < class, class >
class ZipWith;

namespace _detail{
	template < class, class = void >
	struct size_impl_injection_for_zipwith {};

	template < class Left, class Right >
	struct size_impl_injection_for_zipwith<ZipWith<Left, Right>,
		::cranberries::void_t<decltype(std::declval<const Left&>().size() == std::declval<const Right&>().size())>
	>
	{
		constexpr size_t size() const {
			return std::min(
				static_cast<const ZipWith<Left, Right>*>(this)->left.size(),
				static_cast<const ZipWith<Left, Right>*>(this)->right.size()
			); 
		}
	};
}

template < class Left, class Right >
class ZipWith
	: private tag::sentinel_traversal_tag
	, private reversable_traversal
	, public _detail::size_impl_injection_for_zipwith<ZipWith<Left, Right>>
{
	friend struct view_get;
	template < class, class > friend struct _detail::size_impl_injection_for_zipwith;
public:
	using value_type = std::tuple<
		typename remove_cvr_t<Left>::value_type,
		typename remove_cvr_t<Right>::value_type>;
	using iterator = sentinel_iterator<value_type>;
	using reverse_iterator = sentinel_iterator<value_type>;
	using immutable_ = std::nullptr_t;

	ZipWith(Left left, Right right)
		: left{ left }
		, right{ right }
	{}
	ZipWith(ZipWith const&) = delete;
	ZipWith& operator=(ZipWith const&) = delete;
	ZipWith(ZipWith&&) = default;
	ZipWith& operator=(ZipWith&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Left>::value_type>> left_;
		std::unique_ptr<polymorphic_view<typename remove_cvr_t<Right>::value_type>> right_;
	public:
		view_(std::unique_ptr<polymorphic_view<typename remove_cvr_t<Left>::value_type>> left
				, std::unique_ptr<polymorphic_view<typename remove_cvr_t<Right>::value_type>> right)
			: left_{ std::move(left) }
			, right_{ std::move(right) }
		{}
		value_type get() const override final { return std::make_tuple(left_->get(), right_->get()); }
		void next() override final { left_->next(), right_->next(); }
		bool is_end() const override final { return left_->is_end() || right_->is_end(); }
	};

	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>(view_get::fwd(left), view_get::fwd(right));
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
	Left left;
	Right right;
};

namespace _proxy {
	template < class Right >
	class ZipWithX: tag::adaptor_tag {
		Right right;
	public:
		template < class Trav >
		ZipWithX(Trav&& trav): right{ std::forward<Trav>(trav) } {}

		template < class Left >
		auto operator()(Left&& left) {
			return ZipWith<Left, Right>{ std::forward<Left>(left), std::forward<Right>(right) };
		}
	};
}

namespace view {
	template < class Right >
	auto zip_with(Right&& traversal) { return _proxy::ZipWithX<Right>{ std::forward<Right>(traversal) }; }
}

template <class, class...>
class Zipped;

template < size_t... I, class... Traversals >
class Zipped<std::index_sequence<I...>, Traversals...>
	: private tag::sentinel_traversal_tag
	, private forward_traversal
{
	friend struct view_get;
public:
	using value_type = std::tuple<typename std::decay_t<Traversals>::value_type...>;
	using iterator = sentinel_iterator<value_type>;

	template < class... Traversals_ >
	Zipped(Traversals_&&... traversals)
		: traversals_( std::forward_as_tuple(std::forward<Traversals_>(traversals)...) )
	{}
	Zipped(Zipped const&) = delete;
	Zipped& operator=(Zipped const&) = delete;
	Zipped(Zipped&&) = default;
	Zipped& operator=(Zipped&&) = default;
private:
	class view_ final : public polymorphic_view<value_type> {
		std::tuple<typename std::decay_t<Traversals>::iterator...> iters_;
		std::tuple<typename std::decay_t<Traversals>::iterator...> ends_;
		using swallow = std::initializer_list<int>;
	public:
		template < class... Rs >
		view_(Rs&&... traversals)
			: iters_{ cranberries::back_magic::begin(traversals)... }
			, ends_{ cranberries::back_magic::end(traversals)... }
		{}

		virtual value_type get() const override final { return std::forward_as_tuple(*std::get<I>(iters_)...); }

		virtual bool is_end() const override final {
			bool _ = false;
			(void)swallow {
				(void(_ = _ || std::get<I>(iters_) == std::get<I>(ends_)), 0)...
			};
			return _;
		}
		virtual void next() override final {
			(void)swallow {
			(void(++std::get<I>(iters_)), 0)...
		};
		}
	};
	std::unique_ptr<polymorphic_view<value_type>> fwd() const {
		return std::make_unique<view_>( std::get<I>(traversals_)... );
	}
	std::unique_ptr<polymorphic_view<value_type>> rev() const {
		return std::make_unique<default_rev_view<value_type>>(this->fwd());
	}
public:
	iterator begin() const { return { this->fwd() }; }
	iterator end() const { return {}; }
	iterator rbegin() const { return { this->rev() }; }
	iterator rend() const { return {}; }
private:
	std::tuple<Traversals...> traversals_;
};

namespace view {
	template < class... Traversals >
	auto make_zipped(Traversals&&... traversals)
	{
		return Zipped<std::index_sequence_for<Traversals...>, Traversals...>{ std::forward<Traversals>(traversals)... };
	}
}

}}}
#endif
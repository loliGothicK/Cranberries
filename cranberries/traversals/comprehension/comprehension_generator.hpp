#ifndef CRANBERRIES_RANGES_COMPREHENSION_GENERATOR_HPP
#define CRANBERRIES_RANGES_COMPREHENSION_GENERATOR_HPP
#include "../detail/tags.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../../utility/utility.hpp"
#include "../detail/default_sentinel.hpp"
#include <type_traits>

namespace cranberries {
inline namespace experimental {
namespace traversals {

	template < class Traversable1, class Traversable2 = void >
	class ComprehensionGenerator
		: private ::cranberries::traversals::tag::sentinel_traversal_tag
		, private ::cranberries::traversals::forward_traversal
	{
		friend struct view_get;
	public:
		using value_type = decltype(::cranberries::back_magic::make_tuple_cat(std::declval<typename std::decay_t<Traversable1>::value_type>(), std::declval<typename std::decay_t<Traversable2>::value_type>()));
		using iterator = ::cranberries::traversals::sentinel_iterator<value_type>;
		using value_type_1 = typename std::decay_t<Traversable1>::value_type;
		using value_type_2 = typename std::decay_t<Traversable2>::value_type;

		template < class T1, class T2 >
		ComprehensionGenerator(T1&& tr1, T2&& tr2)
			: traversal1{ std::forward<T1>(tr1) }
			, traversal2{ std::forward<T2>(tr2) }
		{}

	private:
		class view_ final : public ::cranberries::traversals::polymorphic_view<value_type> {
			std::unique_ptr<traversals::polymorphic_view<value_type_1>> view;
			std::vector<value_type_2> traversal;
			typename std::vector<value_type_2>::const_iterator iter;
			typename std::vector<value_type_2>::const_iterator last;
		public:
			view_(std::unique_ptr<traversals::polymorphic_view<value_type_1>> tr1, std::vector<value_type_2> tr2)
				: view{std::move(tr1)}
				, traversal{ std::move(tr2) }
				, iter{ ::cranberries::back_magic::cbegin(traversal) }
				, last{ ::cranberries::back_magic::cend(traversal) }
			{}

			virtual value_type get() const override final {
				return ::cranberries::back_magic::make_tuple_cat(view->get(), *iter);
			}
			virtual void next() override final {
				if (++iter == last)
					{ iter = ::cranberries::back_magic::cbegin(traversal); view->next(); }
			}
			virtual bool is_end() const override final { return view->is_end(); }
		};

		std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>> fwd() const {
			std::vector<value_type_2> tmp{ };
			for (auto&& elm : traversal2) tmp.emplace_back(elm);
			return std::make_unique<view_>(view_get::fwd(traversal1), std::move(tmp));
		}

	public:
		iterator begin() const { return { this->fwd() }; }
		iterator end() const { return {}; }
	private:
		Traversable1 traversal1;
		Traversable2 traversal2;
	};

	template < class Traversal >
	class ComprehensionGenerator<Traversal, std::enable_if_t<!is_tuple_v<typename remove_cvr_t<Traversal>::value_type>>>
		: private ::cranberries::traversals::tag::sentinel_traversal_tag
		, private ::cranberries::traversals::forward_traversal
	{
		friend struct view_get;
	public:
		using value_type = std::tuple<typename remove_cvr_t<Traversal>::value_type>;
		using iterator = ::cranberries::traversals::sentinel_iterator<value_type>;

		template < class Trav >
		ComprehensionGenerator(Trav&& trav)
			: traversal{ std::forward<Trav>(trav) }
		{}

	private:
		class view_ final : public ::cranberries::traversals::polymorphic_view<value_type> {
			typename std::decay_t<Traversal>::iterator iter;
			typename std::decay_t<Traversal>::iterator last;
		public:
			template < class Trav >
			view_(Trav&& traversal)
				: iter{ ::cranberries::back_magic::begin(traversal) }
				, last{ ::cranberries::back_magic::end(traversal) }
			{}

			virtual value_type get() const override final {
				return std::make_tuple(*iter);
			}
			virtual void next() override final { ++iter; }
			virtual bool is_end() const override final { return iter == last; }
		};

		std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>> fwd() const {
			return std::make_unique<view_>(traversal);
		}

	public:
		iterator begin() const { return { this->fwd() }; }
		iterator end() const { return {}; }
	private:
		Traversal traversal;
	};


namespace _detail {
	template < class Tr1, class Tr2 >
	auto make_comprehension_generator(Tr1&& tr1, Tr2&& tr2) {
		return ComprehensionGenerator<Tr1, Tr2>{
			std::forward<Tr1>(tr1), std::forward<Tr2>(tr2)
		};
	}

	template < class Tr1 >
	auto into_generator(Tr1&& tr1) {
		return ComprehensionGenerator<Tr1>{ std::forward<Tr1>(tr1) };
	}

}

}}}
#endif
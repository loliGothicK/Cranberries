#ifndef CRANBERRIES_LIST_COMPREHENSION_HPP
#define CRANBERRIES_LIST_COMPREHENSION_HPP
#include "../../functional/lambda.hpp"
#include "../../common/concepts.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"
#include "./comprehension_generator.hpp"
#include "../views/transform.hpp"
#include "../views/filter.hpp"
#include "../../type_traits.hpp"
#include <tuple>
namespace cranberries {
inline namespace experimental {
namespace traversals{
	template < class Traversal1, class Traversal2,
		concepts::concept_required<Traversal1, concepts::iterable> = nullptr,
		concepts::concept_required<Traversal2, concepts::iterable> = nullptr>
	decltype(auto) operator& (Traversal1&& lhs, Traversal2&& rhs)
	{
		return _detail::make_comprehension_generator(std::forward<Traversal2>(rhs), std::forward<Traversal1>(lhs));
	}

	//template < class Traversal, class Pred,
	//	concepts::concept_required<Traversal, concepts::iterable> = nullptr,
	//	enabler_t<is_callable_v<Pred(typename std::decay_t<Traversal>::value_type)>, bool> = nullptr>
	//decltype(auto) operator== (Traversal&& trv, Pred&& pred)
	//{
	//	return ::cranberries::traversals::view::filter(std::forward<Pred>(pred))(std::forward<Traversal>(trv));
	//}

}}


	template < class ET, class Traversable >
	class list_comprehension_t
		: public ::cranberries::traversals::tag::sentinel_traversal_tag
		, private ::cranberries::traversals::forward_traversal
	{
		friend struct ::cranberries::traversals::view_get;
	public:
		using value_type = decltype(::cranberries::apply(std::declval<ET>(), std::declval<typename std::decay_t<Traversable>::value_type>()));
		using iterator = ::cranberries::traversals::sentinel_iterator<value_type>;

		template < class Trav >
		list_comprehension_t(ET expr, Trav&& gen)
			: expr{ expr }
			, gen{ std::forward<Trav>(gen) }
		{}
	private:
		class view_ final : public ::cranberries::traversals::polymorphic_view<value_type> {
			ET expr_;
			std::unique_ptr<::cranberries::traversals::polymorphic_view< typename ::cranberries::remove_cvr_t<Traversable>::value_type >> gen_;
		public:
			view_(ET expr, std::unique_ptr<::cranberries::traversals::polymorphic_view< typename ::cranberries::remove_cvr_t<Traversable>::value_type >> gen)
				: expr_{ expr }
				, gen_{ std::move(gen) }
			{}

			virtual value_type get() const override final { return ::cranberries::apply(expr_, gen_->get()); }
			virtual void next() override final { gen_->next(); }
			virtual bool is_end() const override final { return gen_->is_end(); }
		};

		std::unique_ptr<::cranberries::traversals::polymorphic_view<value_type>> fwd() const {
			return std::make_unique<view_>(expr, traversals::view_get::fwd(gen));
		}

	public:
		iterator begin() const { return { this->fwd() }; }
		iterator end() const { return {}; }
	private:
		ET expr;
		Traversable gen;
	};

	template < class ET, class Traversable >
	list_comprehension_t<ET, Traversable>
	make_list_comprehension(ET&& expr, Traversable&& gen)
	{
		return { std::forward<ET>(expr), std::forward<Traversable>(gen) };
	}


	template < class ET, class Traversable,
		enabler_t<conjunction_v<
			concepts::required<Traversable, concepts::iterable>,
			is_tuple<typename std::decay_t<Traversable>::value_type>,
			cranberries::is_expression<std::decay_t<ET>>
		>> = nullptr >
	auto operator|(ET&& expr, Traversable&& gen)
	{
		return make_list_comprehension( std::forward<ET>(expr), std::forward<Traversable>(gen) );
	}

	template < class ET, class Traversable,
		enabler_t<conjunction_v<
			concepts::required<Traversable, concepts::iterable>,
			negation<is_tuple<typename std::decay_t<Traversable>::value_type>>,
			cranberries::is_expression<std::decay_t<ET>>
		>> = nullptr >
	auto operator|(ET&& expr, Traversable&& gen)
	{
		return make_list_comprehension(
			std::forward<ET>(expr), 
			traversals::_detail::into_generator(std::forward<Traversable>(gen))
		);
	}

}

#endif // !CRANBERRIES_LIST_COMPREHENTION_HPP

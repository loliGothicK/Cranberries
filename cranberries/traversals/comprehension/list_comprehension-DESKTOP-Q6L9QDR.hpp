#ifndef CRANBERRIES_LIST_COMPREHENSION_HPP
#define CRANBERRIES_LIST_COMPREHENSION_HPP
#include "./expression.hpp"
#include "../../../common/concepts.hpp"
#include "../../detail/sentinel_iterator.hpp"
#include "../../detail/tags.hpp"
#include "../../views/multi_demensional.hpp"
#include "../../views/transform.hpp"
#include "../../traversal_pipe.hpp"
#include <tuple>
namespace cranberries {

	template < class ET, class Traversable >
	class list_comprehension_t
		: public traversals::tag::sentinel_traversal_tag
	{
		ET expr;
		Traversable gen;
	public:
		class forward_sentinel {
			ET expr_;
			Traversable gen_;
			typename std::decay_t<Traversable>::iterator iter_;
			typename std::decay_t<Traversable>::iterator last_;
		public:
			using value_type = decltype(::cranberries::apply(std::declval<ET>(), std::declval<typename std::decay_t<Traversable>::value_type>()));

			forward_sentinel(ET expr, Traversable const& gen)
				: expr_{ expr }
				, gen_{ gen }
				, iter_{ cranberries::back_magic::begin(gen_) }
				, last_{ cranberries::back_magic::end(gen_) }
			{}

			auto get() { return ::cranberries::apply(expr_, *iter_); }
			void next() { ++iter_; }
			bool is_end() { return iter_ == last_; }
		};
	public:
		using value_type = decltype(::cranberries::apply(std::declval<ET>(), std::declval<typename std::decay_t<Traversable>::value_type>()));
		using forward_sentinel = forward_sentinel;
		using iterator = traversals::sentinel_iterator<forward_sentinel>;

		list_comprehension_t(ET expr, Traversable const& gen)
			: expr{ expr }
			, gen { gen  }
		{}

		iterator begin() const { return { std::make_unique<forward_sentinel>(expr, gen) }; }
		iterator end() const { return {}; }

	};

	template < class ET, class Traversable >
	list_comprehension_t<ET, Traversable>
	make_list_comprehension(ET&& expr, Traversable&& gen)
	{
		return { std::forward<ET>(expr), std::forward<Traversable>(gen) };
	}


	template < class ET, class Traversable,
		enabler_t<conjunction_v<
			is_tuple<typename std::decay_t<Traversable>::value_type>,
			expressions::is_expr<std::decay_t<ET>>
		>> = nullptr >
	auto operator|(ET&& expr, Traversable&& gen)
	{
		return make_list_comprehension( std::forward<ET>(expr), std::forward<Traversable>(gen) );
	}

	template < class ET, class Traversable,
		enabler_t<conjunction_v<
			negation<is_tuple<typename std::decay_t<Traversable>::value_type>>,
			expressions::is_expr<std::decay_t<ET>>
		>> = nullptr >
	auto operator|(ET&& expr, Traversable&& gen)
	{
		return make_list_comprehension(
			std::forward<ET>(expr), 
			traversals::view::transform([](auto&& item) { return std::make_tuple(item); })(std::forward<Traversable>(gen))
		);
	}

	template < class ET, class Traversable,
		enabler_t<conjunction_v<
			is_tuple<typename std::decay_t<Traversable>::value_type>,
			std::is_bind_expression<std::decay_t<ET>>
		>> = nullptr >
	auto operator|(ET&& expr, Traversable&& gen)
	{
		return make_list_comprehension( std::forward<ET>(expr), std::forward<Traversable>(gen) );
	}

	template < class ET, class Traversable,
		enabler_t<conjunction_v<
			negation<is_tuple<typename std::decay_t<Traversable>::value_type>>,
			std::is_bind_expression<std::decay_t<ET>>
		>> = nullptr >
	auto operator|(ET&& expr, Traversable&& gen)
	{
		return make_list_comprehension(
			std::forward<ET>(expr), 
			traversals::view::transform([](auto&& item) { return std::make_tuple(item); })(std::forward<Traversable>(gen))
		);
	}

}

#endif // !CRANBERRIES_LIST_COMPREHENTION_HPP

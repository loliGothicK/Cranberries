#ifndef CRANBERRIES_LIST_COMPREHENSION_VARIABLE_HPP
#define CRANBERRIES_LIST_COMPREHENSION_VARIABLE_HPP
#include <tuple>
#include "../../../utility/utility.hpp"
#include "../../../pack_operations.hpp"
#include <functional>
namespace cranberries {

	template < size_t N >
	struct placeholder_t {
		static constexpr size_t value = N -1;
	};

	template < class >
	struct is_placeholder : std::false_type {};

	template < size_t N >
	struct is_placeholder<placeholder_t<N>> : std::true_type {};

	template < class T >
	constexpr bool is_placeholder_v = is_placeholder<T>::value;

	template<size_t... Indices>
	struct placeholder_sequence {};

	template<size_t First, size_t Last>
	struct placeholder_span {
		static constexpr size_t first = First;
		static constexpr size_t last = Last;
	};


	template<char...>
	struct make_placeholder_sequence_impl;
	template<char...>
	struct make_placeholder_sequence;

	template<>
	struct make_placeholder_sequence_impl<> {
		static constexpr std::size_t base = 1;
		static constexpr std::size_t value = 0;
		using type = placeholder_sequence<>;
	};

	template<char C, char... Chars>
	struct make_placeholder_sequence_impl<C, Chars...> {
		using impl = make_placeholder_sequence_impl<Chars...>;
		static constexpr std::size_t base = impl::base * 10;
		static constexpr std::size_t value = impl::value + impl::base * (C - '0');
		using type = typename impl::type;
	};

	template<char... Chars>
	struct make_placeholder_sequence_impl<'\'', Chars...> {
		static constexpr std::size_t base = 1;
		static constexpr std::size_t value = 0;
		using type = typename make_placeholder_sequence<Chars...>::type;
	};

	template<char... Chars>
	struct make_placeholder_sequence {
		template<std::size_t N, std::size_t... Ns>
		static auto concat(placeholder_sequence<Ns...>)
			->placeholder_sequence<N, Ns...>;

		using impl = make_placeholder_sequence_impl<Chars...>;
		using type = decltype(concat<impl::value>(typename impl::type{}));
	};

	template < class, char... >
	struct make_placeholder_span_impl;

	template < char C, char... L, char... Rest >
	struct make_placeholder_span_impl<value_pack<char, L...>, C, Rest...>
		: make_placeholder_span_impl<value_pack<char, L..., C>, Rest...>
	{};

	template < char... L, char... Rest >
	struct make_placeholder_span_impl<value_pack<char, L...>, '\'', Rest...>
		: nested_type_class<placeholder_span<to_decimal<L...>::value, to_decimal<Rest...>::value>>
	{};

	template < char... Chars >
	using make_placeholder_span = make_placeholder_span_impl<value_pack<char>, Chars...>;

	template < class, class... >
	struct bind_expression_t;

	template < class T >
	struct is_bind_expression : std::is_bind_expression<T> {};

	template < class F, class... Args >
	struct is_bind_expression<bind_expression_t<F,Args...>> : std::true_type {};

	template < class T >
	constexpr bool is_bind_expression_v = is_bind_expression<T>::value;

	template < class F, class... L >
	struct bind_expression_t {
		F f;
		std::tuple<L...> binds;

		template < size_t N, class T,
			cranberries::enabler_t<
				is_placeholder_v<std::decay_t<std::tuple_element_t<N, std::tuple<L...>>>>
			> = nullptr >
		decltype(auto) get_(T& t) {
			return std::get<std::decay_t<std::tuple_element_t<N, std::tuple<L...>>>::value>(t);
		}

		template < size_t N, class T,
			cranberries::enabler_t<
				is_reference_wrapper_v<std::decay_t<std::tuple_element_t<N, std::tuple<L...>>>>
			> = nullptr >
		decltype(auto) get_(T& t) {
			return std::get<N>(binds).get();
		}

		template < size_t N, class T,
			cranberries::enabler_t<
				is_bind_expression_v<std::decay_t<std::tuple_element_t<N, std::tuple<L...>>>>
			> = nullptr >
		decltype(auto) get_(T& t) {
			return ::cranberries::apply( std::get<N>(binds), t);
		}

		template < class Tuple, size_t... Indices >
		decltype(auto) invoke(Tuple& t, std::index_sequence<Indices...>) {
			return f( get_<Indices>(t)... );
		}
	public:
		template < class F_, class Tuple >
		bind_expression_t(F_&& f, Tuple&& t)
			: f{ std::forward<F_>(f) }
			, binds{ std::forward<Tuple>(t) }
		{}

		template < class... Args >
		decltype(auto) operator()(Args&&... args) {
			auto applies = std::forward_as_tuple(std::forward<Args>(args)...);
			return invoke(applies, std::index_sequence_for<L...>{});
		}
	};

	template < size_t... I >
	auto placeholder_sequence_expand(placeholder_sequence<I...>) {
		return std::make_tuple(placeholder_t<I>{}...);
	}

	template < size_t... I >
	auto placeholder_span_expand_impl(std::index_sequence<I...>) {
		return std::make_tuple(placeholder_t<I>{}...);
	}

	template < size_t First, size_t Last >
	auto placeholder_span_expand(placeholder_span<First, Last>) {
		return placeholder_span_expand_impl(
			make_index_range<First, Last>{}
		);
	}


	template < size_t I, class Tuple, class Head, class... Tail >
	auto expand_(Tuple&& result, Head&& head, Tail&&... tail) {
		return expand_<I+1>(
			std::tuple_cat(
				std::forward<Tuple>(result),
				std::forward_as_tuple(std::forward<Head>(head))),
			std::forward<Tail>(tail)...
		);
	}

	template < size_t I, class Tuple, class... Tail >
	auto expand_(Tuple&& result, placeholder_t<0>, Tail&&... tail) {
		return expand_<I + 1>(
			std::tuple_cat(
				std::forward<Tuple>(result),
				std::make_tuple(placeholder_t<I>{})),
			std::forward<Tail>(tail)...
		);
	}

	template < size_t I, class Tuple, size_t N, class... Tail >
	auto expand_(Tuple&& result, placeholder_t<N> p, Tail&&... tail) {
		return expand_<N + 1>(
			std::tuple_cat(
				std::forward<Tuple>(result),
				std::make_tuple(p)),
			std::forward<Tail>(tail)...
			);
	}

	template < size_t I, class Tuple, size_t First, size_t Last, class... Tail >
	auto expand_(Tuple&& result, placeholder_span<First, Last> span, Tail&&... tail) {
		return expand_<Last+1>(
			std::tuple_cat(
				std::forward<Tuple>(result),
				placeholder_span_expand(span)),
			std::forward<Tail>(tail)...
		);
	}

	template < size_t I, class Tuple, size_t... Id, class... Tail >
	auto expand_(Tuple&& result, placeholder_sequence<Id...>, Tail&&... tail) {
		return expand_<I/*TODO*/>(
			std::tuple_cat(
				std::forward<Tuple>(result),
				std::make_tuple(placeholder_t<Id...>{})),
			std::forward<Tail>(tail)...
		);
	}


	template < size_t N, class Tuple >
	auto expand_(Tuple&& result) {
		return std::forward<Tuple>(result);
	}

	template < class F, class... Args >
	bind_expression_t<F, Args...>
	make_bind_expresstion(F&& f, std::tuple<Args...>&& t) {
		return { std::forward<F>(f), std::move(t) };
	}

	template < class F, class... Args >
	constexpr auto
	bind(F&& f, Args&&... args)
	{
		return make_bind_expresstion( std::forward<F>(f), expand_<1>(std::tuple<>{}, std::forward<Args>(args)...) );
	}

	namespace placeholders {
		constexpr placeholder_t<0> _{};
	}

	namespace literals {
		inline namespace placeholder_literals{
			template < char... Chars >
			placeholder_t<to_decimal<Chars...>::value>
				operator"" _() {
				return {};
			}

			template<char... Chars>
			auto operator"" _seq() { return typename make_placeholder_sequence<Chars...>::type{}; }

			template<char... Chars>
			auto operator"" _span() { return typename make_placeholder_span<Chars...>::type{}; }

		}
	}

}

namespace std {
	template < size_t N >
	struct is_placeholder<::cranberries::placeholder_t<N>>
		: public std::integral_constant<int, N>
	{};

	template < class F, class... Args >
	struct is_bind_expression<::cranberries::bind_expression_t<F, Args...>>
		: public std::true_type
	{};
}


#endif // !CRANBERRIES_LIST_COMPREHENTION_HPP

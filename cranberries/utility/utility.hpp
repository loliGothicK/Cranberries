/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_UTILITY_UTILITY_HPP
#define CRANBERRIES_UTILITY_UTILITY_HPP

#include <utility>
#include <type_traits>
#include <iterator>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <mutex>
#include "../type_traits.hpp"

namespace cranberries
{
	template <class C>
	constexpr auto data(C& c) -> decltype(c.data())
	{
		return c.data();
	}

	template <class C>
	constexpr auto data(const C& c) -> decltype(c.data())
	{
		return c.data();
	}

	template <class T, std::size_t N>
	constexpr T* data(T(&array)[N]) noexcept
	{
		return array.data();
	}

	template <class E>
	constexpr const E* data(std::initializer_list<E> il) noexcept
	{
		return std::begin(il);
	}

	template <class C>
	inline constexpr auto size( C&& c ) -> decltype(c.size())
	{
		return c.size();
	}

	template <class T, size_t N>
	inline constexpr size_t size( const T(&)[N] ) noexcept
	{
		return N;
	}

	template <class T, size_t N>
	inline constexpr size_t size(std::array<T,N> const&) noexcept
	{
		return N;
	}


namespace back_magic {

	template < class T, class U,
		std::enable_if_t<conjunction_v<
			is_tuple<T>, is_tuple<U>
		>, std::nullptr_t> = nullptr >
	constexpr auto make_tuple_cat(T t, U u)
	{
		return std::tuple_cat(t, u);
	}

	template < class T, class U,
		std::enable_if_t<conjunction_v<
			is_tuple<T>, negation<is_tuple<U>>
		>, std::nullptr_t> = nullptr >
	constexpr auto make_tuple_cat(T t, U u)
	{
		return std::tuple_cat(t, std::make_tuple(u));
	}

	template < class T, class U,
		std::enable_if_t<conjunction_v<
			negation<is_tuple<T>>, is_tuple<U>
		>, std::nullptr_t> = nullptr >
	constexpr auto make_tuple_cat(T t, U u)
	{
		return std::tuple_cat(std::make_tuple(t), u);
	}

	template < class T, class U,
		std::enable_if_t<conjunction_v<
			negation<is_tuple<T>>, negation<is_tuple<U>>
		>, std::nullptr_t> = nullptr >
	constexpr auto make_tuple_cat(T t, U u)
	{
		return std::make_tuple(t, u);
	}

	template < class Range >
	constexpr decltype(auto) begin(Range&& range)
	{
		using std::begin;
		return begin(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) end(Range&& range)
	{
		using std::end;
		return end(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) cbegin(Range&& range)
	{
		using std::cbegin;
		return cbegin(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) cend(Range&& range)
	{
		using std::cend;
		return cend(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) rbegin(Range&& range)
	{
		using std::rbegin;
		return rbegin(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) rend(Range&& range)
	{
		using std::rend;
		return rend(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) crbegin(Range&& range)
	{
		using std::crbegin;
		return crbegin(std::forward<Range>(range));
	}

	template < class Range >
	constexpr decltype(auto) crend(Range&& range)
	{
		using std::crend;
		return crend(std::forward<Range>(range));
	}


	template < class T >
	constexpr decltype(auto) size(T&& x) {
		using ::cranberries::size;
		return size(x);
	}

	template < class T >
	constexpr decltype(auto) data(T&& x) {
		using ::cranberries::data;
		return data(std::forward<T>(x));
	}

}

	template < class , class = void >
	struct enable_get : std::false_type{};
	
	template < class T >
	struct enable_get<T,
		cranberries::void_t<decltype(std::get<0>(std::declval<T>()))>
	> : std::true_type {};
	
	template < class T >
	constexpr bool enable_get_v = enable_get<T>::value;  
	
	struct SwallowsNest {
		constexpr SwallowsNest(...) noexcept {}
	};

namespace cranberries_magic
{
	template<class T>
	using not_ref_wrapper = negation<is_reference_wrapper<std::decay_t<T>>>;

	template <class D, class...> struct return_type_helper { using type = D; };
	template <class... Types>
	struct return_type_helper<void, Types...> : std::common_type<Types...> {
		static_assert(conjunction_v<not_ref_wrapper<Types>...>,
			"Types cannot contain reference_wrappers when D is void");
	};

	template <class D, class... Types>
	using return_type = std::array<typename return_type_helper<D, Types...>::type,
		sizeof...(Types)>;

} // ! namespace cranberries_magic


	template < class D = void, class... Types>
	inline constexpr cranberries_magic::return_type<D, Types...> make_array( Types&&... t ) {
		return{{ std::forward<Types>( t )... }};
	}

	template < typename F >
	class Finally
	{
		 F f_;
	public:
		constexpr Finally( F f ) noexcept : f_( f ) {}
		Finally() = delete;
		Finally( const Finally& ) = delete;
		Finally( Finally&& ) = delete;
		Finally& operator=( const Finally& ) = delete;
		Finally& operator=( Finally&& ) = delete;
		~Finally() noexcept { f_(); }
	};

	template < typename F >
	inline constexpr Finally<std::decay_t<F>> make_finally(F&& f){
		return {std::forward<F>(f)};
	}

	template <class F>
	struct fix_result
	{
		F f_;

		template <class ...Args>
		constexpr auto operator()(Args&& ...args) const
			noexcept(noexcept(f_(*std::declval<fix_result const *>(), std::declval<Args>()...)))
			-> decltype(f_(*std::declval<fix_result const *>(), std::declval<Args>()...))
		{
			return f_(std::move(*this), std::forward<Args>(args)...);
		}
	};

	template <class F>
	constexpr fix_result<std::decay_t<F>> fix(F&&f) noexcept
	{
		return { std::forward<F>(f) };
	}
	
 
	template< class F, class... ArgTypes >
	constexpr auto invoke(F&& f, ArgTypes&&... args)
			// exception specification for QoI
			noexcept(noexcept(_detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)))
	 -> decltype(_detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...))
	{
			return _detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
	}

namespace cranberries_magic {

	template <class F, class Tuple, std::size_t... I>
	inline constexpr decltype(auto) apply_impl( F&& f, Tuple&& t, std::index_sequence<I...> )
	{
		return cranberries::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
		// Note: std::invoke is a C++17 feature
	}

} // ! namespace cranberries_magic
 
	template <class F, class Tuple>
	inline constexpr decltype(auto) apply(F&& f, Tuple&& t)
	{
			return cranberries_magic::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
					std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
	}

	template < class Tuple, std::size_t... I >
	inline constexpr decltype(auto) tuple_print_impl(std::ostream& os, Tuple&& t, std::index_sequence<I...>)
	{
		return [&](auto&& head, auto&&... tail) {
			os << "(" << head;
			(void)std::initializer_list<int>{
				(void(os << "," << tail), 0)...
			};
			os << ")";
		}( std::get<I>(std::forward<Tuple>(t))...);
	}

	template < class Tuple >
	decltype(auto) tuple_print(Tuple&& t, std::ostream& os = std::cout) {
		return tuple_print_impl(os, std::forward<Tuple>(t),
			std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
	}

	struct PrintLine {
		template < class Head, class... Tail >
		void operator()(Head&& head, Tail&&... tail) {
			std::cout << head;
			using swallow = std::initializer_list<int>;
			(void)swallow {
				(void(std::cout << " " << tail), 0)...
			};
		}
	};

	static constexpr PrintLine println{};

	template <
		typename Range,
		typename BinaryOp,
		typename T = element_type_of_t<Range>,
		enabler_t<conjunction_v<
					is_range<Range>,
					is_callable<BinaryOp,T,T>
		>> = nullptr
	>
	inline void adjacent_for_each
	(
		Range&& range,
		BinaryOp&& f
	) {
		// for ADL
		using std::begin;
		using std::end;

		auto&& first = begin(range);
		auto&& last = end(range);

		while (std::next(first) != last)
		{
			auto&& a = *first;
			++first;
			auto&& b = *first;
			f(a, b);
		}
	}

	template < typename F >
	inline auto make_delegate(F&& f) noexcept {
		return[f = std::move(f)](auto&&... args){
			return f(std::forward<decltype(args)>(args)...);
		};
	}

	template < size_t B, size_t E >
	struct Exponent_ : size_constant<B*Exponent_<B,E-1>::value> {};
	
	template < size_t B >
	struct Exponent_<B,1> : size_constant<B> {};
	
	
	template < size_t Head, size_t... Digits >
	struct to_decimal
		: size_constant< (Head-size_t('0'))*Exponent_<10, sizeof...(Digits)>::value + to_decimal<Digits...>::value > {};
	
	template < size_t Head >
	struct to_decimal<Head>
		: size_constant< Head-size_t('0') > {};


	template<typename ...>
	struct overload;

	template<typename T>
	struct overload<T> : T {
		using T::operator();

		template<typename X>
		overload(X&& t) : T(std::forward<X>(t)) {}
	};

	template<typename T, typename U, typename ...Args>
	struct overload<T, U, Args...>
		: T
		, overload<U, Args...>
	{

		using T::operator();
		using overload<U, Args...>::operator ();

		template<typename X, typename... TArgs>
		overload(X&& t, TArgs&&... args)
			: T(std::forward<X>(t))
			, overload<U, Args...>(std::forward<TArgs>(args)...) {}

	};


	template<typename... Fs>
	overload<Fs>
		make_overload(Fs&&... funcs) {
		return { std::forward<Fs>(funcs)... };
	}

	struct protean_bool {
		constexpr operator std::true_type() const { return{}; }
		constexpr operator std::false_type() const { return{}; }
	};

namespace cranberries_magic {
	template < class T, class Tuple, size_t... I >
	constexpr T construct_from_tuple_impl(Tuple&& t, std::index_sequence<I...>) {
		return T{ std::get<I>(t)... };
	}
}

	template < class T, class Tuple >
	constexpr T construct_from_tuple(Tuple&& t) {
		return cranberries_magic::construct_from_tuple_impl<T>(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
	}

	template < class Range, class F >
	auto index_for_each(Range&& range, F&& f)
		noexcept(noexcept(is_nothrow_callable_v<std::decay_t<F>(size_t, typename std::decay_t<Range>::value_type)>))
		-> std::enable_if_t<is_callable_v<std::decay_t<F>(size_t, typename std::decay_t<Range>::value_type)>>
	{
		size_t idx{};
		for (auto&& e : range) f(idx++, e);
	}


	template < class Range, class F >
	auto index_for_each(size_t origin, Range&& range, F&& f)
		noexcept(noexcept(is_nothrow_callable_v<std::decay_t<F>(size_t, typename std::decay_t<Range>::value_type)>))
		-> std::enable_if_t<is_callable_v<std::decay_t<F>(size_t, typename std::decay_t<Range>::value_type)>>
	{
		size_t idx{origin};
		for (auto&& e : range) f(idx++, e);
	}

	template < class F, class... Args >
	decltype(auto) repeat(size_t n, F&& f, Args&&... args) {
		for (size_t i{}; i < n; ++i) std::forward<F>(f)(std::forward<Args>(args)...);
	}

	std::unique_lock<std::mutex> auto_lock(std::mutex& mtx_) {
		return std::unique_lock<std::mutex>(mtx_);
	}

	template < class, class, class = void >
	struct is_apply_callable : std::false_type {};

	template < class F, class Tuple >
	struct is_apply_callable<F,Tuple, cranberries::void_t<
			decltype(::cranberries::apply( std::declval<F>(), std::declval<Tuple>() ))
		>> : std::true_type {};

	template < class F, class Tuple >
	constexpr bool is_apply_callable_v = is_apply_callable<F, Tuple>::value;

} // ! - end namespace cranberries
#endif
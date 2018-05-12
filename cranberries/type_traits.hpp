/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_TYPE_TRAITS_HPP
#define CRANBERRIES_TYPE_TRAITS_HPP
#include <type_traits>
#include <utility>
#include <tuple>
#include <bitset>
#include <iterator>

namespace cranberries
{
	enum class endian
	{
#ifdef _WIN32
		little = 0,
		big = 1,
		native = little
#else
		little = __ORDER_LITTLE_ENDIAN__,
		big = __ORDER_BIG_ENDIAN__,
		native = __BYTE_ORDER__,
#endif
	};

	template < class T >
	struct nested_type_class {
		using type = T;
	};

	struct empty_type {};

	template < class... >
	using always_empty = empty_type;

	template < class T > class identity : nested_type_class<T> {};
	
	template < class T >
	using get_type = typename T::type;
	
	template < class T >
	constexpr auto get_value = T::value;

	template < bool B >
	using bool_constant = std::integral_constant<bool, B>;

	template < size_t S >
	using size_constant = std::integral_constant<size_t, S>;

	template < bool Pred, typename IfType = std::nullptr_t >
	using enabler_t = std::enable_if_t<Pred, IfType>;

	template < typename T, typename U, typename IfType = std::nullptr_t >
	using is_same_if_t = std::enable_if_t<std::is_same<std::decay_t<T>,std::decay_t<U>>::value,IfType>;

	template < typename T >
	using remove_cvr = std::remove_cv<std::remove_reference_t<T>>;

	template < typename T >
	using remove_cvr_t = typename remove_cvr<T>::type;

	template < typename ...Dummy >
	using void_t = void;

	template < typename... Dummy >
	using always_t = std::true_type;

	template < typename... Dummy >
	using never_t = std::false_type;

	template < class... >
	static constexpr bool false_v = false;

	template < class... >
	static constexpr bool true_v = true;

	template <class T>
	struct is_reference_wrapper : std::false_type {};
	
	template <class U>
	struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
	
	template <class T>
	constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

	template<class...> struct conjunction : std::true_type { };
	template<class B1> struct conjunction<B1> : B1 { };
	template<class B1, class... Bn>
	struct conjunction<B1, Bn...>
		: std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

	template<class...> struct disjunction : std::false_type { };
	template<class B1> struct disjunction<B1> : B1 { };
	template<class B1, class... Bn>
	struct disjunction<B1, Bn...>
		: std::conditional_t<bool(B1::value), B1, disjunction<Bn...>> { };

	template < class B >
	struct negation : bool_constant<!bool(B::value)> {};

	template < typename ...B >
	constexpr bool conjunction_v = conjunction<B...>::value;

	template < typename ...B >
	constexpr bool disjunction_v = disjunction<B...>::value;

	template < typename B >
	constexpr bool negation_v = negation<B>::value;


	template < template<class>class ...Preds >
	struct conjunctional {
		template < typename T >
		using apply = conjunction<Preds<T>...>;
	};

	template < template<class>class ...Preds >
	struct disjunctional {
		template < typename T >
		using apply = disjunction<Preds<T>...>;
	};

	template < template<class> class Pred >
	struct negational {
		template < typename T >
		using apply = negation<Pred<T>>;
	};

	template<class...>
	struct max_sizeof;

	template<class T>
	struct max_sizeof<T> : std::integral_constant<size_t, sizeof(T)> {};

	template<class Head, class...Tail>
	struct max_sizeof <Head, Tail...>
		: size_constant<
		(sizeof(Head) > max_sizeof<Tail...>::value) ? sizeof(Head) : max_sizeof<Tail...>::value
		>
	{};
	template<class...>
	struct min_sizeof;

	template<class T>
	struct min_sizeof<T> : std::integral_constant<size_t, sizeof(T)> {};

	template<class Head, class...Tail>
	struct min_sizeof <Head, Tail...>
		: size_constant<
		(sizeof(Head) > min_sizeof<Tail...>::value) ? sizeof(Head) : min_sizeof<Tail...>::value
		>
	{};

	template < class... Types >
	constexpr bool max_sizeof_v = max_sizeof<Types...>::value;

	template < class... Types >
	constexpr bool min_sizeof_v = min_sizeof<Types...>::value;


	template < typename T, typename ...Types >
	struct all_match : conjunction<std::is_same<T, Types>...> {};

	template < typename T, typename ...Types >
	constexpr bool all_match_v = all_match<T, Types...>::value;

	template < typename T, typename ...Types >
	struct any_match : disjunction<std::is_same<T, Types>...> {};

	template < typename T, typename ...Types >
	constexpr bool any_match_v = any_match<T, Types...>::value;

	template < typename T, typename ...Types >
	struct none_match : conjunction<negation<std::is_same<T, Types>>...> {};
 
	template < typename T, typename ...Types >
	constexpr bool none_match_v = none_match<T, Types...>::value;

	template < typename T, typename ...Types >
	using is_all_same = all_match<T, Types...>;

	template < typename T, typename ...Types >
	constexpr bool is_all_same_v = all_match<T, Types...>::value;

	template < template<class> class Pred, class ...Types >
	struct all_match_if : conjunction<Pred<Types>...> {};

	template < template<class> class Pred, class ...Types >
	struct any_match_if : disjunction<Pred<Types>...> {};

	template < template<class> class Pred, class ...Types >
	struct none_match_if : disjunction<negation<Pred<Types>>...> {};

	template < template<class> class Pred, class ...Types >
	constexpr bool all_match_if_v = all_match_if<Pred, Types...>::value;

	template < template<class> class Pred, class ...Types >
	constexpr bool any_match_if_v = any_match_if<Pred, Types...>::value;

	template < template<class> class Pred, class ...Types >
	constexpr bool none_match_if_v = none_match_if<Pred, Types...>::value;


	template < typename T >
	struct is_tuple : std::false_type {};

	template < typename ...Types >
	struct is_tuple<std::tuple<Types...>> : std::true_type{};

	template < typename T, size_t N >
	struct is_tuple<std::array<T,N>> : std::true_type {};

	template < typename T >
	constexpr bool is_tuple_v = is_tuple<T>::value;


	template < class T >
	struct is_bitset : std::false_type {};

	template < size_t N >
	struct is_bitset<std::bitset<N>> : std::true_type {};

	template < typename T >
	constexpr bool is_bitset_v = is_bitset<T>::value;

	template < template<class...>class T, class U >
	struct is_specialize_of : std::false_type {};

	template < template <class...>class T, template <class...>class U, class... clazz >
	struct is_specialize_of<T, U<clazz...>>: std::is_same<T<clazz...>, U<clazz...>> {};

	template < template<class...>class T, class U >
	constexpr bool is_specialize_of_v = is_specialize_of<T, U>::value;


namespace cranberries_magic{

	template < class, class = void >
	struct enable_std_begin_end : std::false_type {};

	template < typename T >
	struct enable_std_begin_end<T,
			cranberries::void_t<decltype( std::begin(std::declval<T&>()),std::end(std::declval<T&>()) )>>
	: std::true_type {};

	template < class, class = void >
	struct enable_adl_begin_end : std::false_type {};
	
	template < typename T >
	struct enable_adl_begin_end<T,
			cranberries::void_t<decltype( begin(std::declval<T&>()),end(std::declval<T&>()) )>>
	: std::true_type {};


} // ! namespace cranberries_magic

	template < typename T >
	using is_range = disjunction<
		cranberries_magic::enable_std_begin_end<T>,
		cranberries_magic::enable_adl_begin_end<T>>;

	template < typename T >
	constexpr bool is_range_v = is_range<T>::value;


	template< class, class = void >
	struct is_equality_comparable : std::false_type
	{};

	template< class T >
	struct is_equality_comparable<T,
		void_t<decltype(std::declval<T&>() == std::declval<T&>())>
	> : std::true_type
	{};


	template< class, class, class = void >
	struct is_equality_comparable_to : std::false_type
	{};

	template< typename T, typename U >
	struct is_equality_comparable_to<T, U,
		void_t<decltype(std::declval<T&>() == std::declval<U&>())>
	> : std::true_type
	{};


	template < typename T, typename U >
	constexpr bool is_equality_comparable_to_v = is_equality_comparable_to<T, U>::value;

	template < typename T >
	constexpr bool is_equality_comparable_v = is_equality_comparable<T>::value;

	namespace _detail {

		struct is_referenceable_helper {
			template <typename T>
			static T &test(int);
			template <typename T>
			static std::false_type test(...);
		};

	} // namespace _detail

	template <typename T>
	struct is_referenceable
		:public std::integral_constant<bool, !(std::is_same<decltype(_detail::is_referenceable_helper::test<T>(0)), std::false_type>::value)> {};


	template <class, class=void>
	struct is_complete_type;

	template < class T >
	struct is_complete_type < T,
		void_t<decltype(std::declval<char(&)[sizeof(T)]>()) >
	>
		:public std::true_type {};

	namespace _detail {

		template <typename T>
		struct is_unknown_bound_array
			:public std::false_type {};
		template <typename T>
		struct is_unknown_bound_array<T[]>
			:public std::true_type {};

		template <typename T>
		struct is_swappable_applicable
			:public disjunction<
			is_complete_type<T>,
			std::is_void<T>,
			is_unknown_bound_array<T>> {};

	} // namespace _detail

	namespace _detail {

		namespace _is_swappable_helper_socope {

			using std::swap;

			struct is_swappable_helper {
				template <typename T, typename U,
					typename = decltype(swap(std::declval<T>(), std::declval<U>())),
					typename = decltype(swap(std::declval<U>(), std::declval<T>()))>
					static std::true_type test(int);
				template<typename, typename, typename = void, typename = void>
				static std::false_type test(...);
			};

		} // namespace _is_swappable_helper_scope

		using _is_swappable_helper_socope::is_swappable_helper;

		template <typename T, typename U>
		struct is_swappable_checker {
			constexpr static bool value =
				std::is_same<decltype(is_swappable_helper::test<T, U>(0)), std::true_type>::value;
		};

	} // namespace _detail

	template <typename T, typename U>
	struct is_swappable_with
		:public std::integral_constant<bool, _detail::is_swappable_checker<T, U>::value> {
		static_assert(_detail::is_swappable_applicable<T>::value,
			"the argument of T would cause undefined behavior");
		static_assert(_detail::is_swappable_applicable<U>::value,
			"the argument of U would cause undefined behavior");
	};

	namespace _detail {

		template <typename T, bool is_referenceable>
		struct is_swappable_referenceable_checker;
		template <typename T>
		struct is_swappable_referenceable_checker<T, true>
			:public is_swappable_with<T &, T &> {};
		template <typename T>
		struct is_swappable_referenceable_checker<T, false>
			:public std::false_type {};

	} // namespace _detail

	template <typename T>
	struct is_swappable
		:public _detail::is_swappable_referenceable_checker<T, is_referenceable<T>::value> {};

	namespace _detail {

		namespace _is_nothrow_swappable_helper_socope {

			using std::swap;

			struct is_nothrow_swappable_helper {
				template <typename T, typename U,
					typename = decltype(swap(std::declval<T>(), std::declval<U>())),
					typename = decltype(swap(std::declval<U>(), std::declval<T>())),
					typename = typename std::enable_if<noexcept(swap(std::declval<T>(), std::declval<U>()))>::type,
					typename = typename std::enable_if<noexcept(swap(std::declval<U>(), std::declval<T>()))>::type>
					static std::true_type test(int);
				template <typename, typename, typename = void, typename = void, typename = void, typename = void>
				static std::false_type test(...);
			};

		} // namespace _is_nothrow_swappable_helper_socope

		using _is_nothrow_swappable_helper_socope::is_nothrow_swappable_helper;

		template <typename T, typename U>
		struct is_nothrow_swappable_checker {
			constexpr static bool value =
				std::is_same<decltype(is_nothrow_swappable_helper::test<T, U>(0)), std::true_type>::value;
		};

	} // namespace _detail

	template <typename T, typename U>
	struct is_nothrow_swappable_with
		:public std::integral_constant<bool, _detail::is_nothrow_swappable_checker<T, U>::value> {
		static_assert(_detail::is_swappable_applicable<T>::value,
			"the argument of T would cause undefined behavior");
		static_assert(_detail::is_swappable_applicable<U>::value,
			"the argument of U would cause undefined behavior");
	};

	namespace _detail {

		template <typename T, bool is_referenceable>
		struct is_nothrow_swappable_referenceable_checker;
		template <typename T>
		struct is_nothrow_swappable_referenceable_checker<T, true>
			:public is_nothrow_swappable_with<T &, T &> {};
		template <typename T>
		struct is_nothrow_swappable_referenceable_checker<T, false>
			:public std::false_type {};

	} // namespace _detail

	template <typename T>
	struct is_nothrow_swappable
		:public _detail::is_nothrow_swappable_referenceable_checker<T, is_referenceable<T>::value> {};

	template < class T, class U >
	constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

	template < class T>
	constexpr bool is_swappable_v = is_swappable<T>::value;

	template < class, class = void >
	struct is_dereferencable : std::false_type {};

	template < class T >
	struct is_dereferencable<T,
		void_t<decltype(*std::declval<const T&>())>>
		: std::true_type {};


	template < class, class = void >
	struct has_value_type : std::false_type
	{};
	
	template < class T >
	struct has_value_type<T,
		void_t<decltype(std::declval<typename std::decay_t<T>::value_type>())>
	> : std::true_type
	{};

	template < typename T >
	constexpr bool has_value_type_v = has_value_type<T>::value;


	template < class, class = void >
	struct has_value_field : std::false_type
	{};

	template < class T >
	struct has_value_field<T,
		void_t<decltype(std::decay_t<T>::value)>
	> : std::true_type
	{};

	template < typename T >
	constexpr bool has_value_field_v = has_value_field<T>::value;

	template < class, class = void >
	struct has_type_member : std::false_type
	{};

	template < class T >
	struct has_type_member<T,
		void_t<decltype(std::declval<typename std::decay_t<T>::type>())>
	> : std::true_type
	{};

	template < typename T >
	constexpr bool has_type_member_v = has_type_member<T>::value;

	template < class T >
	struct element_type_of
	: std::enable_if<has_value_type<T>::value, typename T::value_type>
	{};

	template < class... Ts >
	struct element_type_of<std::tuple<Ts...>>
		: std::common_type<Ts...>
	{};


	template < class T >
	struct element_type_of<T*> {
		using type = T;
	};

	template < class T, size_t N >
	struct element_type_of<T[N]> {
		using type = T;
	};

	template < class T >
	using element_type_of_t = typename element_type_of<T>::type;

	template <
		typename T,
		bool B = is_range_v<T>
	>
	struct root_element_type_of {
		using type = typename root_element_type_of<element_type_of_t<T>>::type;
	};

	template < typename T >
	struct root_element_type_of<T, false> {
		using type = T;
	};

	template < typename T >
	using root_element_type_of_t = typename root_element_type_of<T>::type;


	template < typename T >
	struct is_iterator : disjunction<
			std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>,
			std::is_base_of<std::output_iterator_tag, typename std::iterator_traits<T>::iterator_category>>
	{};

	template < typename T >
	constexpr bool is_iterator_v = is_iterator<T>::value;


	enum class return_any {};

	template <class, class R = void, class = void> struct is_callable : std::false_type {};

	template <class Fn, class... ArgTypes, class R>
	struct is_callable<Fn(ArgTypes...), R, cranberries::void_t<decltype(std::declval<Fn>()(std::declval<ArgTypes>()...))>>
		: std::is_convertible<decltype(std::declval<Fn>()(std::declval<ArgTypes>()...)), R> {};

	template <class Fn, class... ArgTypes>
	struct is_callable<Fn(ArgTypes...), return_any, cranberries::void_t<decltype(std::declval<Fn>()(std::declval<ArgTypes>()...))>>
		: std::true_type {};


	template <class, class R = void, class = void> struct is_nothrow_callable; // not defined

	template <class Fn, class... ArgTypes, class R>
	struct is_nothrow_callable<Fn(ArgTypes...), R, cranberries::void_t<decltype(std::declval<Fn>()(std::declval<ArgTypes>()...))>>
		: cranberries::bool_constant<noexcept(std::declval<Fn>()(std::declval<ArgTypes>()...))>
	{};

	template <class Fn, class... ArgTypes>
	struct is_nothrow_callable<Fn(ArgTypes...), return_any, cranberries::void_t<decltype(std::declval<Fn>()(std::declval<ArgTypes>()...))>>
		: cranberries::bool_constant<noexcept(std::declval<Fn>()(std::declval<ArgTypes>()...))>
	{};

	// variable template
	template <class T, class R = return_any>
	constexpr bool is_callable_v = is_callable<T, R>::value;

	template <class T, class R = return_any>
	constexpr bool is_nothrow_callable_v = is_nothrow_callable<T, R>::value;


	template <class T, std::size_t N>
	struct generate_tuple {
		static_assert(N!=0, "Size must be greater than zero.");
		using partial_type = typename generate_tuple<T, N / 2>::type;
		using type = std::conditional_t<N % 2 == 0
			,decltype(std::tuple_cat(std::declval<partial_type>(), std::declval<partial_type>()))
			,decltype(std::tuple_cat(std::declval<partial_type>(), std::declval<partial_type>(), std::declval<std::tuple<T>>()))>;
	};

	template <class T>
	struct generate_tuple<T, 1>
	{
		using type = std::tuple<T>;
	};


	template < typename T, std::size_t N >
	using generate_tuple_t = typename generate_tuple<T,N>::type;

namespace _detail {
 
	template <class R, class Base, class T, class Derived, class... Args>
	constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
		noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
											  std::is_base_of<Base, std::decay_t<Derived>>::value &&
												!std::is_void<remove_cvr_t<R>>::value,R>
	{
		return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
	}

	template <class R, class Base, class T, class Derived, class... Args>
	constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
		noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
											  std::is_base_of<Base, std::decay_t<Derived>>::value &&
												std::is_void<remove_cvr_t<R>>::value>
	{
		return static_cast<void>((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...));
	}
 
	template <class R, class Base, class T, class RefWrap, class... Args>
	constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
		noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
											 is_reference_wrapper_v<std::decay_t<RefWrap>> &&
											 !std::is_void<remove_cvr_t<R>>::value,R>
 
	{
		return (ref.get().*pmf)(std::forward<Args>(args)...);
	}

	template <class R, class Base, class T, class RefWrap, class... Args>
	constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
		noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
											 is_reference_wrapper_v<std::decay_t<RefWrap>> &&
											 std::is_void<remove_cvr_t<R>>::value>
 
	{
		return static_cast<void>((ref.get().*pmf)(std::forward<Args>(args)...));
	}
 
	template <class R, class Base, class T, class Pointer, class... Args>
	constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
		noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
											 !is_reference_wrapper_v<std::decay_t<Pointer>> &&
											 !std::is_base_of<Base, std::decay_t<Pointer>>::value &&
											 !std::is_void<remove_cvr_t<R>>::value,R>
	{
		return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
	}

	template <class R, class Base, class T, class Pointer, class... Args>
	constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
		noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
											 !is_reference_wrapper_v<std::decay_t<Pointer>> &&
											 !std::is_base_of<Base, std::decay_t<Pointer>>::value &&
											 std::is_void<remove_cvr_t<R>>::value>
	{
		return static_cast<void>(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...));
	}

 
	template <class R, class Base, class T, class Derived>
	constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
		noexcept(noexcept(std::forward<Derived>(ref).*pmd))
		-> std::enable_if_t<!std::is_function<T>::value &&
											 std::is_base_of<Base, std::decay_t<Derived>>::value &&
											 !std::is_void<remove_cvr_t<R>>::value,R>
	{
		return std::forward<Derived>(ref).*pmd;
	}

	template <class R, class Base, class T, class Derived>
	constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
		noexcept(noexcept(std::forward<Derived>(ref).*pmd))
		-> std::enable_if_t<!std::is_function<T>::value &&
											  std::is_base_of<Base, std::decay_t<Derived>>::value &&
											  std::is_void<remove_cvr_t<R>>::value>
	{
		return static_cast<void>(std::forward<Derived>(ref).*pmd);
	}
 
	template <class R, class Base, class T, class RefWrap>
	constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
		noexcept(noexcept(ref.get().*pmd))
		-> std::enable_if_t<!std::is_function<T>::value &&
											  is_reference_wrapper_v<std::decay_t<RefWrap>> &&
											  !std::is_void<remove_cvr_t<R>>::value,R>
	{
		return ref.get().*pmd;
	}

	template <class R, class Base, class T, class RefWrap>
	constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
		noexcept(noexcept(ref.get().*pmd))
	-> std::enable_if_t<!std::is_function<T>::value &&
											is_reference_wrapper_v<std::decay_t<RefWrap>> &&
											std::is_void<remove_cvr_t<R>>::value>
	{
		return static_cast<void>(ref.get().*pmd);
	}

 
	template <class R, class Base, class T, class Pointer>
	constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
			noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
	 -> std::enable_if_t<!std::is_function<T>::value &&
											 !is_reference_wrapper_v<std::decay_t<Pointer>> &&
											 !std::is_base_of<Base, std::decay_t<Pointer>>::value &&
											 !std::is_void<remove_cvr_t<R>>::value,R>
	{
		return (*std::forward<Pointer>(ptr)).*pmd;
	}

	template <class R, class Base, class T, class Pointer>
	constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
			noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
	 -> std::enable_if_t<!std::is_function<T>::value &&
											 !is_reference_wrapper_v<std::decay_t<Pointer>> &&
											 !std::is_base_of<Base, std::decay_t<Pointer>>::value &&
											 std::is_void<remove_cvr_t<R>>::value>
	{
		return static_cast<void>((*std::forward<Pointer>(ptr)).*pmd);
	}

 
	template <class R, class F, class... Args>
	constexpr auto INVOKE(F&& f, Args&&... args)
			noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
	 -> std::enable_if_t<
            !std::is_member_pointer<std::decay_t<F>>::value &&
            !std::is_void<remove_cvr_t<R>>::value,
        R>
	{
		return std::forward<F>(f)(std::forward<Args>(args)...);
	}

	template <class R, class F, class... Args>
	constexpr auto INVOKE(F&& f, Args&&... args)
		noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
	 -> std::enable_if_t<
            !std::is_member_pointer<std::decay_t<F>>::value &&
            std::is_void<remove_cvr_t<R>>::value
        >
	{
		return static_cast<void>(std::forward<F>(f)(std::forward<Args>(args)...));
	}

	template <class Base, class T, class Derived, class... Args>
	constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
		noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
		std::is_base_of<Base, std::decay_t<Derived>>::value,
		decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>
	{
		return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
	}

	template <class Base, class T, class RefWrap, class... Args>
	constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
		noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
		is_reference_wrapper_v<std::decay_t<RefWrap>>,
		decltype((ref.get().*pmf)(std::forward<Args>(args)...))>

	{
		return (ref.get().*pmf)(std::forward<Args>(args)...);
	}

	template <class Base, class T, class Pointer, class... Args>
	constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
		noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
		-> std::enable_if_t<std::is_function<T>::value &&
		!is_reference_wrapper_v<std::decay_t<Pointer>> &&
		!std::is_base_of<Base, std::decay_t<Pointer>>::value,
		decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>
	{
		return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
	}

	template <class Base, class T, class Derived>
	constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
		noexcept(noexcept(std::forward<Derived>(ref).*pmd))
		-> std::enable_if_t<!std::is_function<T>::value &&
		std::is_base_of<Base, std::decay_t<Derived>>::value,
		decltype(std::forward<Derived>(ref).*pmd)>
	{
		return std::forward<Derived>(ref).*pmd;
	}

	template <class Base, class T, class RefWrap>
	constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
		noexcept(noexcept(ref.get().*pmd))
		-> std::enable_if_t<!std::is_function<T>::value &&
		is_reference_wrapper_v<std::decay_t<RefWrap>>,
		decltype(ref.get().*pmd)>
	{
		return ref.get().*pmd;
	}

	template <class Base, class T, class Pointer>
	constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
		noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
		-> std::enable_if_t<!std::is_function<T>::value &&
		!is_reference_wrapper_v<std::decay_t<Pointer>> &&
		!std::is_base_of<Base, std::decay_t<Pointer>>::value,
		decltype((*std::forward<Pointer>(ptr)).*pmd)>
	{
		return (*std::forward<Pointer>(ptr)).*pmd;
	}

	template <class F, class... Args>
	constexpr auto INVOKE(F&& f, Args&&... args)
		noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
		-> std::enable_if_t<!std::is_member_pointer<std::decay_t<F>>::value,
		decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
	{
		return std::forward<F>(f)(std::forward<Args>(args)...);
	}
} // ! namespace _detail


namespace _detail {
	struct failure_t {};
	template < class T >
	struct invoke_result_inject { using type = T; };
	template < >
	struct invoke_result_inject<failure_t> {};

	template<class AlwaysVoid, class... Types>
	struct invoke_traits
	{
		using type = failure_t;
		using is_invocable_impl = std::false_type;
		using is_nothrow_invocable_impl = std::false_type;
		template<class Rx>
		using is_invocable_r_impl = std::false_type;
		template<class Rx>
		using is_nothrow_invocable_r_impl = std::false_type;
	};

	template<class... Types>
	struct invoke_traits<
		::cranberries::void_t<decltype(::cranberries::_detail::INVOKE(std::declval<Types>()...))>,
		Types...>
	{
		using type = decltype(::cranberries::_detail::INVOKE(std::declval<Types>()...));
		using is_invocable_impl = std::true_type;
		using is_nothrow_invocable_impl = ::cranberries::bool_constant<noexcept(::cranberries::_detail::INVOKE(std::declval<Types>()...))>;
		template<class Rx>
		using is_invocable_r_impl = ::cranberries::disjunction<std::is_void<Rx>, std::is_convertible<type, Rx>>;
		template<class Rx>
		using is_nothrow_invocable_r_impl = ::cranberries::disjunction<
			is_nothrow_invocable_impl,
			::cranberries::disjunction<std::is_void<Rx>, ::cranberries::is_nothrow_convertible<type, Rx>>>;
	};

}

template <class F, class... ArgTypes>
struct invoke_result : _detail::invoke_result_inject<typename _detail::invoke_traits<void, F, ArgTypes...>::type> {};

template < class F, class... ArgTypes >
struct is_invocable : _detail::invoke_traits<void, F, ArgTypes...>::is_invocable_impl {};

template < class R, class F, class... ArgTypes >
struct is_invocable_r : _detail::invoke_traits<void, F, ArgTypes...>:: template is_invocable_r_impl<R> {};

template < class F, class... ArgTypes >
struct is_nothrow_invocable : _detail::invoke_traits<void, F, ArgTypes...>::is_nothrow_invocable_impl {};

template < class R, class F, class... ArgTypes >
struct is_nothrow_invocable_r : _detail::invoke_traits<void, F, ArgTypes...>:: template is_nothrow_invocable_r_impl<R> {};


template <class F, class... ArgTypes>
using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

template < class F, class... ArgTypes >
constexpr bool is_invocable_v = is_invocable<F, ArgTypes...>::value;

template < class R, class F, class... ArgTypes >
constexpr bool is_invocable_r_v = is_invocable_r<R, F, ArgTypes...>::value;

template < class F, class... ArgTypes >
constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<F, ArgTypes...>::value;

template < class R, class F, class... ArgTypes >
constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, F, ArgTypes...>::value;

}

#endif // !CRANBERRIES_TYPE_TRAITS_HPP

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
  struct is_specialize_of<T, U<clazz...>> {
    static constexpr bool value = std::is_same<T<clazz...>, U<clazz...>>::value;
  };

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
    void_t<decltype(std::declval<std::decay_t<T>::value>())>
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

  template <
    typename T,
    bool A = std::is_array<remove_cvr_t<T>>::value,
    bool B = is_tuple_v<remove_cvr_t<T>>
  >
  struct element_type_of
  {
    using type = typename remove_cvr_t<T>::value_type;
  };

  template <
    typename T
  >
  struct element_type_of<T, true, false>
  {
    using type = std::remove_extent_t<remove_cvr_t<T>>;
  };

  template <
    typename T
  >
  struct element_type_of<T, false, true>
  {
    using type = typename std::tuple_element_t<0, remove_cvr_t<T>>;
  };


  template < typename T >
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

}

#endif // !CRANBERRIES_TYPE_TRAITS_HPP

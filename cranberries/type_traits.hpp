#ifndef CRANBERRIES_TYPE_TRAITS_HPP
#define CRANBERRIES_TYPE_TRAITS_HPP
#include <type_traits>
#include <utility>

namespace cranberries
{
  template < bool Pred, typename IfType = std::nullptr_t >
  using enabler_t = std::enable_if_t<Pred, IfType>;

  template < typename T, typename U, typename IfType = std::nullptr_t >
  using is_same_if_t = std::enable_if_t<std::is_same<std::decay_t<T>,std::decay_t<U>>::value,IfType>;

  template < typename T >
  struct remove_cvr {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
  };

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

namespace cranberries_magic
{
  template < typename Head, typename ...Tail >
  struct conj_impl : bool_constant<Head::value && conj_impl<Tail...>::value> {};

  template < typename B >
  struct conj_impl<B> : bool_constant<B::value> {};

  template < typename Head, typename ...Tail >
  struct disj_impl : bool_constant<Head::value || conj_impl<Tail...>::value> {};

  template < typename B >
  struct disj_impl<B> : bool_constant<B::value> {};
} // ! namespace cranberries_magic

  template < typename ...B >
  struct conjunction : cranberries_magic::conj_impl<B...> {};

  template < typename ...B >
  struct disjunction : cranberries_magic::disj_impl<B...> {};

  template < typename B >
  struct negation : bool_constant<!B::value> {};


  template < typename ...B >
  constexpr bool conjunction_v = conjunction<B...>::value;

  template < typename ...B >
  constexpr bool disjunction_v = disjunction<B...>::value;

  template < typename B >
  constexpr bool negation_v = negation<B>::value;


  template < typename T, template<class> class Head, template<class> class ...Tail >
  struct conjunctional_impl : bool_constant< Head<T>::value && conjunctional_impl<T, Tail...>::value > {};

  template < typename T, template<class> class Pred >
  struct conjunctional_impl<T,Pred> : bool_constant< Pred<T>::value > {};

namespace cranberries_magic{
  template < typename T, template<class> class Head, template<class> class ...Tail >
  struct disjunctional_impl : bool_constant< Head<T>::value && conjunctional_impl<T, Tail...>::value > {};

  template < typename T, template<class> class Pred >
  struct disjunctional_impl<T, Pred> : bool_constant< Pred<T>::value > {};

  template < typename T, template<class> class Pred >
  struct negational_impl : bool_constant<!Pred<T>::value> {};
} // ! namespace cranberries_magic

  template <
    template<class>class ...Preds
  >
  struct conjunctional
  {
    template < typename T >
    using type = cranberries_magic::conjunctional_impl<T, Preds...>;
  };

  template < template<class>class ...Preds >
  struct disjunctional
  {
    template < typename T >
    using type = cranberries_magic::conjunctional_impl<T, Preds...>;
  };

  template < template<class> class Pred >
  struct negational
  {
    template < typename T >
    using type = cranberries_magic::negational_impl<T, Pred>;
  };


  template < typename T, typename Tuple >
  struct tuple_all_match;

  template < typename T >
  struct tuple_all_match<T, std::tuple<>> : std::false_type {};

  template < typename T, typename U, typename ...Rest >
  struct tuple_all_match<T, std::tuple<U, Rest...>> : std::false_type {};

  template < typename T, typename ...Rest >
  struct tuple_all_match<T, std::tuple<T, Rest...>> : tuple_all_match<T, std::tuple<Rest...>> {};

  template < typename T >
  struct tuple_all_match<T, std::tuple<T>> : std::true_type {};

  template < typename T, typename Tuple >
  struct tuple_any_match;

  template < typename T >
  struct tuple_any_match<T, std::tuple<>> : std::false_type {};

  template < typename T, typename U, typename ...Rest >
  struct tuple_any_match<T, std::tuple<U, Rest...>> : tuple_any_match<T, std::tuple<Rest...>> {};

  template < typename T, typename ...Rest >
  struct tuple_any_match<T, std::tuple<T, Rest...>> : std::true_type {};

  template < typename T, typename Tuple >
  struct tuple_none_match;

  template < typename T >
  struct tuple_none_match<T, std::tuple<>> : std::false_type {};

  template < typename T, typename U, typename ...Rest >
  struct tuple_none_match<T, std::tuple<U, Rest...>> : tuple_none_match<T, std::tuple<Rest...>> {};

  template < typename T, typename ...Rest >
  struct tuple_none_match < T, std::tuple<T, Rest...>> : std::false_type {};

  template < typename T, typename U >
  struct tuple_none_match<T, std::tuple<U>> : std::true_type {};

  template < template <class> class Pred, typename Tuple >
  struct tuple_all_match_if;

  template < template <class> class Pred >
  struct tuple_all_match_if<Pred, std::tuple<>> : std::false_type {};

  template < template <class> class Pred, typename T, typename ...Rest >
  struct tuple_all_match_if<Pred, std::tuple<T, Rest...>> : std::conditional_t<Pred<T>::value, tuple_all_match_if<Pred,std::tuple<Rest...>>,std::false_type> {};

  template < template <class> class Pred, typename T >
  struct tuple_all_match_if<Pred, std::tuple<T>> : std::conditional_t<Pred<T>::value, std::true_type, std::false_type> {};

  template < template <class> class Pred, typename Tuple >
  struct tuple_any_match_if;

  template < template <class> class Pred >
  struct tuple_any_match_if<Pred, std::tuple<>> : std::false_type {};

  template < template <class> class Pred, typename T, typename ...Rest >
  struct tuple_any_match_if<Pred, std::tuple<T, Rest...>> : std::conditional_t<Pred<T>::value, std::true_type, tuple_any_match_if<Pred, std::tuple<Rest...>>> {};

  template < template <class> class Pred, typename T >
  struct tuple_any_match_if<Pred, std::tuple<T>> : std::conditional_t<Pred<T>::value, std::true_type, std::false_type> {};

  template < template <class> class Pred, typename Tuple >
  struct tuple_none_match_if;

  template < template <class> class Pred >
  struct tuple_none_match_if<Pred, std::tuple<>> : std::false_type {};

  template < template <class> class Pred, typename T, typename ...Rest >
  struct tuple_none_match_if<Pred, std::tuple<T, Rest...>> : std::conditional_t<!Pred<T>::value, tuple_all_match_if<Pred, std::tuple<Rest...>>, std::false_type> {};

  template < template <class> class Pred, typename T >
  struct tuple_none_match_if<Pred, std::tuple<T>> : std::conditional_t<!Pred<T>::value, std::true_type, std::false_type> {};


  template < typename T, class Tuple >
  constexpr bool tuple_all_match_v = tuple_all_match<T, Tuple>::value;

  template < typename T, class Tuple >
  constexpr bool tuple_any_match_v = tuple_any_match<T, Tuple>::value;
  
  template < typename T, class Tuple >
  constexpr bool tuple_none_match_v = tuple_none_match<T, Tuple>::value;

  template < template<class> class Pred, class Tuple >
  constexpr bool tuple_all_match_if_v = tuple_all_match_if<Pred, Tuple>::value;

  template < template<class> class Pred, class Tuple >
  constexpr bool tuple_any_match_if_v = tuple_any_match_if<Pred, Tuple>::value;

  template < template<class> class Pred, class Tuple >
  constexpr bool tuple_none_match_if_v = tuple_none_match_if<Pred, Tuple>::value;

  template < typename T, typename ...Args >
  struct all_match : tuple_all_match<T, std::tuple<Args...>> {};

  template < typename T, typename ...Args >
  struct any_match : tuple_any_match<T, std::tuple<Args...>> {};

  template < typename T, typename ...Args >
  struct none_match : tuple_none_match<T, std::tuple<Args...>> {};

  template < template<class> class Pred, typename ...Args >
  struct all_match_if : tuple_all_match_if<Pred, std::tuple<Args...>> {};

  template < template<class> class Pred, typename ...Args >
  struct any_match_if : tuple_any_match_if<Pred, std::tuple<Args...>> {};

  template < template<class> class Pred, typename ...Args >
  struct none_match_if : tuple_none_match_if<Pred, std::tuple<Args...>> {};


  template < typename T, typename ...Args >
  constexpr bool all_match_v = all_match<T, Args...>::value;

  template < typename T, typename ...Args >
  constexpr bool any_match_v = any_match<T, Args...>::value;

  template < typename T, typename ...Args >
  constexpr bool none_match_v = none_match<T, Args...>::value;

  template < template<class> class Pred, typename ...Args >
  constexpr bool all_match_if_v = all_match_if<Pred, Args...>::value;

  template < template<class> class Pred, typename ...Args >
  constexpr bool any_match_if_v = any_match_if<Pred, Args...>::value;

  template < template<class> class Pred, typename ...Args >
  constexpr bool none_match_if_v = none_match_if<Pred, Args...>::value;

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
  struct is_bitset : is_bitset<T> {};

  template < typename T >
  constexpr bool is_bitset_v = is_bitset<T>::value;


namespace cranberries_magic{

  template < class, class = void >
  struct enable_std_begin_end : std::false_type {};
  
  template < typename T >
  struct enable_std_begin_end<T,
      std::void_t<decltype( std::begin(std::declval<const T&>()),std::end(std::declval<const T&>()) )>>
  : std::true_type {};

  template < class, class = void >
  struct enable_adl_begin_end : std::false_type {};
  
  template < typename T >
  struct enable_adl_begin_end<T,
      std::void_t<decltype( begin(std::declval<const T&>()),end(std::declval<const T&>()) )>>
  : std::true_type {};


} // ! namespace cranberries_magic

  template < typename T >
  using is_range = typename disjunctional<
      cranberries_magic::enable_std_begin_end,
      cranberries_magic::enable_adl_begin_end>::template type<T>;

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
    static_assert(
      tuple_all_match<
        std::tuple_element_t<0, remove_cvr_t<T>>, remove_cvr_t<T>
      >::value,
      "tuple"
    );
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
  template <class T, class R = void>
  constexpr bool is_callable_v = is_callable<T, R>::value;

  template <class T, class R = void>
  constexpr bool is_nothrow_callable_v = is_nothrow_callable<T, R>::value;


  template < typename T, std::size_t N >
  struct generate_tuple
  {
    template < std::size_t ...I >
    auto seq(std::index_sequence<I...>)
      -> std::tuple<std::decay_t< decltype(I,std::declval<T>())>... >;
    
    using type = decltype(seq(std::make_index_sequence<N>());
  };

  template < typename T, std::size_t N >
  using generate_tuple_t = typename generate_tuple<T,N>::type;

}

#endif // !CRANBERRIES_TYPE_TRAITS_HPP
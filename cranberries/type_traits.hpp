#ifndef CRANBERRIES_TYPE_TRAITS_HPP
#define CRANBERRIES_TYPE_TRAITS_HPP
#include <type_traits>
#include <utility>
#include <tuple>
#include <bitset>
#include <iterator>

namespace cranberries
{
  template < bool B >
  using bool_constant = std::integral_constant<bool, B>;

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
  struct disj_impl : bool_constant<Head::value || disj_impl<Tail...>::value> {};

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


  template < template<class>class ...Preds >
  struct conjunctional {
    template < typename T >
    using pred = conjunction<Preds<T>...>;
  };

  template < template<class>class ...Preds >
  struct disjunctional {
    template < typename T >
    using pred = disjunction<Preds<T>...>;
  };

  template < template<class> class Pred >
  struct negational {
    template < typename T >
    using pred = negation<Pred<T>>;
  };

  template < template<class...> class Pred, typename ...Types >
  struct apply_ {
    using type = Pred<Types...>;
  };

  template < template<class...> class Pred, typename ...Types >
  struct apply_decay {
    using type = Pred<std::decay_t<Types>...>;
  };

  template < template<class...> class Pred, typename ...Types >
  struct apply_remove_cvr {
    using type = Pred<remove_cvr_t<Types>...>;
  };

  template < template<class...> class Pred, typename ...Types >
  using apply_t = typename apply_<Pred, Types...>::type;

  template < template<class...> class Pred, typename ...Types >
  using apply_decay_t = typename apply_decay<Pred, Types...>::type;

  template < template<class...> class Pred, typename ...Types >
  using apply_remove_cvr_t = typename apply_remove_cvr<Pred, Types...>::type;

  template < template<class...> class Pred, typename ...Types >
  using apply_result_t = typename apply_t<Pred, Types...>::type;

  template < template<class...> class Pred, typename ...Types >
  constexpr bool apply_result_v = apply_t<Pred, Types...>::value;

  template < template<class...> class Pred, typename ...Types >
  using apply_decay_result_t = typename apply_decay_t<Pred, Types...>::type;

  template < template<class...> class Pred, typename ...Types >
  constexpr bool apply_decay_result_v = apply_decay_t<Pred, Types...>::value;

  template < template<class...> class Pred, typename ...Types >
  using apply_remove_cvr_result_t = typename apply_remove_cvr_t<Pred, Types...>::type;

  template < template<class...> class Pred, typename ...Types >
  constexpr bool apply_remove_cvr_result_v = apply_remove_cvr_t<Pred, Types...>::value;

namespace cranberries_magic {

  struct meta_bind_placeholders {};

  template < typename T >
  constexpr bool is_placeholder_v = std::is_base_of<meta_bind_placeholders, std::decay_t<T>>::value;

  template < class ...Types >
  struct pack {
    template < template<class...>class Expr >
    using expr = Expr<Types...>;
  };

  template < class >
  struct pack_split {};

  template < class Head, class ...Tail >
  struct pack_split<pack<Head, Tail...>> {
    using head = pack<Head>;
    using tail = pack<Tail...>;
  };

  template < >
  struct pack_split<pack<>> {
    using head = pack<>;
    using tail = pack<>;
  };

  template < class Pack1, class Pack2 >
  struct pack_cat {
    template < typename ...L, typename ...R >
    static constexpr pack<L..., R...> cat(pack<L...>, pack<R...>);

    using type = decltype(cat(std::declval<Pack1>(), std::declval<Pack2>()));
  };

  template < class Pack1, class Pack2 >
  using pack_cat_t = typename pack_cat<Pack1, Pack2>::type;

  template < class Tuple >
  using pack_head = typename pack_split<Tuple>::head;

  template < class Tuple >
  using pack_tail = typename pack_split<Tuple>::tail;

  template < class >
  struct pack_if {};

  template < class Head, class ...Tail >
  struct pack_if<pack<Head, Tail...>> {
    static constexpr bool value = is_placeholder_v<Head>;
  };

  template < class X, class A, class Result = pack<> >
  struct expansion {
    using type = std::conditional_t< pack_if<X>::value,
      typename expansion< pack_tail<X>, pack_tail<A>, pack_cat_t< Result, pack_head<A> > >::type,
      typename expansion< pack_tail<X>, A, pack_cat_t< Result, pack_head<X> > >::type>;
  };


  template < class ...A, class Result >
  struct expansion<pack<>, pack<A...>, Result> {
    using type = Result;
  };

} // ! namespace cranberries_magic

  class x_ : cranberries_magic::meta_bind_placeholders {};

  template < template<class...> class Expr, typename ...Types >
  struct bind_ {
    template < typename ...Apply >
    using expr = typename cranberries_magic::expansion<
      cranberries_magic::pack<Types...>, cranberries_magic::pack<Apply...>
    >::type:: template expr<Expr>;
  };

  template < template<class...> class Expr, typename ...Types >
  struct bind_single {
    template < typename Apply >
    using expr = typename cranberries_magic::expansion<
      cranberries_magic::pack<Types...>, cranberries_magic::pack<Apply>
    >::type:: template expr<Expr>;
  };

  template < template<class...> class Pred, class ...Types>
  using bind_1st = bind_single<Pred, x_, Types...>;

  template < template<class...> class Pred, class T, class ...Types >
  using bind_2nd = bind_single<Pred, T, x_, Types...>;


  template < typename T, typename ...Types >
  struct all_match : conjunction<std::is_same<T, Types>...> {};

  template < typename T, typename ...Types >
  struct any_match : disjunction<std::is_same<T, Types>...> {};

  template < typename T, typename ...Types >
  struct none_match : conjunction<negation<std::is_same<T, Types>>...> {};

  template < typename T, typename ...Types >
  using all_same = all_match<T, Types...>;

  template < template<class> class Pred, class ...Types >
  struct all_match_if : conjunction<Pred<Types>...> {};

  template < template<class> class Pred, class ...Types >
  struct any_match_if : disjunction<Pred<Types>...> {};

  template < template<class> class Pred, class ...Types >
  struct none_match_if : disjunction<negation<Pred<Types>>...> {};
  
namespace cranberries_magic {
  template < template<class...> class Pred, class T, class Tuple >
  struct tuple_match_impl {
    template < class U, class ...Types >
    static constexpr auto x(std::tuple<Types...>)->Pred<U, Types...>;
    
    using type = decltype(x(std::declval<Tuple>()));
  };
  template < template<template<class>class, class...> class Match, template <class> class Pred, class Tuple >
  struct tuple_match_if_impl {
    template < template<class> class P, class ...Types >
    static constexpr auto x(std::tuple<Types...>)->Match<Pred, Types...>;
    
    using type = decltype(x(std::declval<Tuple>()));
  };
}
  template < typename T, typename Tuple >
  using tuple_all_match = typename cranberries_magic::tuple_match_impl<all_match,T, Tuple>::type;

  template < class Tuple >
  using tuple_all_same = tuple_all_match<std::tuple_element_t<0, Tuple>, Tuple>;

  template < typename T, typename Tuple >
  using tuple_any_match = typename cranberries_magic::tuple_match_impl<any_match, T, Tuple>::type;

  template < typename T, typename Tuple >
  using tuple_none_match = typename cranberries_magic::tuple_match_impl<none_match, T, Tuple>::type;
    
  template < template<class> class Pred, typename Tuple >
  using tuple_all_match_if = typename cranberries_magic::tuple_match_if_impl<all_match_if, Pred, Tuple>::type;

  template < template<class> class Pred, typename Tuple >
  using tuple_any_match_if = typename cranberries_magic::tuple_match_if_impl<any_match_if, Pred, Tuple>::type;

  template < template<class> class Pred, typename Tuple >
  using tuple_none_match_if = typename cranberries_magic::tuple_match_if_impl<none_match_if, Pred, Tuple>::type;

  template < typename T, class Tuple >
  constexpr bool tuple_all_match_v = tuple_all_match<T, Tuple>::value;

  template < class Tuple >
  constexpr bool tuple_all_same_v = tuple_all_same<Tuple>::value;

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
  using is_range = disjunctional<
    bind_1st<cranberries_magic::enable_std_begin_end>::expr,
    bind_1st<cranberries_magic::enable_adl_begin_end>::expr>::pred<T>;

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

  template <class T, int N>
  struct generate_tuple {
    using partial_type = typename generate_tuple<T, N / 2>::type;
    using type = std::conditional_t<N % 2 == 0
      ,decltype(std::tuple_cat(std::declval<partial_type>(), std::declval<partial_type>()))
      ,decltype(std::tuple_cat(std::declval<partial_type>(), std::declval<partial_type>(), std::declval<std::tuple<T>>()))>;
  };

  template <class T>
  struct generate_tuple<T,1>
  {
    using type = std::tuple<T>;
  };
  template < typename T, std::size_t N >
  using generate_tuple_t = typename generate_tuple<T,N>::type;

}

#endif // !CRANBERRIES_TYPE_TRAITS_HPP

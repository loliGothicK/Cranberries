#ifndef CRANBERRIES_PACK_TRAITS_HPP
#define CRANBERRIES_PACK_TRAITS_HPP
#include <type_traits>
#include "type_traits.hpp"

namespace cranberries {

  template < class >
  struct is_type_pack : std::false_type {};

  template < template < class... > class T, class... Types >
  struct is_type_pack<T<Types...>> : std::true_type {};

  template < class T >
  constexpr bool is_type_pack_v = is_type_pack<T>::value;

  template < class >
  struct is_value_pack : std::false_type {};

  template < template < class Ty, Ty... > class T, class Type, Type... Values >
  struct is_value_pack<T<Type, Values...>> : std::true_type {};

  template < class T >
  constexpr bool is_value_pack_v = is_type_pack<T>::value;

  template < class T >
  struct is_pack 
    : disjunction<is_type_pack<T>, is_value_pack<T>>
  {};

  template < class T >
  constexpr bool is_pack_v = is_pack<T>::value;


  template < class ...Types >
  struct type_pack {
    template < template<class...>class Expr >
    using expr = Expr<Types...>;
  };

  template < class T, T ...Args >
  struct value_pack {};


  template< std::size_t I, class T >
  struct pack_element;

  // recursive case
  template< std::size_t I, template < class... > class Pack, class Head, class... Tail >
  struct pack_element<I, Pack<Head, Tail...>>
    : pack_element<I - 1, Pack<Tail...>>
  {
    static_assert(I <= sizeof...(Tail), "pack out of range.");
  };

  // base case
  template< template < class... > class Pack, class Head, class... Tail >
  struct pack_element<0, Pack<Head, Tail...>> {
    using type = Head;
  };

  template < size_t N, typename Pack >
  using pack_element_t = typename pack_element<N, Pack>::type;

  template < class >
  struct pack_size;

  template < template< class... > class Pack, class... Types >
  struct pack_size<Pack<Types...>>
  {
    static constexpr bool value = sizeof...(Types);
  };

  template < template < class Ty, Ty... > class Pack, class T, T... Values >
  struct pack_size<Pack<T, Values...>>
  {
    static constexpr bool value = sizeof...(Values);
  };


  template < class >
  struct pack_middle_point;

  template < template < class... > class Pack, class... Types >
  struct pack_middle_point<Pack<Types...>>
  {
    static constexpr bool value = sizeof...(Types) > 0 ? sizeof...(Types) / 2 - 1 : 0;
  };

  template < template < class T, T... > class Pack, class Ty, Ty... Values >
  struct pack_middle_point<Pack<Ty, Values...>>
  {
    static constexpr bool value = sizeof...(Values) > 0 ? sizeof...(Values) / 2 - 1 : 0;
  };


  template < class Pack >
  constexpr bool pack_size_v = pack_size<Pack>::value;

  template < class Pack >
  constexpr bool pack_middle_point_v = pack_middle_point<Pack>::value;



namespace cranberries_magic {

  template < size_t, class, class >
  struct pack_slicer_impl;

  template < size_t I, template < class... > class Pack, class Head, class... Left, class... Right >
  struct pack_slicer_impl<I, Pack<Left...>, Pack<Head, Right...>>
    : pack_slicer_impl<I-1, Pack<Left..., Head>, Pack<Right...>>
  {};

  template < template < class... > class Pack, class Head, class... Left, class... Right >
  struct pack_slicer_impl<0, Pack<Left...>, Pack<Head, Right...>>
  {
    using left_pack = Pack<Left..., Head>;
    using right_pack = Pack<Right...>;
  };

  template < template < class... > class Pack >
  struct pack_slicer_impl<0, Pack<>, Pack<>>
  {
    using left_pack = Pack<>;
    using right_pack = Pack<>;
  };


  template < size_t I, template < class Ty, Ty... > class Pack, class T, T Head, T... Left, T... Right >
  struct pack_slicer_impl<I, Pack<T, Left...>, Pack<T, Head, Right...>>
    : pack_slicer_impl<I-1, Pack<T, Left..., Head>, Pack<T, Right...>>
  {};

  template < template < class Ty, Ty... > class Pack, class T, T Head, T... Left, T... Right >
  struct pack_slicer_impl<0, Pack<T, Left...>, Pack<T, Head, Right...>>
  {
    using left_pack = Pack<T, Left..., Head>;
    using right_pack = Pack<T, Right...>;
  };

  template < template < class Ty, Ty... > class Pack, class T >
  struct pack_slicer_impl<0, Pack<T>, Pack<T>>
  {
    using right_pack = Pack<T>;
    using left_pack = Pack<T>;
  };

}

  template < size_t, class >
  struct pack_slicer {};

  template < size_t I, template < class... > class Pack, class ...Types >
  struct pack_slicer<I, Pack<Types...>> {
    using right = typename cranberries_magic::pack_slicer_impl<I, Pack<>, Pack<Types...>>::right_pack;
    using left = typename cranberries_magic::pack_slicer_impl<I, Pack<>, Pack<Types...>>::left_pack;
  };

  template < size_t I, template < class Ty, Ty... > class Pack, class T, T... Values >
  struct pack_slicer<I, Pack<T, Values...>> {
    using right = typename cranberries_magic::pack_slicer_impl<I, Pack<T>, Pack<T,Values...>>::right_pack;
    using left = typename cranberries_magic::pack_slicer_impl<I, Pack<T>, Pack<T, Values...>>::left_pack;
  };

  template < size_t I, class Pack >
  using pack_sliced_l = typename pack_slicer<I, Pack>::left;

  template < size_t I, class Pack >
  using pack_sliced_r = typename pack_slicer<I, Pack>::right;

  template < class Pack >
  using pack_head = pack_sliced_l<0, Pack>;

  template < class Pack >
  using pack_tail = pack_sliced_r<0, Pack>;

  template < class Pack >
  using pack_half_sliced_l = pack_sliced_l<pack_middle_point_v<Pack>, Pack>;

  template < class Pack >
  using pack_half_sliced_r = pack_sliced_r<pack_middle_point_v<Pack>, Pack>;


  template < class... >
  struct pack_cat;

  template < template < class... > class Pack, class... Left, class... Right >
  struct pack_cat<Pack<Left...>, Pack<Right...>> {
    using type = Pack<Left..., Right...>;
  };

  template < class Pack1st, class Pack2nd, class... PackTail >
  struct pack_cat<Pack1st, Pack2nd, PackTail...>
    : pack_cat<typename pack_cat<Pack1st,Pack2nd>::type,PackTail...>
  {};


  template < class... Pack >
  using pack_cat_t = typename pack_cat<Pack...>::type;


  template < class, class >
  struct pack_revert;

  template < template < class... > class Seq, class... R, class Head, class... Tail >
  struct pack_revert< Seq<R...>, Seq<Head, Tail...>> 
    : pack_revert< Seq<Head, R...>, Seq<Tail...> >
  {};

  template < template < class... > class Seq, class... R, class Head >
  struct pack_revert<Seq<R...>, Seq<Head>> {
    using type = Seq<Head, R...>;
  };

  template < template < class T, T... > class Seq, class Ty, Ty... R, Ty Head, Ty... Tail >
  struct pack_revert<Seq<Ty, R...>, Seq<Ty, Head, Tail...>>
    : pack_revert<Seq<Ty, Head, R...>, Seq<Ty, Tail...>>
  {};

  template < template < class T, T... > class Seq, class Ty, Ty... R, Ty Head >
  struct pack_revert<Seq<Ty, R...>, Seq<Ty, Head>> {
    using type = Seq<Ty, Head, R...>;
  };

  template < class From, class To >
  struct replace_pred;

  template < class, class, class >
  struct pack_replace;

  template < template < class... > class Seq, class From, class To, class... R, class Head, class... Tail >
  struct pack_replace<replace_pred<From, To>, Seq<R...>, Seq<Head, Tail...>>
    : pack_replace<replace_pred<From, To>, Seq<R..., std::conditional_t<std::is_same<From, Head>::value, To, Head>>, Seq<Tail...>>
  {};

  template < template < class... > class Seq, class From, class To, class... R, class Head >
  struct pack_replace<replace_pred<From, To>, Seq<R...>, Seq<Head>> {
    using type = Seq<R..., std::conditional_t<std::is_same<From, Head>::value, To, Head>>;
  };

  template < template<class>class, class, class, class >
  struct pack_replace_if_impl;

  template < template < class... > class Seq, template<class>class Pred, class To, class... R, class Head, class... Tail >
  struct pack_replace_if_impl<Pred, To, Seq<R...>, Seq<Head, Tail...>>
    : pack_replace_if_impl<Pred, To, Seq<R..., std::conditional_t<Pred<Head>::value, To, Head>>, Seq<Tail...>>
  {};

  template < template < class... > class Seq, template<class>class Pred, class To, class... R, class Head >
  struct pack_replace_if_impl<Pred, To, Seq<R...>, Seq<Head>> {
    using type = Seq<R..., std::conditional_t<Pred<Head>::value, To, Head>>;
  };

  template < class >
  struct pack_reverse;

  template < template < class T, T... > class Sequence, class Type, Type... Values >
  struct pack_reverse< Sequence<Type, Values...> > {
    using type = typename pack_revert< Sequence<Type>, Sequence<Type, Values...> >::type;
  };

  template<template<class...> class T, typename... Args>
  struct pack_reverse< T<Args...> > {
    using type = typename pack_revert< T<>, T<Args...> >::type;
  };

  template < typename T >
  using pack_reverse_t = typename pack_reverse<T>::type;

  template < class >
  struct pack_replace_all;

  template<template<typename...> class T, typename... Args>
  struct pack_replace_all< T<Args...> > {

    template < typename... Ts >
    using type = T<Ts...>;
  };

  template < class T, class... Ts >
  using pack_replace_all_t = typename pack_replace_all<T>:: template type<Ts...>;

  template < class >
  struct replace;

  template < template<class...>class T, typename... Args >
  struct replace<T<Args...>> {
    template < class From, class To >
    using type = typename pack_replace< replace_pred<From, To>, T<>, T<Args...> >::type;
  };

  template < class T, class From, class To >
  using pack_replace_t = typename replace<T>:: template type<From, To>;


  template < class, template<class>class, class >
  struct pack_replace_if;

  template < template<class...>class T, typename... Args, template<class>class Pred, class To >
  struct pack_replace_if<T<Args...>, Pred, To> {
    using type = typename pack_replace_if_impl< Pred, To, T<>, T<Args...> >::type;
  };

  template < class T, template <class>class Pred, class To >
  using pack_replace_if_t = typename pack_replace_if<T, Pred, To>::type;

  template < class, template < class ... > class >
  struct repack;

  template< template<class...> class T, template<class...> class U, class... Args>
  struct repack< T<Args...>, U > {
    using type = U<Args...>;
  };

  template < typename From, template < class ... > class To >
  using repack_t = typename repack<From,To>::type;

namespace cranberries_magic {

  template < class, class, class >
  struct pack_remove_impl;

  template < class T, template < class... > class Pack, class... Result, class Head, class... Tail >
  struct pack_remove_impl<T, Pack<Head, Tail...>, Pack<Result...>>
    : std::conditional_t<std::is_same<T, Head>::value,
        pack_remove_impl<T, Pack<Tail...>, Pack<Result...>>,
        pack_remove_impl<T, Pack<Tail...>, Pack<Result..., Head>>
    >
  {};

  template < class T, template < class... > class Pack, class... Result >
  struct pack_remove_impl<T, Pack<>, Pack<Result...>>
  {
    using type = Pack<Result...>;
  };

  template < template<class> class, class, class >
  struct pack_remove_if_impl;

  template < template<class> class Pred, template < class... > class Pack, class... Result, class Head, class... Tail >
  struct pack_remove_if_impl<Pred, Pack<Head, Tail...>, Pack<Result...>>
    : std::conditional_t<Pred<Head>::value,
    pack_remove_if_impl<Pred, Pack<Tail...>, Pack<Result...>>,
    pack_remove_if_impl<Pred, Pack<Tail...>, Pack<Result..., Head>>
    > {};

  template < template<class> class Pred, template < class... > class Pack, class... Result >
  struct pack_remove_if_impl<Pred, Pack<>, Pack<Result...>>
  {
    using type = Pack<Result...>;
  };

}

  template < class, class >
  struct pack_remove;

  template < class T, template < class... > class Pack, class... Types >
  struct pack_remove<T, Pack<Types...>> {
    using type = typename cranberries_magic::pack_remove_impl<T, Pack<Types...>, Pack<>>::type;
  };

  template < class T, class Pack >
  using pack_remove_t = typename pack_remove<T,Pack>::type;

  template < template <class> class, class >
  struct pack_remove_if;

  template < template <class> class Pred, template < class... > class Pack, class... Types >
  struct pack_remove_if<Pred, Pack<Types...>> {
    using type = typename cranberries_magic::pack_remove_if_impl<Pred, Pack<Types...>, Pack<>>::type;
  };

  template < template <class> class Pred, class Pack >
  using pack_remove_if_t = typename pack_remove_if<Pred, Pack>::type;

  template < size_t, class, class >
  struct pack_insert;

  template < size_t I, class T, template < class... > class Pack, class... Types >
  struct pack_insert<I, T, Pack<Types...>> {
    using type = pack_cat_t<pack_sliced_l<I, Pack<Types...>>, Pack<T>, pack_sliced_r<I, Pack<Types...>>>;
  };

  template < size_t I, class T, class Pack >
  using pack_insert_t = typename pack_insert<I, T, Pack>::type;

  template < class, class >
  struct pack_push_back;

  template < class T, template < class... > class Pack, class... Types >
  struct pack_push_back<T, Pack<Types...>> {
    using type = pack_cat_t<Pack<Types...>, Pack<T>>;
  };

  template < class T, class Pack >
  using pack_push_back_t = typename pack_push_back<T, Pack>::type;

  template < class, class >
  struct pack_push_front;

  template < class T, template < class... > class Pack, class... Types >
  struct pack_push_front<T, Pack<Types...>> {
    using type = pack_cat_t<Pack<T>, Pack<Types...>>;
  };

  template < class T, class Pack >
  using pack_push_front_t = typename pack_push_front<T, Pack>::type;

  template < class >
  struct pack_pop_front;

  template < template < class... > class Pack, class Head, class... Types >
  struct pack_pop_front<Pack<Head, Types...>> {
    using type = Pack<Types...>;
  };

  template < class Pack >
  using pack_pop_front_t = typename pack_pop_front<Pack>::type;

  template < class Pack >
  struct pack_pop_back {
    using type = pack_reverse_t<pack_pop_front_t<pack_reverse_t<Pack>>>;
  };

  template < class Pack >
  using pack_pop_back_t = typename pack_pop_back<Pack>::type;

  template < size_t I, class Pack >
  struct pack_erase
  {
    using type = pack_cat_t<pack_pop_back_t<pack_sliced_l<I, Pack>>, pack_sliced_r<I, Pack>>;
  };

  template < class Pack >
  struct pack_erase<0, Pack>
  {
    using type = pack_pop_front_t<Pack>;
  };


  template < size_t I, class Pack >
  using pack_erase_t = typename pack_erase<I, Pack>::type;

  template < class Pack >
  using pack_pop_back_t = typename pack_pop_back<Pack>::type;

  template < size_t From, size_t To,  class Pack >
  struct pack_range_erase
  {
    using type = pack_cat_t<pack_sliced_l<From-1, Pack>, pack_sliced_r<To, Pack>>;
  };

  template < size_t To, class Pack >
  struct pack_range_erase<0, To, Pack>
  {
    using type = pack_sliced_r<To, Pack>;
  };

  template < size_t From, size_t To, class Pack >
  using pack_range_erase_t = typename pack_range_erase<From, To, Pack>::type;


  template < template<class...>class, class Fn >
  struct args_to_pack;

  template < template < class...> class Pack, class Fn, class... Args >
  struct args_to_pack<Pack,Fn(Args...)> {
    using type = Pack<Args...>;
  };

  template < template<class...>class Pack, class Fn >
  using args_to_pack_t = typename args_to_pack<Pack,Fn>::type;


  template < size_t, class >
  struct arg_element;

  template < size_t N, class Fn, class... ArgTypes >
  struct arg_element<N, Fn(ArgTypes...)> {
    using type = pack_element_t<N, type_pack<ArgTypes...>>;
  };

  template < size_t N, class Fn >
  using arg_element_t = typename arg_element<N, Fn>::type;

}

#endif
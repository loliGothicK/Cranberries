/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_PACK_OPERATIONS_HPP
#define CRANBERRIES_PACK_OPERATIONS_HPP
#include <type_traits>
#include "type_traits.hpp"

namespace cranberries {

	template < class >
	struct is_type_pack : std::false_type {};

	template < template < class... > class T,
						 class... Types >
	struct
		is_type_pack<T<Types...>> : std::true_type {};

	template < class T >
	constexpr bool is_type_pack_v = is_type_pack<T>::value;

	template < class >
	struct is_value_pack : std::false_type {};

	template < template < class Ty, Ty... > class T,
						 class Type,
						 Type... Values >
	struct
		is_value_pack<T<Type, Values...>> : std::true_type {};

	template < class T >
	constexpr bool is_value_pack_v = is_type_pack<T>::value;

	template < class T >
	struct is_pack
		: disjunction<is_type_pack<T>, is_value_pack<T>> {};

	template < class T >
	constexpr bool is_pack_v = is_pack<T>::value;


	template < class ...Types >
	struct type_pack;

	template < class T, T ...Args >
	struct value_pack;

	template < bool ...Bs >
	using bool_pack = value_pack<bool, Bs...>;

	template < size_t ...I >
	using size_pack = value_pack<size_t, I...>;


	template < class >
	struct value_type_of;

	template < class T, template < class , T... > class vPack, T... E >
	struct value_type_of<vPack<T,E...>>{
		using type = T;
	};

	template < class vPack >
	using value_type_of_t = typename value_type_of<vPack>::type;

	template < size_t... Indices >
	struct swallow_t {};

	template < class >
	struct make_swallow_impl;

	template < size_t... I >
	struct make_swallow_impl<std::integer_sequence<size_t,I...>> {
		using type = swallow_t<I...>;
	};

	template < size_t N >
	using make_swallow_t = typename make_swallow_impl<std::make_index_sequence<N>>::type;

	template < class ... Packs >
	struct swallow_indices {};

	template < size_t... X >
	using make_swallow_indices = swallow_indices<make_swallow_t<X>...>;



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
	template< template < class... > class Pack,
						class Head, class... Tail >
	struct
		pack_element<0, Pack<Head, Tail...>>
		: nested_type_class<Head> {};

	template < size_t N,
						 typename Pack >
	using pack_element_t = typename pack_element<N, Pack>::type;

	// recursive case
	template< std::size_t I, class T, template < class, T... > class vPack, T Head, T... Tail >
	struct pack_element<I, vPack<T, Head, Tail...>>
		: pack_element<I - 1, vPack<T, Tail...>>
	{
		static_assert(I <= sizeof...(Tail), "pack out of range.");
	};

	// base case
	template< class T, template < class, T... > class vPack, T Head, T... Tail >
	struct pack_element<0, vPack<T, Head, Tail...>>
	{
		static constexpr T value = Head;
	};

	template < size_t N,
		typename Pack >
		using pack_element_t = typename pack_element<N, Pack>::type;

	template < size_t I,
						 class T >
	struct
		pack_expand
		: nested_type_class<T> {};

	template < size_t I,
						 class... Types >
	struct
		pack_expand<I, type_pack<Types...>>
		: nested_type_class<pack_element_t<I, type_pack<Types...>>> {};

	template < size_t I,
						 class T >
	using pack_expand_t = typename pack_expand<I, T>::type;


	template < class >
	struct pack_size;

	template < template< class... > class Pack,
						 class... Types >
	struct
		pack_size<Pack<Types...>>
		: size_constant<sizeof...(Types)> {};

	template < template < class Ty, Ty... > class Pack,
						 class T,
						 T... Values >
	struct
		pack_size<Pack<T, Values...>>
		: size_constant<sizeof...(Values)> {};


	template < class >
	struct pack_middle_point;

	template < template < class... > class Pack,
						 class... Types >
	struct
		pack_middle_point<Pack<Types...>>
		: size_constant< (sizeof...(Types) % 2 == 0) ? sizeof...(Types) / 2 : sizeof...(Types) / 2 + 1 > {};

	template < template < class T, T... > class Pack,
						 class Ty,
						 Ty... Values >
	struct
		pack_middle_point<Pack<Ty, Values...>>
		: size_constant< (sizeof...(Values)%2 == 0) ? sizeof...(Values) / 2 : sizeof...(Values) / 2 + 1 > {};

	template < class Pack >
	constexpr size_t pack_size_v = pack_size<Pack>::value;

	template < class Pack >
	constexpr size_t pack_middle_point_v = pack_middle_point<Pack>::value;



	namespace cranberries_magic {

		template< size_t, class, class, bool >
		struct pack_slicer_impl {};

		template< size_t I,
							template <class...> class tPack,
							class ...Left,
							class RightHead,
							class... RightTial>
		struct
			pack_slicer_impl<I, tPack<Left...>, tPack<RightHead, RightTial...>, true >
			: pack_slicer_impl<I - 1, tPack<Left..., RightHead>, tPack<RightTial...>, (I!=1)> {};

		template< template<class...>class tPack,
							class ...Left >
		struct
			pack_slicer_impl<0,
											 tPack<Left...>,
											 tPack<>,
											 false >
		{
			using left_pack = tPack<Left...>;
			using right_pack = tPack<>;
		};

		template< template <class...> class tPack,
							class ...Left,
							class RightHead,
							class... RightTail>
		struct
			pack_slicer_impl<0,
											 tPack<Left...>,
											 tPack<RightHead, RightTail...>,
											 false >
		{
			using left_pack = tPack<Left...>;
			using right_pack = tPack<RightHead, RightTail...>;
		};

		template< size_t I,
							class T,
							template <class Ty, Ty...> class vPack,
							T ...Left,
							T RightHead,
							T... RightTial >
		struct
			pack_slicer_impl<I, vPack<T, Left...>, vPack<T, RightHead, RightTial...>, true >
			: pack_slicer_impl<I - 1, vPack<T, Left..., RightHead>, vPack<T, RightTial...>, (I!=1) > {};

		template< class T,
							template<class Ty, Ty...> class vPack,
							T ...Left >
		struct
			pack_slicer_impl<0,
											 vPack<T, Left...>,
											 vPack<T>,
											 false >
		{
			using left_pack = vPack<T, Left...>;
			using right_pack = vPack<T>;
		};

		template< class T,
							template <class Ty, Ty...> class vPack,
							T ...Left,
							T RightHead,
							T... RightTail >
		struct
			pack_slicer_impl<0,
											 vPack<T, Left...>,
											 vPack<T, RightHead, RightTail...>,
											 false >
		{
			using left_pack = vPack<T, Left...>;
			using right_pack = vPack<T, RightHead, RightTail...>;
		};

	}

	template < size_t, class >
	struct pack_slicer;

	template < size_t I,
						 template < class... > class Pack,
						 class ...Types >
	struct
		pack_slicer<I, Pack<Types...>>
	{
		using right_pack = typename cranberries_magic::pack_slicer_impl<I, Pack<>, Pack<Types...>, (I!=0)>::right_pack;
		using left_pack = typename cranberries_magic::pack_slicer_impl<I, Pack<>, Pack<Types...>, (I!=0)>::left_pack;
	};

	template < size_t I,
						 template < class Ty, Ty... > class vPack, 
						 class T,
						 T... Values >
	struct
		pack_slicer<I, vPack<T, Values...>> 
	{
		using right_pack = typename cranberries_magic::pack_slicer_impl<I, vPack<T>, vPack<T, Values...>, (I!=0)>::right_pack;
		using left_pack = typename cranberries_magic::pack_slicer_impl<I, vPack<T>, vPack<T, Values...>, (I!=0)>::left_pack;
	};

	template < size_t I, class Pack >
	using pack_sliced_l = typename pack_slicer<I, Pack>::left_pack;

	template < size_t I, class Pack >
	using pack_sliced_r = typename pack_slicer<I, Pack>::right_pack;

	template < class Pack >
	using pack_head = pack_sliced_l<1, Pack>;

	template < class Pack >
	using pack_tail = pack_sliced_r<1, Pack>;

	template < class Pack >
	using pack_half_sliced_l = pack_sliced_l<pack_middle_point_v<Pack>, Pack>;

	template < class Pack >
	using pack_half_sliced_r = pack_sliced_r<pack_middle_point_v<Pack>, Pack>;


	template < class... >
	struct pack_cat;

	template < template < class... > class Pack,
						 class... Left,
						 class... Right >
	struct
		pack_cat<Pack<Left...>, Pack<Right...>>
		: nested_type_class<Pack<Left..., Right...>> {};

	template < class T, template <class Ty, Ty...> class vPack, T... L, T... R >
	struct pack_cat<vPack<T, L...>, vPack<T, R...>> {
		using type = vPack<T, L..., R...>;
	};

	template < class Pack1st,
						 class Pack2nd,
						 class... PackTail >
	struct
		pack_cat<Pack1st, Pack2nd, PackTail...>
		: pack_cat<typename pack_cat<Pack1st, Pack2nd>::type, PackTail...> {};


	template < class... Packs >
	using pack_cat_t = typename pack_cat<Packs...>::type;


	template < class, class >
	struct pack_revert;

	// type pack revert
	template < template <class... > class tPack >
	struct
		pack_revert<tPack<>, tPack<>>
		: nested_type_class<tPack<>> {};

	template < template < class... > class Seq,
						 class... R, class Head,
						 class... Tail >
	struct
		pack_revert< Seq<R...>,
								Seq<Head, Tail...> >
		: pack_revert<Seq<Head, R...>, 
									Seq<Tail...> > {};

	template < template < class... > class Seq,
						 class... R >
	struct
		pack_revert< Seq<R...>,
								 Seq<> >
		: nested_type_class<Seq<R...>> {};

	// value pack revert
	template < class T, template <class, T... > class vPack >
	struct
		pack_revert<vPack<T>, vPack<T>>
		: nested_type_class<vPack<T>> {};
	
	template < template < class T, T... > class Seq,
						 class Ty,
						 Ty... R,
						 Ty Head,
						 Ty... Tail >
	struct
		pack_revert< Seq<Ty, R...>,
								 Seq<Ty, Head, Tail...> >
		: pack_revert<Seq<Ty, Head, R...>,
									Seq<Ty, Tail...> > {};

	template < template < class T, T... > class Seq,
						 class Ty,
						 Ty... R >
	struct
		pack_revert< Seq<Ty, R...>,
											Seq<Ty>>
		: nested_type_class<Seq<Ty,R...>> {};


	template < class, class, class, class >
	struct pack_replace;

	template < template < class... > class Seq,
						 class From,
						 class To,
						 class... R,
						 class Head,
						 class... Tail >
	struct
		pack_replace<From,
								 To,
								 Seq<R...>,
								 Seq<Head, Tail...> >
		: pack_replace<From,
									 To,
									 Seq<R..., std::conditional_t<std::is_same<From, Head>::value, To, Head>>,
									 Seq<Tail...> > {};

	template < template < class... > class Seq,
						 class From,
						 class To,
						 class... R,
						 class Head >
	struct
		pack_replace<From, To, Seq<R...>,
								 Seq<Head> > 
		: nested_type_class<Seq<R..., std::conditional_t<std::is_same<From, Head>::value, To, Head>>> {};


	template < template<class>class, class, class, class >
	struct pack_replace_if_impl;

	template < template < class... > class Seq,
						 template<class>class Pred,
						 class To,
						 class... R,
						 class Head,
						 class... Tail >
	struct
		pack_replace_if_impl<Pred,
												 To,
												 Seq<R...>,
												 Seq<Head, Tail...> >
		: pack_replace_if_impl<Pred,
													 To,
													 Seq<R..., std::conditional_t<Pred<Head>::value, To, Head>>, Seq<Tail...> > {};

	template < template < class... > class Seq,
						 template<class>class Pred,
						 class To,
						 class... R,
						 class Head >
	struct
		pack_replace_if_impl<Pred,
												 To,
												 Seq<R...>,
												 Seq<Head> >
		: nested_type_class<Seq<R..., std::conditional_t<Pred<Head>::value, To, Head>>> {};



	template < class >
	struct pack_reverse;

	template < template < class T, T... > class Sequence,
						 class Type,
						 Type... Values >
	struct
		pack_reverse< Sequence<Type, Values...> >
		: nested_type_class< typename pack_revert< Sequence<Type>, Sequence<Type, Values...> >::type > {};

	template< template<class...> class tPack,
						class... Types >
	struct
		pack_reverse< tPack<Types...> > 
		: nested_type_class< typename pack_revert< tPack<>, tPack<Types...> >::type >{};

	template < class T >
	using pack_reverse_t = typename pack_reverse<T>::type;

	template < class >
	struct pack_replace_all;

	template< template< class... > class tPack, class... Types >
	struct
		pack_replace_all< tPack<Types...> >
	{
		template < class... NewTypes >
		using type = tPack<NewTypes...>;
	};

	template < class T, class... Ts >
	using pack_replace_all_t = typename pack_replace_all<T>:: template type<Ts...>;

	template < class >
	struct replace;

	template < template < class... > class tPack,
						 typename... Types >
	struct
		replace<tPack<Types...>>
	{
		template < class From, class To >
		using type = typename pack_replace< From, To, tPack<>, tPack<Types...> >::type;
	};

	template < class T, class From, class To >
	using pack_replace_t = typename replace<T>:: template type<From, To>;


	template < class, template< class > class, class >
	struct pack_replace_if;

	template < template<class...>class tPack,
						 class... Types,
						 template< class > class Pred,
						 class To >
	struct
		pack_replace_if<tPack<Types...>, Pred, To>
		: nested_type_class< typename pack_replace_if_impl< Pred, To, tPack<>, tPack<Types...> >::type > {};

	template < class T, template <class>class Pred, class To >
	using pack_replace_if_t = typename pack_replace_if<T, Pred, To>::type;



	template < class, template < class ... > class >
	struct repack;

	template< template<class...> class tPack,
						template<class...> class RePacker,
						class... Types >
	struct
		repack< tPack<Types...>, RePacker >
		: nested_type_class< RePacker<Types...> > {};

	template < typename From, template < class ... > class To >
	using repack_t = typename repack<From, To>::type;



namespace cranberries_magic {

	template < class, class, class >
	struct pack_remove_impl;

	template < class T,
						 template < class... > class tPack,
						 class... Result,
						 class Head,
						 class... Tail >
	struct
		pack_remove_impl<T, tPack<Head, Tail...>, tPack<Result...>>
		: std::conditional_t<std::is_same<T, Head>::value,
				pack_remove_impl<T, tPack<Tail...>, tPack<Result...>>,
				pack_remove_impl<T, tPack<Tail...>, tPack<Result..., Head>>
		> {};


	template < class T,
						 template < class... > class tPack,
						 class... Result >
	struct
		pack_remove_impl<T, tPack<>, tPack<Result...>>
		: nested_type_class< tPack<Result...> > {};



	template < template<class> class, class, class >
	struct pack_remove_if_impl;

	template < template<class> class Pred,
						 template < class... > class tPack,
						 class... Result,
						 class Head,
						 class... Tail >
	struct
		pack_remove_if_impl<Pred, tPack<Head, Tail...>, tPack<Result...>>
		: std::conditional_t<Pred<Head>::value,
				pack_remove_if_impl<Pred, tPack<Tail...>, tPack<Result...>>,
				pack_remove_if_impl<Pred, tPack<Tail...>, tPack<Result..., Head>>
		> {};

	template < template<class> class Pred,
						 template < class... > class Pack,
						 class... Result >
	struct
		pack_remove_if_impl<Pred, Pack<>, Pack<Result...>>
		: nested_type_class<Pack<Result...>> {};

}

	template < class, class >
	struct pack_remove;

	template < class T,
						 template < class... > class Pack,
						 class... Types >
	struct
		pack_remove<T, Pack<Types...>>
		: nested_type_class< typename cranberries_magic::pack_remove_impl<T, Pack<Types...>, Pack<>>::type >{};

	template < class T, class Pack >
	using pack_remove_t = typename pack_remove<T, Pack>::type;

	template < template <class> class, class >
	struct pack_remove_if;

	template < template <class> class Pred,
						 template < class... > class Pack,
						 class... Types >
	struct
		pack_remove_if<Pred, Pack<Types...>>
		: nested_type_class< typename cranberries_magic::pack_remove_if_impl<Pred, Pack<Types...>, Pack<>>::type >{};


	template < template <class> class Pred, class Pack >
	using pack_remove_if_t = typename pack_remove_if<Pred, Pack>::type;



	template < size_t, class, class >
	struct pack_insert;

	template < size_t I,
						 class T,
						 template < class... > class Pack,
						 class... Types >
	struct
		pack_insert<I, T, Pack<Types...>>
		: nested_type_class< pack_cat_t<pack_sliced_l<I, Pack<Types...>>, Pack<T>, pack_sliced_r<I, Pack<Types...>>> > {};

	template < size_t I, class T, class Pack >
	using pack_insert_t = typename pack_insert<I, T, Pack>::type;



	template < class, class >
	struct pack_push_back;

	template < class T,
						 template < class... > class Pack,
						 class... Types >
	struct
		pack_push_back<T, Pack<Types...>> 
		: nested_type_class< pack_cat_t<Pack<Types...>, Pack<T>> >{};

	template < class T, class Pack >
	using pack_push_back_t = typename pack_push_back<T, Pack>::type;



	template < class, class >
	struct pack_push_front;

	template < class T,
						 template < class... > class Pack,
						 class... Types >
	struct
		pack_push_front<T, Pack<Types...>>
		: nested_type_class< pack_cat_t<Pack<T>, Pack<Types...>> >{};

	template < class T, class Pack >
	using pack_push_front_t = typename pack_push_front<T, Pack>::type;



	template < class >
	struct pack_pop_front;

	template < template < class... > class Pack,
						 class Head,
						 class... Types >
	struct
		pack_pop_front<Pack<Head, Types...>>
		: nested_type_class< Pack<Types...> >{};

	template < class Pack >
	using pack_pop_front_t = typename pack_pop_front<Pack>::type;

	template < class Pack >
	struct
		pack_pop_back
		: nested_type_class< pack_reverse_t<pack_pop_front_t<pack_reverse_t<Pack>>> > {};

	template < class Pack >
	using pack_pop_back_t = typename pack_pop_back<Pack>::type;


	template < size_t I, class Pack >
	struct pack_erase
		 : nested_type_class< pack_cat_t< pack_sliced_l<I,Pack>, pack_sliced_r<I+1,Pack> > >{};


	template < size_t I, class Pack >
	using pack_erase_t = typename pack_erase<I, Pack>::type;

	template < class Pack >
	using pack_pop_back_t = typename pack_pop_back<Pack>::type;

	template < size_t From,
						 size_t To,
						 class Pack >
	struct
		pack_range_erase
		: nested_type_class< pack_cat_t<pack_sliced_l<From - 1, Pack>, pack_sliced_r<To, Pack>> >{};

	template < size_t To, class Pack >
	struct
		pack_range_erase<0, To, Pack>
		: nested_type_class< pack_sliced_r<To, Pack> >{};

	template < size_t From,
						 size_t To,
						 class Pack >
	using pack_range_erase_t = typename pack_range_erase<From, To, Pack>::type;

	template < class >
	struct pack_clear;

	template < template <class...> class tPack,
						 class... Types >
	struct
		pack_clear<tPack<Types...>>
		: nested_type_class<tPack<>> {};

	template < class tPack >
	using pack_clear_t = typename pack_clear<tPack>::type;


	template < template<class...>class, class Fn >
	struct args_to_pack;

	template < template < class...> class Pack,
						 class Fn,
						 class... Args >
	struct
		args_to_pack<Pack, Fn(Args...)>
		: nested_type_class< Pack<Args...> > {};

	template < template< class... > class Pack, class Fn >
	using args_to_pack_t = typename args_to_pack<Pack, Fn>::type;


	template < size_t, class >
	struct arg_element;

	template < size_t N,
						 class Fn,
						 class... ArgTypes >
	struct
		arg_element<N, Fn(ArgTypes...)>
		: nested_type_class< pack_element_t<N, type_pack<ArgTypes...>> > {};

	template < size_t N, class Fn >
	using arg_element_t = typename arg_element<N, Fn>::type;


	template < template < class > class , class > 
	struct pack_type_transform;

	template < template < class... > class Pack,
						 template < class > class Operand,
						 class... Types >
	struct
		pack_type_transform<Operand, Pack<Types...>>
		: nested_type_class< Pack<get_type<Operand<Types>>...> > {};

	template < template < class > class Operand, class Pack >
	using pack_t2t_transform_t = typename pack_type_transform<Operand, Pack>::type;


	template < template < class > class, class >
	struct pack_t2v_transform;

	template < template < class... > class Pack,
						 template < class > class Operand,
						 class Head,
						 class... Types >
	struct
		pack_t2v_transform<Operand, Pack<Head,Types...>>
		: nested_type_class< value_pack<std::decay_t<decltype(Operand<Head>::value)>,
																		Operand<Head>::value, Operand<Types>::value...> > {};

	template < class T, template < T > class, class >
	struct pack_v2v_transform;

	template < class T,
						 template < class, T... > class vPack,
						 template < T > class Operand,
						 T Head,
						 T... Types >
	struct
		pack_v2v_transform<T, Operand, vPack<T, Head, Types...>>
		: nested_type_class< value_pack<decltype(Operand<Head>::value),
																		Operand<Head>::value, Operand<Types>::value...> > {};

	template < class T, template < T > class Operand, class vPack >
	using pack_v2v_transform_t = typename pack_v2v_transform<T, Operand, vPack>::type;



	template < class T, template < T > class, class >
	struct pack_v2t_transform;

	template < class T,
						 template < class, T... > class vPack,
						 template < T > class Operand,
						 T... E >
	struct
		pack_v2t_transform<T, Operand, vPack<T, E...>>
		: nested_type_class< type_pack<get_type<Operand<E>>...> > {};


	template < class T, template < T > class Operand, class vPack >
	using pack_v2t_transform_t = typename pack_v2t_transform<T, Operand, vPack>::type;


	template < template < class > class Operand, class Pack >
	using pack_t2v_transform_t = typename pack_t2v_transform<Operand, Pack>::type;

	template < template < class > class, class >
	struct pack_all_match_if;

	template < template < class > class Pred, template < class... > class tPack, class... Args >
	struct pack_all_match_if<Pred,tPack<Args...>>
		: conjunction<Pred<Args>...> {};

	template < template < class > class, class >
	struct pack_any_match_if;

	template < template < class > class Pred, template < class... > class tPack, class... Args >
	struct pack_any_match_if<Pred, tPack<Args...>>
		: disjunction<Pred<Args>...> {};

	template < template < class > class, class >
	struct pack_none_match_if;

	template < template < class > class Pred, template < class... > class tPack, class... Args >
	struct pack_none_match_if<Pred, tPack<Args...>>
		: negation<disjunction<Pred<Args>...>> {};

	template < template < class > class Pred, class tPack >
	constexpr bool pack_all_match_if_v = pack_all_match_if<Pred, tPack>::value;

	template < template < class > class Pred, class tPack >
	constexpr bool pack_any_match_if_v = pack_any_match_if<Pred, tPack>::value;

	template < template < class > class Pred, class tPack >
	constexpr bool pack_none_match_if_v = pack_none_match_if<Pred, tPack>::value;

	template < class, class >
	struct pack_all_match;

	template < class T, template < class... > class tPack, class... Args >
	struct pack_all_match<T, tPack<Args...>>
		: conjunction<std::is_same<T,Args>...> {};

	template < class, class >
	struct pack_any_match;

	template < class T, template < class... > class tPack, class... Args >
	struct pack_any_match<T, tPack<Args...>>
		: disjunction<std::is_same<T, Args>...> {};

	template < class, class >
	struct pack_none_match;

	template < class T, template < class... > class tPack, class... Args >
	struct pack_none_match<T, tPack<Args...>>
		: negation<disjunction<std::is_same<T,Args>...>> {};

	template < class T, class tPack >
	constexpr bool pack_all_match_v = pack_all_match<T, tPack>::value;

	template < class T, class tPack >
	constexpr bool pack_any_match_v = pack_any_match<T, tPack>::value;

	template < class T, class tPack >
	constexpr bool pack_none_match_v = pack_none_match<T, tPack>::value;


	template < class >
	struct
		pack_conjunction : std::true_type {};

	template < class T, template < class , T... > class vPack, T Head, T... Tail >
	struct 
		pack_conjunction<vPack<T, Head, Tail...>>
		: std::conditional_t<Head,
												 pack_conjunction<vPack<T, Tail...>>,
												 std::false_type> {};

	template < class T, template < class, T... > class vPack, T Last >
	struct
		pack_conjunction<vPack<T, Last>>
		: bool_constant<Last> {};

	template < class vPack >
	constexpr bool pack_conjunction_v = pack_conjunction<vPack>::value;

	template < class >
	struct
		pack_disjunction : std::false_type {};

	template < class T, template < class , T... > class vPack, T Head, T... Tail >
	struct 
		pack_disjunction<vPack<T, Head, Tail...>>
		: std::conditional_t<Head,
												 std::true_type,
												 pack_conjunction<vPack<T, Tail...>>> {};

	template < class T, template < class, T... > class vPack, T Last >
	struct
		pack_disjunction<vPack<T, Last>>
		: bool_constant<Last> {};

	template < class vPack >
	constexpr bool pack_disjunction_v = pack_disjunction<vPack>::value;


namespace cranberries_magic {
	template< size_t, class, class, template < class... > class >
	struct pack_chunk_impl;

	template< size_t N,
						template <class...>class tPack,
						class ...Chucnked,
						class ...RemainderTypes,
						template <class...>class NestPack >
	struct
		pack_chunk_impl<N,
										tPack<Chucnked...>,
										tPack<RemainderTypes...>,
										NestPack >
		: pack_chunk_impl<N,
											pack_cat_t<tPack<Chucnked...>,
											tPack<pack_sliced_l<N, NestPack<RemainderTypes...>>>>,
											pack_sliced_r<N, tPack<RemainderTypes...>>,
											NestPack > {};

	template< size_t N,
						template <class...> class tPack,
						class ...Chucnked,
						template <class...> class NestPack >
	struct
		pack_chunk_impl<N,
										tPack<Chucnked...>,
										tPack<>,
										NestPack >
		: nested_type_class<tPack<Chucnked...>> {};
}

	template < size_t N,
						 class Pack,
						 template < class... > class NestPack = pack_replace_all<Pack>::template type >
	using
		pack_chunk = cranberries_magic::pack_chunk_impl< N, pack_clear_t<Pack>, pack_sliced_l < pack_size_v<Pack> - (pack_size_v<Pack>%N), Pack >, NestPack> ;

	template< size_t N,
						class Pack,
						template < class... > class NestPack = pack_replace_all<Pack>::template type >
	using
		pack_chunk_t = typename pack_chunk<N, Pack, NestPack>::type;

namespace cranberries_magic {
	template < size_t, class, class, class, bool, template < class... > class >
	struct pack_adjacent_chunk_impl;

	template < size_t N,
						 template < class... > class tPack,
						 class Head,
						 class... RemainderTypes,
						 class... ChunkedTypes,
						 size_t... I,
						 template < class... > class NestPack >
	struct
		pack_adjacent_chunk_impl<N,
														 tPack<ChunkedTypes...>,
														 tPack<Head, RemainderTypes...>,
														 swallow_t<I...>,
														 true,
														 NestPack>
		: pack_adjacent_chunk_impl<N,
															 tPack<ChunkedTypes..., pack_sliced_l<sizeof...(I), NestPack<Head, RemainderTypes...>>>,
															 tPack<RemainderTypes...>,
															 swallow_t<I...>,
															 (N <= sizeof...(RemainderTypes)),
															 NestPack>{};

	template < size_t N,
						 template < class... > class tPack,
						 class... RemainderTypes,
						 class... ChunkedTypes,
						 size_t... I,
						 template < class... > class NestPack >
	struct
		pack_adjacent_chunk_impl<N, 
														 tPack<ChunkedTypes...>,
														 tPack<RemainderTypes...>,
														 swallow_t<I...>,
														 false,
														 NestPack>
		: nested_type_class<tPack<ChunkedTypes...>> {};
}
	template < size_t N,
						 class tPack,
						 template < class... > class NestPack = pack_replace_all<tPack>::template type >
	using
		pack_adjacent_chunk = cranberries_magic::pack_adjacent_chunk_impl<N,pack_clear_t<tPack>, tPack, make_swallow_t<N>, (N <= pack_size_v<tPack>), NestPack>;

	template < size_t N,
						 class tPack,
						 template < class... > class NestPack = pack_replace_all<tPack>::template type >
	using
		pack_adjacent_chunk_t = typename pack_adjacent_chunk<N, tPack, NestPack>::type;

namespace _detail {
	template <class IntegerType, IntegerType First, IntegerType Last>
	struct make_integer_range_impl {
		using type = pack_cat_t<
			typename make_integer_range_impl<IntegerType, First, First + (Last - First) / 2>::type,
			typename make_integer_range_impl<IntegerType, First + (Last - First) / 2 + 1, Last>::type>;
	};

	template <class IntegerType, IntegerType Val>
	struct make_integer_range_impl<IntegerType, Val, Val> {
		using type = std::integer_sequence<IntegerType, Val>;
	};
}

	template <class IntegerType, size_t First, size_t Last>
	using make_integer_range = typename _detail::make_integer_range_impl<IntegerType, First, Last>::type;

	template <size_t First, size_t Last>
	using make_index_range = typename _detail::make_integer_range_impl<size_t, First, Last>::type;

	template < class IntegerType, IntegerType Bound >
	using make_integer_sequence = make_integer_range<IntegerType, 0, Bound - 1>;

	template < size_t Bound >
	using make_index_sequence = make_index_range<0, Bound - 1>;

	template < class IntegerType, class... Ts >
	using integer_sequence_for = make_integer_sequence<IntegerType, sizeof...(Ts)>;

	template < class... Ts >
	using index_sequence_for = make_index_sequence<sizeof...(Ts)>;



	//=============== value_pack operations ================//
	template < class > struct pack_max;

	template < class T, template <class Ty, Ty... > class vPack, T Head, T... Tail >
	struct pack_max<vPack<T, Head, Tail...>>
		: std::integral_constant<T, (Head < pack_max<value_pack<T, Tail...>>::value
			? pack_max<value_pack<T, Tail...>>::value : Head)>
	{};

	template < class T, template <class Ty, Ty... > class vPack, T Last >
	struct pack_max<vPack<T, Last>>
		: std::integral_constant<T, Last>
	{};

	template < class T, template <class Ty, Ty... > class vPack >
	struct pack_max<vPack<T>> {
		static_assert(false_v<T>, "empty pack operation assert!");
	};

	template < class > struct pack_min;

	template < class T, template <class Ty, Ty... > class vPack, T Head, T... Tail >
	struct pack_min<vPack<T, Head, Tail...>>
		: std::integral_constant<T, (Head > pack_min<value_pack<T, Tail...>>::value
			? pack_min<value_pack<T, Tail...>>::value : Head)>
	{};

	template < class T, template <class Ty, Ty... > class vPack, T Last >
	struct pack_min<vPack<T, Last>>
		: std::integral_constant<T, Last>
	{};

	template < class T, template <class Ty, Ty... > class vPack >
	struct pack_min<vPack<T>> {
		static_assert(false_v<T>, "empty pack operation assert!");
	};

	template < class vPack >
	constexpr auto pack_max_v = pack_max<vPack>::value;

	template < class vPack >
	constexpr auto pack_min_v = pack_min<vPack>::value;

	//template < class T,
	//				   class,
	//					 class,
	//	         T,
	//	         T... >
	//struct value_pack_split_impl;

	//template < class T,
	//				   template <class, T...> class vPack, T Delim, T Head, T... Tail,
	//					 class ResultPack,
	//					 T... RemainderValues>
	//struct value_pack_split_impl<
	//					 T,
	//					 vPack<T, Head, Tail...>,
	//					 ResultPack,
	//		       Delim,
	//					 RemainderValues...>
	//	: nested_type_class<
	//		std::conditional_t<Head==Delim,
	//			value_pack_split_impl<T, vPack<T, Tail...>, pack_push_back_t<vPack<T, RemainderValues...>, ResultPack>, Delim>,
	//			value_pack_split_impl<T, vPack<T, Tail...>, ResultPack, Delim, RemainderValues..., Head>
	//	>>
	//{};

	//template < class T,
	//				   template <class, T...> class vPack, T Delim, T Head,
	//					 class ResultPack,
	//					 T... RemainderValues>
	//struct value_pack_split_impl<
	//					 T,
	//					 vPack<T, Head>,
	//					 ResultPack,
	//		       Delim,
	//					 RemainderValues...>
	//	: nested_type_class<
	//		std::conditional_t<Head==Delim,
	//			pack_push_back_t<vPack<T, RemainderValues...>, ResultPack>,
	//			pack_push_back_t<vPack<T, RemainderValues..., Head>, ResultPack>
	//	>>
	//{};

	
}

#endif
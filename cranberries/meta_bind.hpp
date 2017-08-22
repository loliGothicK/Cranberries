/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_MATA_BIND_HPP
#define CRANBERRIES_MATA_BIND_HPP
#include <type_traits>
#include "pack_operations.hpp"
#include "type_traits.hpp"


namespace workaround {
  namespace for_ {
    namespace meta {
    namespace bind  {

        namespace detail {

          template < size_t, class, class >
          struct w_pack_slicer_impl;


          template < template < class... > class Pack, class Head, class... Left, class... Right >
          struct w_pack_slicer_impl<0, Pack<Left...>, Pack<Head, Right...>>
          {
            using left_pack = Pack<Left..., Head>;
            using right_pack = Pack<Right...>;
          };

          template < size_t I, template < class... > class Pack >
          struct w_pack_slicer_impl<I, Pack<>, Pack<>>
          {
            using left_pack = Pack<>;
            using right_pack = Pack<>;
          };

          template < size_t I, template < class... > class Pack, class Head, class... Left, class... Right >
          struct w_pack_slicer_impl<I, Pack<Left...>, Pack<Head, Right...>>
            : w_pack_slicer_impl<I - 1, Pack<Left..., Head>, Pack<Right...>>
          {};



          template < template < class Ty, Ty... > class Pack, class T, T Head, T... Left, T... Right >
          struct w_pack_slicer_impl<0, Pack<T, Left...>, Pack<T, Head, Right...>>
          {
            using left_pack = Pack<T, Left..., Head>;
            using right_pack = Pack<T, Right...>;
          };

          template < template < class Ty, Ty... > class Pack, class T >
          struct w_pack_slicer_impl<0, Pack<T>, Pack<T>>
          {
            using right_pack = Pack<T>;
            using left_pack = Pack<T>;
          };

          template < size_t I, template < class Ty, Ty... > class Pack, class T, T Head, T... Left, T... Right >
          struct w_pack_slicer_impl<I, Pack<T, Left...>, Pack<T, Head, Right...>>
            : w_pack_slicer_impl<I - 1, Pack<T, Left..., Head>, Pack<T, Right...>>
          {};

        }

        template < size_t, class >
        struct w_pack_slicer {};

        template < size_t I, template < class... > class Pack, class ...Types >
        struct w_pack_slicer<I, Pack<Types...>> {
          using right = typename detail::w_pack_slicer_impl<I, Pack<>, Pack<Types...>>::right_pack;
          using left = typename detail::w_pack_slicer_impl<I, Pack<>, Pack<Types...>>::left_pack;
        };

        template < size_t I, template < class Ty, Ty... > class Pack, class T, T... Values >
        struct w_pack_slicer<I, Pack<T, Values...>> {
          using right = typename detail::w_pack_slicer_impl<I, Pack<T>, Pack<T, Values...>>::right_pack;
          using left = typename detail::w_pack_slicer_impl<I, Pack<T>, Pack<T, Values...>>::left_pack;
        };

        template < size_t I, class Pack >
        using w_pack_sliced_l = typename w_pack_slicer<I, Pack>::left;

        template < size_t I, class Pack >
        using w_pack_sliced_r = typename w_pack_slicer<I, Pack>::right;

        template < class Pack >
        using w_pack_head = w_pack_sliced_l<0, Pack>;

        template < class Pack >
        using w_pack_tail = w_pack_sliced_r<0, Pack>;

      }
    }
  }
}

namespace cranberries {
namespace cranberries_magic {

  class meta_bind_placeholders {};

  template < class >
  struct pack_if {};

  template < class Head, class ...Tail >
  struct pack_if<type_pack<Head, Tail...>> : std::is_base_of<meta_bind_placeholders, Head>{};

  template < class X, class A, class Result = type_pack<> >
  struct expansion {
    using type = std::conditional_t< pack_if<X>::value,
      typename expansion< workaround::for_::meta::bind::w_pack_tail<X>, workaround::for_::meta::bind::w_pack_tail<A>, pack_cat_t< Result, workaround::for_::meta::bind::w_pack_head<A> > >::type,
      typename expansion< workaround::for_::meta::bind::w_pack_tail<X>, A, pack_cat_t< Result, workaround::for_::meta::bind::w_pack_head<X> > >::type>;
  };


  template < class ...A, class Result >
  struct expansion<type_pack<>, type_pack<A...>, Result> {
    using type = Result;
  };

}

template < template<class...> class, class... >
struct apply_ ;


template < template<class...> class Pred, class... Types >
struct apply_ : Pred<Types...> {};

template < template<class...> class Pred, class... Types >
struct apply_<Pred, type_pack<Types...>> : Pred<Types...> {};


template < template<class...> class Pred, typename ...Types >
using apply_result_t = typename apply_<Pred, Types...>::type;

template < template<class...> class Pred, typename ...Types >
constexpr bool apply_result_v = apply_<Pred, Types...>::value;


  class x_ : private cranberries_magic::meta_bind_placeholders {};

  template < template<class...> class Expr, typename ...Types >
  struct bind_ {
    template < typename ...Apply >
    using expr = apply_<Expr,typename cranberries_magic::expansion<
      type_pack<Types...>, type_pack<Apply...>
    >::type>;
  };

  template < template<class...> class Expr, typename ...Types >
  struct bind_single {
    template < typename Apply >
    using expr = apply_<Expr,typename cranberries_magic::expansion<
      type_pack<Types...>, type_pack<Apply>
    >::type>;
  };

  template < template<class...> class Pred, class ...Types>
  using bind_1st = bind_single<Pred, x_, Types...>;

  template < template<class...> class Pred, class T, class ...Types >
  using bind_2nd = bind_single<Pred, T, x_, Types...>;


  template < template < class > class G, template < class > class... Preds >
  struct composition_ {
    template < class T >
    using pred = G<apply_result_t<composition_<Preds...>::template pred,T>>;
  };


  template < template < class > class G, template < class > class F >
  struct composition_<G,F> {
    template < class T >
    using pred = G<typename F<T>::type>;
  };


  template < class F, class... Args >
  struct bind_friendly_result_of : nested_type_class<decltype(std::declval<F>()(std::declval<Args>()...))> {};

  template < class F, class... Args >
  struct bind_friendly_result_of<F, type_pack<Args...>> : nested_type_class<decltype(std::declval<F>()(std::declval<Args>()...))> {};


  template < class F, class... Args >
  struct bind_friendly_no_throw_callable : bool_constant<is_nothrow_callable<F(Args...), return_any>::value> {};

  template < class F, class... Args >
  struct bind_friendly_no_throw_callable<F,type_pack<Args...>> : bool_constant<is_nothrow_callable<F(Args...), return_any>::value> {};

}
#endif // !CRANBERRIES_MATA_BIND_HPP

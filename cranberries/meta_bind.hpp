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


namespace cranberries {

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

namespace cranberries_magic {
  class meta_bind_placeholders {};
}

  template < size_t N >
  class x_ : private cranberries_magic::meta_bind_placeholders, public size_constant<N> {
    static_assert(0 != N, "Invalid meta placeholder");
  };

namespace cranberries_magic {
  template < class T, class Pack >
  struct bind_expand_t
    : nested_type_class<T> {};

  template < size_t N, class Pack >
  struct bind_expand_t<x_<N>, Pack>
    : nested_type_class<pack_element_t<N-1, Pack>> {};
}

  template < template<class...> class Expr, typename ...Bind >
  struct bind_ {
    template < typename ...Apply >
    using expr = apply_<Expr, typename cranberries_magic::bind_expand_t<Bind, type_pack<Apply...>>::type... >;
  };

  template < template<class...> class Pred, class ...Types>
  using bind_1st = bind_<Pred, x_<1>, Types...>;

  template < template<class...> class Pred, class T, class ...Types >
  using bind_2nd = bind_<Pred, T, x_<1>, Types...>;


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

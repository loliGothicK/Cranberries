#ifndef CRANBERRIES_MATA_BIND_HPP
#define CRANBERRIES_MATA_BIND_HPP
#include <type_traits>
#include "pack_traits.hpp"

namespace cranberries {
namespace cranberries_magic {
  class meta_bind_placeholders {};

  template < class >
  struct pack_if {};

  template < class Head, class ...Tail >
  struct pack_if<type_pack<Head, Tail...>> {
    static constexpr bool value = std::is_base_of<meta_bind_placeholders, Head>::value;
  };

  template < class X, class A, class Result = type_pack<> >
  struct expansion {
    using type = std::conditional_t< pack_if<X>::value,
      typename expansion< pack_tail<X>, pack_tail<A>, pack_cat_t< Result, pack_head<A> > >::type,
      typename expansion< pack_tail<X>, A, pack_cat_t< Result, pack_head<X> > >::type>;
  };


  template < class ...A, class Result >
  struct expansion<type_pack<>, type_pack<A...>, Result> {
    using type = Result;
  };

}
  class x_ : private cranberries_magic::meta_bind_placeholders {};

  template < template<class...> class Expr, typename ...Types >
  struct bind_ {
    template < typename ...Apply >
    using expr = typename cranberries_magic::expansion<
      type_pack<Types...>, type_pack<Apply...>
    >::type:: template expr<Expr>;
  };

  template < template<class...> class Expr, typename ...Types >
  struct bind_single {
    template < typename Apply >
    using expr = typename cranberries_magic::expansion<
      type_pack<Types...>, type_pack<Apply>
    >::type:: template expr<Expr>;
  };

  template < template<class...> class Pred, class ...Types>
  using bind_1st = bind_single<Pred, x_, Types...>;

  template < template<class...> class Pred, class T, class ...Types >
  using bind_2nd = bind_single<Pred, T, x_, Types...>;


}
#endif // !CRANBERRIES_MATA_BIND_HPP

#ifndef CRANBERRIES_DETECTION_TOOLKIT_HPP
#define CRANBERRIES_DETECTION_TOOLKIT_HPP
#include <type_traits>
#include <utility>
#include "type_traits.hpp"
#include "utility/utility.hpp"

namespace cranberries {
namespace cranberries_magic {
#define CRANBERRIES_HAS_TYPE(TYPE, XXX) \
  bool(false \
  ? cranberries::make_overload( \
    [](auto x)->decltype(std::declval<typename decltype(x)::XXX>(), std::true_type{}) {return{}; }, \
    [](...)->std::false_type {return{}; } \
  )(std::declval<TYPE>()) \
    : cranberries::protean_bool_v)

#define CRANBERRIES_HAS_FIELD(TYPE, XXX) \
  bool(false \
  ? cranberries::make_overload( \
    [](auto x)->decltype(decltype(x)::XXX, std::true_type{}) {return{}; }, \
    [](...)->std::false_type {return{}; } \
  )(std::declval<TYPE>()) \
    : cranberries::protean_bool_v)

#define CRANBERRIES_HAS_NONTYPE(TYPE, XXX) \
  bool(false \
  ? cranberries::make_overload( \
    [](auto x)->decltype(&decltype(x)::XXX, std::true_type{}) {return{}; }, \
    [](...)->std::false_type {return{}; } \
  )(std::declval<TYPE>()) \
    : cranberries::protean_bool_v)



  template < class Default
           , class 
           , template < class... > class Require
           , class... Args >
  struct
    detector : nested_type_class<Default>, std::false_type {};

  template < class Default
           , template < class... > class Require
           , class... Args >
  struct
    detector< Default, void_t<Require<Args...>>, Require, Args... >
    : nested_type_class<Default>, std::true_type {};

}

  template < template < class... > class Require, class... Args >
  using is_satisfied = cranberries_magic::detector<void, void, Require, Args...>;

  template < template < class... > class Require, class... Args >
  using is_satisfied_t = typename is_satisfied<Require, Args...>::type;

  template < template < class... > class Require, class... Args >
  constexpr bool is_satisfied_v = is_satisfied<Require, Args...>::value;

  template < template <class> class... Requires >
  struct requires;

  template < class, class, class = std::nullptr_t >
  struct concept_require;

  template < class T, template < class > class ...Requirements >
  struct concept_require<T, requires<Requirements...>>
    : nested_type_class<std::enable_if_t<conjunctional<Requirements...>::template apply<T>::value, std::nullptr_t>> {};

  template < class T, template < class > class ...Requirements, class IfType >
  struct concept_require<T, requires<Requirements...>, IfType>
    : nested_type_class<std::enable_if_t<conjunctional<Requirements...>::template apply<T>::value, IfType>> {};

  template < class T, class Req, class IfType = std::nullptr_t >
  using concept_require_t = typename concept_require<T, Req, IfType>::type;

  template < class Iter, class IfType = std::nullptr_t >
  using iter_require
    = std::enable_if_t<conjunction_v<std::is_same<Iter, decltype(std::declval<Iter&>()++)>,
                                     std::is_same<Iter&,decltype(++std::declval<Iter&>())>,
                                     std::is_copy_constructible<Iter>,
                                     std::is_copy_assignable<Iter>,
                                     std::is_destructible<Iter>,
                                     is_dereferencable<Iter>>,IfType >;

  template < class Iter, class IfType = std::nullptr_t >
  using input_iter_require
    = std::enable_if_t<conjunction_v<
        iter_require<Iter,std::true_type>,
        std::is_same<bool, decltype(std::declval<const Iter&>() == std::declval<const Iter&>())>,
        std::is_same<bool, decltype(std::declval<const Iter&>() != std::declval<const Iter&>())>>,
      IfType >;


  template < class Iter, class IfType = std::nullptr_t >
  using output_iter_require
    = std::enable_if_t<conjunction_v<
        iter_require<Iter,std::true_type>,
        std::is_assignable<decltype(*std::declval<Iter>()), decltype(*std::declval<Iter>())> >,
      IfType >;

  template < class Iter, class IfType = std::nullptr_t >
  using forward_iter_require
    = std::enable_if_t<conjunction_v<
        input_iter_require<Iter, std::true_type>,
        output_iter_require<Iter, std::true_type>,
        std::is_default_constructible<Iter>>,
      IfType >;

  template < class Iter, class IfType = std::nullptr_t >
  using bidrectional_iter_require
    = std::enable_if_t<conjunction_v<
        std::is_same<Iter, decltype(std::declval<Iter&>()--)>,
        std::is_same<Iter&, decltype(--std::declval<Iter&>())>,
        forward_iter_require<Iter, std::true_type>>,
      IfType >;

  template < class Iter, class IfType = std::nullptr_t >
  using random_accsess_iter_require
    = std::enable_if_t<conjunction_v<
        std::is_same<Iter, decltype(std::declval<const Iter&>()+2)>,
        std::is_same<Iter, decltype(std::declval<const Iter&>()-2)>,
        std::is_same<Iter&, decltype(std::declval<Iter&>()+=2)>,
        std::is_same<Iter&, decltype(std::declval<Iter&>()-=2)>,
        std::is_same<bool, decltype(std::declval<const Iter&>()<std::declval<const Iter&>())>,
        std::is_same<bool, decltype(std::declval<const Iter&>()<=std::declval<const Iter&>())>,
        std::is_same<bool, decltype(std::declval<const Iter&>()>std::declval<const Iter&>())>,
        std::is_same<bool, decltype(std::declval<const Iter&>()>=std::declval<const Iter&>())>,
        bidrectional_iter_require<Iter, std::true_type>>,
      IfType >;

  template < class T, class IfType = std::nullptr_t >
  using arithmetic4_require
    = std::enable_if_t<conjunction_v<
        std::is_same<T, std::decay_t<decltype(std::declval<const T&>() + std::declval<const T&>())>>,
        std::is_same<T, std::decay_t<decltype(std::declval<const T&>() - std::declval<const T&>())>>, 
        std::is_same<T, std::decay_t<decltype(std::declval<const T&>() * std::declval<const T&>())>>, 
        std::is_same<T, std::decay_t<decltype(std::declval<const T&>() / std::declval<const T&>())>>>,
      IfType >;

  template < class T, class IfType = std::nullptr_t >
  using equality_comparable_require
    = std::enable_if_t<conjunction_v<
        std::is_same<bool, decltype(std::declval<const T&>() == std::declval<const T&>())>,
        std::is_same<bool, decltype(std::declval<const T&>() != std::declval<const T&>())>>,
      IfType >;

  template < class T, class IfType = std::nullptr_t >
  using comparable_require
    = std::enable_if_t<conjunction_v<
        equality_comparable_require<T,std::true_type>,
        std::is_same<bool, decltype(std::declval<const T&>() < std::declval<const T&>())>,
        std::is_same<bool, decltype(std::declval<const T&>() <= std::declval<const T&>())>,
        std::is_same<bool, decltype(std::declval<const T&>() > std::declval<const T&>())>,
        std::is_same<bool, decltype(std::declval<const T&>() >= std::declval<const T&>())>>,
      IfType >;

}

#endif

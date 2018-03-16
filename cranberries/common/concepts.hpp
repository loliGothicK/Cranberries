#ifndef CRANBERRIES_RANGES_CONCEPTS_HPP
#define CRANBERRIES_RANGES_CONCEPTS_HPP
#include "../detection_toolkit.hpp"
#include "../pack_operations.hpp"
#include "../type_traits.hpp"
#include "../utility/utility.hpp"

namespace cranberries {
namespace concepts {

template <class T, class... Concepts>
struct required : conjunction<is_satisfied<Concepts::template requires, T>...> {
};

template <class T, class... Concepts>
constexpr bool required_v = required<T, Concepts...>::value;

template <class T, class... Concepts>
using concept_required =
    std::enable_if_t<required<T, Concepts...>::value, std::nullptr_t>;

template <class T, class... Concepts>
using concept_required_not =
    std::enable_if_t<!required<T, Concepts...>::value, std::nullptr_t>;

template <class, class... Concepts>
struct concept_required_seq_impl;

template <template <class...> class Seq, class... tPack, class... Concepts>
struct concept_required_seq_impl<Seq<tPack...>, Concepts...> {
  using type = std::enable_if_t<conjunction_v<required<tPack, Concepts...>...>>;
};

template <class TypePack, class... Concepts>
using concept_required_seq =
    typename concept_required_seq_impl<TypePack, Concepts...>::type;

template <template <class> class Pred>
struct concept_ {
  template <typename T>
  using requires = std::enable_if_t<Pred<T>::value>;
};

template <class... Concepts>
struct combined_concept {
  template <class T>
  using requires = required<T, Concepts...>;
};

struct incrementable {
  template <class T>
  using requires = decltype(++std::declval<T &>());
};

struct decrementable {
  template <class T>
  using requires = decltype(--std::declval<T &>());
};

struct sized {
  template <class T>
  using requires = decltype(back_magic::size(std::declval<T &>()));
};

struct iterator_requirements {
  template <class Iter>
  using requires = std::enable_if_t<conjunction_v<
      std::is_same<Iter, decltype(std::declval<Iter &>()++)>,
      std::is_same<Iter &, decltype(++std::declval<Iter &>())>,
      std::is_copy_constructible<Iter>, std::is_copy_assignable<Iter>,
      std::is_destructible<Iter>, is_dereferencable<Iter>, is_swappable<Iter>,
      always_t<typename std::iterator_traits<Iter>::difference_type>,
      always_t<typename std::iterator_traits<Iter>::value_type>,
      always_t<typename std::iterator_traits<Iter>::pointer>,
      always_t<typename std::iterator_traits<Iter>::reference>,
      always_t<typename std::iterator_traits<Iter>::iterator_category>>>;
};

struct input_iterator_requirements {
  template <class Iter>
  using requires = std::enable_if_t<conjunction_v<
      required<Iter, iterator_requirements>,
      std::is_constructible<bool, decltype(std::declval<const Iter &>() ==
                                           std::declval<const Iter &>())>,
      std::is_constructible<bool, decltype(std::declval<const Iter &>() !=
                                           std::declval<const Iter &>())>>>;
};

struct output_iterator_requirements {
  template <class Iter>
  using requires = std::enable_if_t<
      conjunction_v<required<Iter, iterator_requirements>,
                    std::is_assignable<decltype(*std::declval<Iter>()),
                                       decltype(*std::declval<Iter>())>>>;
};

struct forward_iterator_requirements {
  template <class Iter>
  using requires = std::enable_if_t<
      conjunction_v<required<Iter, input_iterator_requirements>,
                    required<Iter, output_iterator_requirements>,
                    std::is_default_constructible<Iter>>>;
};

struct bidirectional_iterator_requirements {
  template <class Iter>
  using requires = std::enable_if_t<
      conjunction_v<std::is_same<Iter, decltype(std::declval<Iter &>()--)>,
                    std::is_same<Iter &, decltype(--std::declval<Iter &>())>,
                    required<Iter, forward_iterator_requirements>>>;
};

struct random_access_iterator_requirements {
  template <class Iter>
  using requires = std::enable_if_t<conjunction_v<
      std::is_same<Iter, decltype(std::declval<const Iter &>() + 2)>,
      std::is_same<Iter, decltype(std::declval<const Iter &>() - 2)>,
      std::is_same<Iter &, decltype(std::declval<Iter &>() += 2)>,
      std::is_same<Iter &, decltype(std::declval<Iter &>() -= 2)>,
      always_t<decltype(
          bool(std::declval<const Iter &>() < std::declval<const Iter &>()))>,
      always_t<decltype(
          bool(std::declval<const Iter &>() <= std::declval<const Iter &>()))>,
      always_t<decltype(
          bool(std::declval<const Iter &>() > std::declval<const Iter &>()))>,
      always_t<decltype(
          bool(std::declval<const Iter &>() >= std::declval<const Iter &>()))>,
      required<Iter, bidirectional_iterator_requirements>>>;
};

struct four_arithmetic_requirements {
  template <class T>
  using requires = std::enable_if_t<conjunction_v<
      std::is_convertible<T, std::decay_t<decltype(std::declval<const T &>() +
                                                   std::declval<const T &>())>>,
      std::is_convertible<T, std::decay_t<decltype(std::declval<const T &>() -
                                                   std::declval<const T &>())>>,
      std::is_convertible<T, std::decay_t<decltype(std::declval<const T &>() *
                                                   std::declval<const T &>())>>,
      std::is_convertible<T,
                          std::decay_t<decltype(std::declval<const T &>() /
                                                std::declval<const T &>())>>>>;
};

// 振る舞いについては定義を信頼する
struct equality_requirements {
  template <class T>
  using requires = std::enable_if_t<
      conjunction_v<always_t<decltype(bool(std::declval<const T &>() ==
                                           std::declval<const T &>()))>,
                    always_t<decltype(bool(std::declval<const T &>() !=
                                           std::declval<const T &>()))>>>;
};

// 振る舞いについては定義を信頼する
struct full_equivalence_requirements {
  template <class T>
  using requires = std::enable_if_t<conjunction_v<
      required<T, equality_requirements>,
      std::is_constructible<bool, decltype(std::declval<const T &>() <
                                           std::declval<const T &>())>,
      std::is_constructible<bool, decltype(std::declval<const T &>() <=
                                           std::declval<const T &>())>,
      std::is_constructible<bool, decltype(std::declval<const T &>() >
                                           std::declval<const T &>())>,
      std::is_constructible<bool, decltype(std::declval<const T &>() >=
                                           std::declval<const T &>())>>>;
};

struct minimal_equivalence_requirements {
  template <class T>
  using requires = std::enable_if_t<
      std::is_constructible<bool, decltype(std::declval<const T &>() <
                                           std::declval<const T &>())>::value>;
};

/*
|---------------------------|
|	Type Classification				|
|---------------------------|
|	Concept				|	Operation	|
|---------------|-----------|
|								|	T a;
| |	Semi-regular	|	T a = b;	| |
|	~T(a);		|
|								|	a = b;
|
|---------------------------|
|	Regular				|	a == b;		|
|								| a != b;
|
|---------------------------|
|	Ordered				|	a < b;		|
|---------------------------|
*/

using regular_type =
    combined_concept<concept_<std::is_default_constructible>,
                     concept_<std::is_copy_constructible>,
                     concept_<std::is_copy_assignable>, equality_requirements>;

using semi_ragular_type =
    combined_concept<concept_<std::is_default_constructible>,
                     concept_<std::is_copy_constructible>,
                     concept_<std::is_copy_assignable>>;

using ordered = minimal_equivalence_requirements;

struct iterable {
  template <class T>
  using Begin = iterator_requirements::requires<decltype(
      back_magic::begin(std::declval<T &>()))>;
  template <class T>
  using End =
      regular_type::requires<decltype(back_magic::end(std::declval<T &>()))>;

  template <class T>
  using requires = required<T, concept_<Begin>, concept_<End>>;
};

struct reverse_iterable {
  template <class T>
  using Begin = iterator_requirements::requires<decltype(
      back_magic::rbegin(std::declval<T &>()))>;
  template <class T>
  using End =
      regular_type::requires<decltype(back_magic::rend(std::declval<T &>()))>;

  template <class T>
  using requires = required<T, concept_<Begin>, concept_<End>>;
};

struct forward_iterable {
  template <class T>
  using requires = forward_iterator_requirements::requires<T>;
};

struct random_access_iterable {
  template <class T>
  using requires = random_access_iterator_requirements::requires<T>;
};

struct bounded {
  template <class T>
  using requires = std::enable_if_t<
      std::is_same<decltype(back_magic::begin(std::declval<T &>())),
                   decltype(back_magic::end(std::declval<T &>()))>::value>;
};

struct limited {
  template <class T>
  using is_unlimited = typename std::decay_t<T>::is_unlimited;

  template <class T>
  using requires = std::enable_if_t<is_satisfied_v<is_unlimited, T>>;
};

struct writable {
  template <class T>
  using requires = decltype(std::declval<T>() = std::declval<T>());
};

struct printable {
  template <class T>
  using requires = decltype(std::cout << std::declval<const T &>());
};

}  // namespace concepts
}  // namespace cranberries

#endif
/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_FUNCTION_UTILITY_FOR_GCC_CLANG_HPP
#define CRANBERRIES_FUNCTION_UTILITY_FOR_GCC_CLANG_HPP

#include <array>
#include <initializer_list>
#include <random>
#include <tuple>
#include <type_traits>
#include <utility>
#include "../../algorithm.hpp"
#include "../../common/defaulted_type.hpp"
#include "../../math.hpp"
#include "../../pack_operations.hpp"
#include "../../type_traits.hpp"
#include "../../utility/utility.hpp"

namespace cranberries {
namespace func_util {

template <class F, class A, enabler_t<!enable_get_v<A>> = nullptr>
constexpr decltype(auto) operator<<=(F &&f, A &&a) noexcept(
    noexcept(f(std::forward<A>(a)))) {
  return f(std::forward<A>(a));
}

template <class F, class A, enabler_t<enable_get_v<A>> = nullptr>
constexpr decltype(auto) operator<<=(F &&f, A &&a) noexcept(
    noexcept(cranberries::apply(std::forward<F>(f), std::forward<A>(a)))) {
  return cranberries::apply(std::forward<F>(f), std::forward<A>(a));
}

namespace cranberries_magic {
template <class F, class Tuple, size_t... I>
constexpr std::enable_if_t<disjunction_v<std::is_void<std::result_of_t<
    F(decltype(std::get<I>(std::declval<std::decay_t<Tuple>>())))>>...>>
each_(F &&f, Tuple &&tup, std::index_sequence<I...>) noexcept(
    conjunction_v<bool_constant<
        noexcept(std::declval<F>()(std::get<I>(std::declval<Tuple>())))>...>) {
  using swallow = std::initializer_list<int>;
  (void)swallow{(void(f(std::get<I>(tup))), 0)...};
}

template <class F, class Tuple, size_t... I>
constexpr std::enable_if_t<
    conjunction_v<negation<std::is_void<std::result_of_t<
        F(decltype(std::get<I>(std::declval<std::decay_t<Tuple>>())))>>>...>,
    std::tuple<std::result_of_t<
        F(decltype(std::get<I>(std::declval<std::decay_t<Tuple>>())))>...>>
each_(F &&f, Tuple &&tup, std::index_sequence<I...>) noexcept(
    conjunction_v<bool_constant<
        noexcept(std::declval<F>()(std::get<I>(std::declval<Tuple>())))>...>) {
  return {f(std::get<I>(tup))...};
}
}  // namespace cranberries_magic

template <class F, class Tuple>
constexpr decltype(auto) operator|=(F &&f, Tuple &&tup) {
  return cranberries_magic::each_(
      std::forward<F>(f), std::forward<Tuple>(tup),
      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

template <class F, class A, enabler_t<enable_get_v<A>> = nullptr>
constexpr auto operator|(A &&a, F &&f)
    -> decltype(cranberries::apply(std::forward<F>(f), std::forward<A>(a))) {
  return cranberries::apply(std::forward<F>(f), std::forward<A>(a));
}

template <class F, class A, enabler_t<!enable_get_v<A>> = nullptr>
constexpr auto operator|(A &&a, F &&f) -> decltype(f(std::forward<A>(a))) {
  return f(std::forward<A>(a));
}

namespace cranberries_magic {

template <size_t N, class... Args>
struct chunk_each_proxy {
  std::tuple<Args...> tup;

  template <size_t I, class F, class Tuple, size_t... Indices>
  static constexpr decltype(auto)
  invoke(F &&f, Tuple &&t, std::index_sequence<Indices...>) noexcept(
      noexcept(f(std::get<I * N + Indices>(t)...))) {
    return f(std::get<I * N + Indices>(t)...);
  }

  template <class F, size_t... I>
  constexpr std::enable_if_t<disjunction_v<std::is_void<
      decltype(invoke<I>(std::declval<F>(), std::declval<std::tuple<Args...>>(),
                         std::make_index_sequence<N>{}))>...>>
  apply(F &&f, std::index_sequence<I...>) const noexcept(
      conjunction_v<bool_constant<noexcept(invoke<I>(
          std::forward<F>(f), tup, std::make_index_sequence<N>{}))>...>) {
    using swallow = std::initializer_list<int>;
    (void)swallow{(
        void(invoke<I>(std::forward<F>(f), tup, std::make_index_sequence<N>{})),
        0)...};
  }

  template <class F, size_t... I>
  constexpr std::enable_if_t<
      conjunction_v<negation<std::is_void<decltype(
          invoke<I>(std::declval<F>(), std::declval<std::tuple<Args...>>(),
                    std::make_index_sequence<N>{}))>>...>,
      std::tuple<decltype(invoke<I>(std::declval<F>(),
                                    std::declval<std::tuple<Args...>>(),
                                    std::make_index_sequence<N>{}))...>>
  apply(F &&f, std::index_sequence<I...>) const noexcept(
      conjunction_v<bool_constant<noexcept(invoke<I>(
          std::forward<F>(f), tup, std::make_index_sequence<N>{}))>...>) {
    return {
        invoke<I>(std::forward<F>(f), tup, std::make_index_sequence<N>{})...};
  }
};

template <size_t N, class... Args>
struct adjacent_each_proxy {
  std::tuple<Args...> tup;

  template <size_t I, class F, class Tuple, size_t... Indices>
  static constexpr decltype(auto)
  invoke(F &&f, Tuple &&t, std::index_sequence<Indices...>) noexcept(
      noexcept(f(std::get<I + Indices>(t)...))) {
    return f(std::get<I + Indices>(t)...);
  }

  template <class F, size_t... I>
  constexpr std::enable_if_t<disjunction_v<std::is_void<
      decltype(invoke<I>(std::declval<F>(), std::declval<std::tuple<Args...>>(),
                         std::make_index_sequence<N>{}))>...>>
  apply(F &&f, std::index_sequence<I...>) const noexcept(
      conjunction_v<bool_constant<noexcept(invoke<I>(
          std::forward<F>(f), tup, std::make_index_sequence<N>{}))>...>) {
    using swallow = std::initializer_list<int>;
    (void)swallow{(
        void(invoke<I>(std::forward<F>(f), tup, std::make_index_sequence<N>{})),
        0)...};
  }

  template <class F, size_t... I>
  constexpr std::enable_if_t<
      conjunction_v<negation<std::is_void<decltype(
          invoke<I>(std::declval<F>(), std::declval<std::tuple<Args...>>(),
                    std::make_index_sequence<N>{}))>>...>,
      std::tuple<decltype(invoke<I>(std::declval<F>(),
                                    std::declval<std::tuple<Args...>>(),
                                    std::make_index_sequence<N>{}))...>>
  apply(F &&f, std::index_sequence<I...>) const noexcept(
      conjunction_v<bool_constant<noexcept(invoke<I>(
          std::forward<F>(f), tup, std::make_index_sequence<N>{}))>...>) {
    return {
        invoke<I>(std::forward<F>(f), tup, std::make_index_sequence<N>{})...};
  }
};

template <class T, size_t N, size_t Times, size_t ElemSize>
class random_sample_chunk_each_proxy {
  std::array<T, ElemSize> samples;
  std::mt19937 mt;
  std::uniform_int_distribution<size_t> index_gen;

 public:
  random_sample_chunk_each_proxy(std::array<T, ElemSize> arr) noexcept
      : samples{arr}, mt{std::random_device{}()}, index_gen(0, ElemSize - 1) {}

  template <class F, size_t... I, size_t... Loop>
  constexpr std::enable_if_t<std::is_void<std::result_of_t<
      F(std::tuple_element_t<I, std::array<T, ElemSize>>...)>>::value>
  apply(F &&f, std::index_sequence<I...>, std::index_sequence<Loop...>) {
    using swallow = std::initializer_list<size_t>;
    (void)swallow{(void(f(samples[void(I), index_gen(mt)]...)), Loop)...};
  }

  template <class F, size_t... I, size_t... Loop>
  constexpr std::enable_if_t<
      !std::is_void<std::result_of_t<
          F(std::tuple_element_t<I, std::array<T, ElemSize>>...)>>::value,
      generate_tuple_t<std::result_of_t<F(std::tuple_element_t<
                                          I, std::array<T, ElemSize>>...)>,
                       Times>>
  apply(F &&f, std::index_sequence<I...>, std::index_sequence<Loop...>) {
    return {(void(Loop), f(samples[void(I), index_gen(mt)]...))...};
  }
};

template <size_t N, size_t R, class T>
struct permutation_each_proxy {
  std::array<T, N> arr;

  template <class F, size_t... I, size_t... Loop>
  constexpr std::enable_if_t<std::is_void<
      decltype(std::declval<F>()((void(I), std::declval<T>())...))>::value>
  apply(F &&f, swallow_indices<swallow_t<I...>, swallow_t<Loop...>>) {
    using swallow = std::initializer_list<size_t>;
    (void)swallow{(void(f(arr[I]...)),
                   cranberries::next_partial_permutation(
                       begin(arr), begin(arr) + R, end(arr)),
                   Loop)...};
  }

  template <class F, size_t Dummy, size_t... I, size_t... Loop>
  constexpr std::enable_if_t<
      !std::is_void<decltype(std::declval<F>()((void(I),
                                                std::declval<T>())...))>::value,
      generate_tuple_t<decltype(std::declval<F>()((void(I),
                                                   std::declval<T>())...)),
                       sizeof...(Loop) + 1>>
  apply(F &&f, swallow_indices<swallow_t<I...>, swallow_t<Dummy, Loop...>>) {
    return {f(arr[I]...), (void(Loop),
                           void(cranberries::next_partial_permutation(
                               begin(arr), begin(arr) + R, end(arr))),
                           f(arr[I]...))...};
  }
};

template <size_t N, size_t R, class T>
struct combination_each_proxy {
  std::array<T, N> arr;

  template <class F, size_t... I, size_t... Loop>
  constexpr std::enable_if_t<std::is_void<
      decltype(std::declval<F>()((void(I), std::declval<T>())...))>::value>
  apply(F &&f, swallow_indices<swallow_t<I...>, swallow_t<Loop...>>) {
    using swallow = std::initializer_list<size_t>;
    (void)swallow{
        (void(f(arr[I]...)),
         cranberries::next_combination(begin(arr), begin(arr) + R, end(arr)),
         Loop)...};
  }

  template <class F, size_t Dummy, size_t... I, size_t... Loop>
  constexpr std::enable_if_t<
      !std::is_void<decltype(std::declval<F>()((void(I),
                                                std::declval<T>())...))>::value,
      generate_tuple_t<decltype(std::declval<F>()((void(I),
                                                   std::declval<T>())...)),
                       sizeof...(Loop) + 1>>
  apply(F &&f, swallow_indices<swallow_t<I...>, swallow_t<Dummy, Loop...>>) {
    return {f(arr[I]...), (void(Loop),
                           void(cranberries::next_combination(
                               begin(arr), begin(arr) + R, end(arr))),
                           f(arr[I]...))...};
  }
};
}  // namespace cranberries_magic

template <class F, size_t N, class... Args>
constexpr auto operator|=(
    F &&f,
    cranberries_magic::chunk_each_proxy<N, Args...>
        proxy) noexcept(noexcept(proxy.apply(std::forward<F>(f),
                                             std::make_index_sequence<
                                                 sizeof...(Args) / N>{}))) {
  return proxy.apply(std::forward<F>(f),
                     std::make_index_sequence<sizeof...(Args) / N>{});
}

template <class F, size_t N, class... Args>
constexpr auto operator|=(
    F &&f,
    cranberries_magic::adjacent_each_proxy<N, Args...>
        proxy) noexcept(noexcept(proxy.apply(std::forward<F>(f),
                                             std::make_index_sequence<
                                                 sizeof...(Args) - N + 1>{}))) {
  return proxy.apply(std::forward<F>(f),
                     std::make_index_sequence<sizeof...(Args) - N + 1>{});
}

template <class F, class T, size_t N, size_t Times, size_t Size, class... Args>
constexpr auto operator|=(
    F &&f,
    cranberries_magic::random_sample_chunk_each_proxy<T, N, Times, Size>
        proxy) noexcept(noexcept(proxy
                                     .apply(
                                         std::forward<F>(f),
                                         std::make_index_sequence<N>{},
                                         std::make_index_sequence<Times>{}))) {
  return proxy.apply(std::forward<F>(f), std::make_index_sequence<N>{},
                     std::make_index_sequence<Times>{});
}

template <class F, class T, size_t N, size_t R>
constexpr auto operator|=(
    F &&f, cranberries_magic::permutation_each_proxy<N, R, T> proxy) {
  return proxy.apply(std::forward<F>(f),
                     make_swallow_indices<R, permutations(N, R)>{});
}

template <class F, class T, size_t N, size_t R>
constexpr auto operator|=(
    F &&f, cranberries_magic::combination_each_proxy<N, R, T> proxy) {
  return proxy.apply(std::forward<F>(f),
                     make_swallow_indices<R, combinations(N, R)>{});
}

template <size_t Times>
struct randomized : size_constant<Times> {};

template <size_t N, class = cranberries::cranberries_magic::defaulted_t>
struct chunk {
  template <class... Args>
  static constexpr cranberries_magic::chunk_each_proxy<N, std::decay_t<Args>...>
  bind(Args &&... args) noexcept {
    return {std::forward_as_tuple(std::forward<Args>(args)...)};
  }
  template <class... Args>
  static constexpr cranberries_magic::chunk_each_proxy<N, std::decay_t<Args>...>
  bind(const std::tuple<Args...> &tup) noexcept {
    return {tup};
  }
};

template <size_t N, size_t Times>
struct chunk<N, randomized<Times>> {
  template <class... Args>
  static constexpr cranberries_magic::random_sample_chunk_each_proxy<
      pack_element_t<0, type_pack<std::decay_t<Args>...>>, N, Times,
      sizeof...(Args)>
  bind(Args &&... args) noexcept {
    static_assert(is_all_same_v<std::decay_t<Args>...>, "Invaild arguments!");
    static_assert(N < sizeof...(Args), "Too few arguments!");
    return {make_array(std::forward<Args>(args)...)};
  }
  template <class T, size_t Size>
  static constexpr cranberries_magic::random_sample_chunk_each_proxy<
      T, N, Times, Size>
  bind(const std::array<T, Size> &arr) noexcept {
    static_assert(N < Size, "Too few arguments!");
    return {arr};
  }
};

template <size_t N>
struct adjacent {
  template <class... Args>
  static constexpr cranberries_magic::adjacent_each_proxy<N,
                                                          std::decay_t<Args>...>
  bind(Args &&... args) noexcept {
    return {std::forward_as_tuple(std::forward<Args>(args)...)};
  }
  template <class... Args>
  static constexpr cranberries_magic::adjacent_each_proxy<N,
                                                          std::decay_t<Args>...>
  bind(std::tuple<Args...> tup) noexcept {
    return {tup};
  }
};

template <size_t R = 0>
struct permutation {
  template <class... Args>
  static constexpr cranberries_magic::permutation_each_proxy<
      sizeof...(Args), R == 0 ? sizeof...(Args) : R,
      pack_element_t<0, type_pack<std::decay_t<Args>...>>>
  bind(Args &&... args) noexcept {
    return {{{std::forward<Args>(args)...}}};
  }
};

template <size_t R = 0>
struct combination {
  template <class... Args>
  static constexpr cranberries_magic::combination_each_proxy<
      sizeof...(Args), R == 0 ? sizeof...(Args) : R,
      pack_element_t<0, type_pack<std::decay_t<Args>...>>>
  bind(Args &&... args) noexcept {
    return {{{std::forward<Args>(args)...}}};
  }
};

}  // namespace func_util
}  // namespace cranberries

#endif
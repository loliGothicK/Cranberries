#ifndef CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_PERTITION_HPP
#define CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_PERTITION_HPP
#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include "../../common/concepts.hpp"
#include "../../functional/relational.hpp"
#include "../../optional.hpp"
#include "../detail/default_sentinel.hpp"
#include "../detail/sentinel_iterator.hpp"
#include "../detail/tags.hpp"
#undef min
#undef max

namespace cranberries {
inline namespace experimental {
namespace traversals {

enum class map_flag { ordered, unordered };

template <class Pred, map_flag = map_flag::ordered,
          template <class T, class Alloc = std::allocator<T>> class Container =
              std::vector>
class ToMap : private tag::adaptor_tag {
  Pred pred_;

 public:
  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires =
      std::enable_if_t<concepts::required_v<Range, concepts::iterable>>;

  ToMap(Pred pred) : pred_{pred} {}

  template <
      class Traversable,
      std::enable_if_t<
          is_callable_v<Pred(typename std::decay_t<Traversable>::value_type)>,
          std::nullptr_t> = nullptr>
  auto operator()(Traversable &&trv) {
    std::map<bool,
             Container<decltype(*(::cranberries::back_magic::begin(trv)))>>
        result{};
    for (auto const &value : trv) result[pred_(value)].emplace_back(value);
    return result;
  }
};

template <class Pred,
          template <class T, class Alloc = std::allocator<T>> class Container>
class ToMap<Pred, map_flag::unordered, Container> : private tag::adaptor_tag {
  Pred pred_;

 public:
  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires =
      std::enable_if_t<concepts::required_v<Range, concepts::iterable>>;

  ToMap(Pred pred) : pred_{pred} {}

  template <
      class Traversable,
      std::enable_if_t<
          is_callable_v<Pred(typename std::decay_t<Traversable>::value_type)>,
          std::nullptr_t> = nullptr>
  auto operator()(Traversable &&trv) {
    std::unordered_map<
        bool, Container<decltype(*(::cranberries::back_magic::begin(trv)))>>
        result{};
    for (auto const &value : trv) result[pred_(value)].emplace_back(value);
    return result;
  }
};

template <class Pred, template <class T, class Alloc = std::allocator<T>>
                      class Container = std::vector>
class Partition : private tag::adaptor_tag {
  Pred pred_;

 public:
  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires =
      std::enable_if_t<concepts::required_v<Range, concepts::iterable>>;

  Partition(Pred pred) : pred_{pred} {}

  template <class Traversable,
            std::enable_if_t<
                is_callable_v<
                    Pred(typename std::decay_t<Traversable>::value_type), bool>,
                std::nullptr_t> = nullptr>
  auto operator()(Traversable &&trv) {
    std::array<Container<decltype(*(::cranberries::back_magic::begin(trv)))>, 2>
        result{};
    for (auto const &value : trv) {
      if (pred_(value))
        result[static_cast<size_t>(true)].emplace_back(value);
      else
        result[static_cast<size_t>(false)].emplace_back(value);
    }
    return result;
  }
};

namespace detail_ {
enum class fold_mode {
  left,
  right,
};
enum class try_fold {
  enable,
  disable,
};

enum class fold_init {
  with,
  without,
};

template <fold_mode>
struct IntoIter;

template <>
struct IntoIter<fold_mode::left> {
  template <class Traversable>
  static decltype(auto) begin(Traversable &trv) {
    return ::cranberries::back_magic::begin(trv);
  }
  template <class Traversable>
  static decltype(auto) end(Traversable &trv) {
    return ::cranberries::back_magic::end(trv);
  }
};
template <>
struct IntoIter<fold_mode::right> {
  template <class Traversable>
  static decltype(auto) begin(Traversable &trv) {
    return ::cranberries::back_magic::rbegin(trv);
  }
  template <class Traversable>
  static decltype(auto) end(Traversable &trv) {
    return ::cranberries::back_magic::rend(trv);
  }
};
template <class T, fold_init = fold_init::with>
struct fold_result_type {
  using type = T;
};

template <class T>
struct fold_result_type<T, fold_init::without> {
  using type = cranberries::optional<T>;
};

}  // namespace detail_
template <class, detail_::try_fold, detail_::fold_init>
struct fold_impl_injection;

template <class Derived, detail_::fold_init Ini_>
struct fold_impl_injection<Derived, detail_::try_fold::enable, Ini_> {
  template <class Traversable,
            enabler_t<!is_specialize_of_v<
                cranberries::optional,
                typename remove_cvr_t<Traversable>::value_type>> = nullptr>
  cranberries::optional<typename remove_cvr_t<Traversable>::value_type>
  operator()(Traversable &&trv) try {
    auto iter = static_cast<Derived *>(this)->begin(trv);
    auto last = static_cast<Derived *>(this)->end(trv);
    if (iter == last) return {static_cast<Derived *>(this)->default_value()};
    auto result_ = static_cast<Derived *>(this)->init(iter);
    do {
      result_ = static_cast<Derived *>(this)->step(result_, *iter);
    } while (++iter != last);
    return result_;
  } catch (...) {
    return {cranberries::nullopt};
  }

  template <class Traversable,
            enabler_t<is_specialize_of_v<
                cranberries::optional,
                typename remove_cvr_t<Traversable>::value_type>> = nullptr>
  cranberries::optional<
      typename remove_cvr_t<Traversable>::value_type::value_type>
  operator()(Traversable &&trv) noexcept try {
    auto iter = static_cast<Derived *>(this)->begin(trv);
    auto last = static_cast<Derived *>(this)->end(trv);
    if (iter == last) return {static_cast<Derived *>(this)->default_value()};
    auto result_ = static_cast<Derived *>(this)->init(iter);
    do {
      if (!result_ && !*iter) goto DETECT_NULLOPT;
      result_ =
          static_cast<Derived *>(this)->step(result_.value(), (*iter).value());
    } while (++iter != last);
    return result_;
  DETECT_NULLOPT:
    return {cranberries::nullopt};
  } catch (...) {
    return {cranberries::nullopt};
  }
};

template <class Derived, detail_::fold_init Ini_>
struct fold_impl_injection<Derived, detail_::try_fold::disable, Ini_> {
  template <class Traversable,
            class E = typename remove_cvr_t<Traversable>::value_type>
  typename detail_::fold_result_type<E, Ini_>::type operator()(
      Traversable &&trv) noexcept {
    auto iter = static_cast<Derived *>(this)->begin(trv);
    auto last = static_cast<Derived *>(this)->end(trv);
    if (iter == last) return {};
    auto result_ = static_cast<Derived *>(this)->init(iter);
    do {
      result_ = static_cast<Derived *>(this)->step(result_, *iter);
    } while (++iter != last);
    return result_;
  }
};

template <class Op, class T = cranberries::cranberries_magic::defaulted_t>
struct FoldBind {
  Op op;
  T init_;

  T default_value() { return init_; }
  T init(...) { return init_; }
  template <class T1, class T2>
  auto operator()(T1 &&a, T2 &&b) {
    return op(std::forward<T1>(a), std::forward<T2>(b));
  }
};

template <class Op>
struct FoldBind<Op, cranberries::cranberries_magic::defaulted_t> {
  Op op;

  cranberries::nullopt_t default_value() { return {}; }
  template <class Iter>
  auto init(Iter &iter) {
    auto tmp = *iter;
    ++iter;
    return tmp;
  }
  template <class T1, class T2>
  auto operator()(T1 &&a, T2 &&b) {
    return op(std::forward<T1>(a), std::forward<T2>(b));
  }
};

template <detail_::fold_mode Mode_, detail_::try_fold Try_,
          detail_::fold_init Ini_, class Op>
class Fold
    : private tag::adaptor_tag,
      public fold_impl_injection<Fold<Mode_, Try_, Ini_, Op>, Try_, Ini_> {
  Op op;

 public:
  template <class Op_>
  Fold(Op_ &&op) : op{std::forward<Op_>(op)} {}

  template <class Traversable>
  auto begin(Traversable &trv) {
    return detail_::IntoIter<Mode_>::begin(trv);
  }

  template <class Traversable>
  auto end(Traversable &trv) {
    return detail_::IntoIter<Mode_>::end(trv);
  }

  template <class Iter>
  auto init(Iter &iter) {
    return op.init(iter);
  }

  auto default_value() { return op.default_value(); }

  template <class T1, class T2>
  auto step(T1 &&a, T2 &&b) {
    return op(std::forward<T1>(a), std::forward<T2>(b));
  }

  template <class Traversable,
            typename ElemType = typename std::decay_t<Traversable>::value_type>
  using requires = std::enable_if_t<
      conjunction_v<concepts::required<Traversable, concepts::iterable>>>;
};

namespace detail_ {
template <fold_mode Mode_, try_fold Try_, class Op>
Fold<Mode_, Try_, fold_init::without, FoldBind<Op>> make_fold(Op &&op) {
  return {FoldBind<Op>{std::forward<Op>(op)}};
}
template <fold_mode Mode_, try_fold Try_, class Init, class Op>
Fold<Mode_, Try_, fold_init::with, FoldBind<Op, Init>> make_fold(Init init,
                                                                 Op &&op) {
  return {FoldBind<Op, Init>{std::forward<Op>(op), init}};
}
}  // namespace detail_

class Sum : private tag::adaptor_tag {
 public:
  Sum() = default;

  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires = std::enable_if_t<
      conjunction_v<concepts::required<Range, concepts::iterable>,
                    concepts::required<ElemType, concepts::regular_type>,
                    always_t<decltype(std::declval<ElemType &>() +=
                                      std::declval<const ElemType &>())>>>;

  template <class Traversable, typename ResultType = typename remove_cvr_t<
                                   Traversable>::value_type>
  ResultType operator()(Traversable &&traversal) {
    ResultType ret{};
    for (auto const &e : traversal) ret += e;
    return ret;
  }
};

enum class minmax_flag {
  min,
  max,
  minmax,
};

template <class Derived, minmax_flag>
struct minmax_impl_injection;

template <class Derived>
struct minmax_impl_injection<Derived, minmax_flag::min> {
  template <class Traversable>
  auto operator()(Traversable &&traversal) {
    using result_type =
        ::cranberries::optional<typename remove_cvr_t<Traversable>::value_type>;
    using std::begin;
    using std::end;
    auto iter = begin(traversal);
    auto last = end(traversal);
    if (iter == last) return result_type{cranberries::nullopt};

    auto min_ = *iter;
    while (++iter != last)
      if (static_cast<Derived *>(this)->comp(*iter, min_)) {
        min_ = *iter;
      }

    return result_type{min_};
  }
};

template <class Derived>
struct minmax_impl_injection<Derived, minmax_flag::max> {
  template <class Traversable>
  auto operator()(Traversable &&traversal) {
    using result_type =
        ::cranberries::optional<typename remove_cvr_t<Traversable>::value_type>;
    using std::begin;
    using std::end;
    auto iter = begin(traversal);
    auto last = end(traversal);
    if (iter == last) return result_type{cranberries::nullopt};

    auto max_ = *iter;
    while (++iter != last)
      if (static_cast<Derived *>(this)->comp(max_, *iter)) {
        max_ = *iter;
      }

    return result_type{max_};
  }
};

template <class Derived>
struct minmax_impl_injection<Derived, minmax_flag::minmax> {
  template <class Traversable>
  auto operator()(Traversable &&traversal) {
    using result_type = ::cranberries::optional<
        std::pair<typename remove_cvr_t<Traversable>::value_type,
                  typename remove_cvr_t<Traversable>::value_type>>;
    using std::begin;
    using std::end;
    auto iter = begin(traversal);
    auto last = end(traversal);
    if (iter == last) return result_type{cranberries::nullopt};

    auto max_ = *iter;
    auto min_ = *iter;
    while (++iter != last) {
      if (static_cast<Derived *>(this)->comp(*iter, min_)) {
        min_ = *iter;
      }

      if (static_cast<Derived *>(this)->comp(max_, *iter)) {
        max_ = *iter;
      }
    }
    return result_type{std::make_pair(min_, max_)};
  }
};

template <minmax_flag Flag, class Pred = cranberries::less<>>
class MinMax : tag::adaptor_tag,
               public minmax_impl_injection<MinMax<Flag, Pred>, Flag> {
  Pred pred_;

 public:
  template <class Pred_>
  constexpr MinMax(Pred_ &&pred) noexcept : pred_{std::forward<Pred_>(pred)} {}

  constexpr MinMax() noexcept : pred_{} {}

  template <class L, class R>
  bool comp(L &&l, R &&r) {
    return pred_(std::forward<L>(l), std::forward<R>(r));
  }

  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires = std::enable_if_t<
      conjunction_v<concepts::required<Range, concepts::iterable>,
                    concepts::required<ElemType, concepts::regular_type>>>;
};
namespace unzip_result_type {
template <class>
struct result_type;

template <class... Args>
struct result_type<std::tuple<Args...>>
    : nested_type_class<std::tuple<std::vector<Args>...>> {};
}  // namespace unzip_result_type

class Unzip : tag::adaptor_tag {
 public:
  constexpr Unzip() noexcept {}

  template <typename Range,
            typename ElemType = typename std::decay_t<Range>::value_type>
  using requires = std::enable_if_t<
      conjunction_v<concepts::required<Range, concepts::iterable>,
                    concepts::required<ElemType, concepts::regular_type>>>;

  template <size_t I, class ResultTuple, class Tuple>
  static std::nullptr_t assign(ResultTuple &result, Tuple &tup) {
    return std::get<I>(result).emplace_back(std::get<I>(tup)), nullptr;
  }

  template <class ResultTuple, class Tuple, size_t... Indices>
  static void decompose(ResultTuple &result, Tuple &tup,
                        std::index_sequence<Indices...>) {
    SwallowsNest{assign<Indices>(result, tup)...};
  }

  template <class Traversable>
  decltype(auto) operator()(Traversable &&trv) noexcept {
    typename unzip_result_type::result_type<
        typename remove_cvr_t<Traversable>::value_type>::type result;
    for (auto &&e : trv)
      decompose(result, e,
                std::make_index_sequence<
                    std::tuple_size<remove_cvr_t<decltype(e)>>::value>{});
    return result;
  }
};

namespace collect {
template <class Pred>
ToMap<Pred> to_map(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <template <class T, class Alloc = std::allocator<T>> class Container,
          class Pred>
ToMap<Pred, map_flag::ordered, Container> to_map(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <class Pred>
ToMap<Pred, map_flag::unordered> to_unordered_map(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <template <class T, class Alloc = std::allocator<T>> class Container,
          class Pred>
ToMap<Pred, map_flag::unordered, Container> to_unordered_map(
    Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <class Pred>
Partition<Pred> partition(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <template <class T, class Alloc = std::allocator<T>> class Container,
          class Pred>
Partition<Pred, Container> partition(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <class Op>
auto foldl(Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::left,
                            detail_::try_fold::disable>(std::forward<Op>(op));
}

template <class Op>
auto foldr(Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::right,
                            detail_::try_fold::disable>(std::forward<Op>(op));
}

template <class T, class Op>
auto foldl(T ini_, Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::left,
                            detail_::try_fold::disable>(ini_,
                                                        std::forward<Op>(op));
}

template <class T, class Op>
auto foldr(T ini_, Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::right,
                            detail_::try_fold::disable>(ini_,
                                                        std::forward<Op>(op));
}

template <class Op>
auto try_foldl(Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::left,
                            detail_::try_fold::enable>(std::forward<Op>(op));
}

template <class Op>
auto try_foldr(Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::right,
                            detail_::try_fold::enable>(std::forward<Op>(op));
}

template <class T, class Op>
auto try_foldl(T ini_, Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::left,
                            detail_::try_fold::enable>(ini_,
                                                       std::forward<Op>(op));
}

template <class T, class Op>
auto try_foldr(T ini_, Op &&op) noexcept {
  return detail_::make_fold<detail_::fold_mode::right,
                            detail_::try_fold::enable>(ini_,
                                                       std::forward<Op>(op));
}

inline Sum sum() noexcept { return {}; }

inline MinMax<minmax_flag::min> min() noexcept { return {}; }
inline MinMax<minmax_flag::max> max() noexcept { return {}; }
inline MinMax<minmax_flag::minmax> minmax() noexcept { return {}; }

template <class Pred>
MinMax<minmax_flag::min, Pred> min_by(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}
template <class Pred>
MinMax<minmax_flag::max, Pred> max_by(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}
template <class Pred>
MinMax<minmax_flag::minmax, Pred> minmax_by(Pred &&pred) noexcept {
  return {std::forward<Pred>(pred)};
}

template <class F>
auto min_by_key(F &&f) noexcept {
  return min_by([f = std::forward<F>(f), pred = cranberries::less<>{}](
                    auto &&l, auto &&r) mutable {
    return pred(f(std::forward<decltype(l)>(l)),
                f(std::forward<decltype(r)>(r)));
  });
}
template <class F>
auto max_by_key(F &&f) noexcept {
  return max_by([f = std::forward<F>(f), pred = cranberries::less<>{}](
                    auto &&l, auto &&r) mutable {
    return pred(f(std::forward<decltype(l)>(l)),
                f(std::forward<decltype(r)>(r)));
  });
}
template <class F>
auto minmax_by_key(F &&f) noexcept {
  return minmax_by([f = std::forward<F>(f), pred = cranberries::less<>{}](
                       auto &&l, auto &&r) mutable {
    return pred(f(std::forward<decltype(l)>(l)),
                f(std::forward<decltype(r)>(r)));
  });
}

inline Unzip unzip() noexcept { return {}; }
}  // namespace collect

}  // namespace traversals
}  // namespace experimental
}  // namespace cranberries
#endif  // !CRANBERRIES_RANGES_SENTINEL_RANGES_VIEW_ADAPTOR_CYCLIC_HPP

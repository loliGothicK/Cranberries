/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBRIIES_INTERVAL_LIB_COMPARE_FUNC_OBJ_HPP
#define CRANBRIIES_INTERVAL_LIB_COMPARE_FUNC_OBJ_HPP

#include "../interval.hpp"
#include "order_symbol.hpp"

namespace cranberries {
namespace compare {

/*  interval compare function object  */
template <OrderPolocy Policy = OrderPolocy::Weak>
struct less;

template <>
struct less<OrderPolocy::Weak> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return weak_less(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return weak_less(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return weak_less(a, b);
  }
};

template <>
struct less<OrderPolocy::Partial> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return partial_less(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return partial_less(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return partial_less(a, b);
  }
};

template <>
struct less<OrderPolocy::Interval> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<L> const &b) const
      noexcept {
    return interval_less(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return interval_less(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return interval_less(a, b);
  }
};

template <OrderPolocy Policy = OrderPolocy::Weak>
struct less_or_equal;

template <>
struct less_or_equal<OrderPolocy::Weak> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return weak_less_or_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return weak_less_or_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return weak_less_or_equal(a, b);
  }
};

template <>
struct less_or_equal<OrderPolocy::Partial> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return partial_less_or_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return partial_less_or_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return partial_less_or_equal(a, b);
  }
};

template <>
struct less_or_equal<OrderPolocy::Interval> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<L> const &b) const
      noexcept {
    return interval_less_or_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return interval_less_or_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return interval_less_or_equal(a, b);
  }
};

template <OrderPolocy Policy = OrderPolocy::Weak>
struct greater;

template <>
struct greater<OrderPolocy::Weak> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return weak_greater(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return weak_greater(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return weak_greater(a, b);
  }
};

template <>
struct greater<OrderPolocy::Partial> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return partial_greater(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return partial_greater(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return partial_greater(a, b);
  }
};

template <>
struct greater<OrderPolocy::Interval> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return interval_greater(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return interval_greater(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return interval_greater(a, b);
  }
};

template <OrderPolocy Policy = OrderPolocy::Weak>
struct greater_or_equal;

template <>
struct greater_or_equal<OrderPolocy::Weak> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return weak_greater_or_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return weak_greater_or_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return weak_greater_or_equal(a, b);
  }
};

template <>
struct greater_or_equal<OrderPolocy::Partial> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return partial_greater_or_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return partial_greater_or_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return partial_greater_or_equal(a, b);
  }
};

template <>
struct greater_or_equal<OrderPolocy::Interval> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<L> const &b) const
      noexcept {
    return interval_greater_or_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return interval_greater_or_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return interval_greater_or_equal(a, b);
  }
};

template <OrderPolocy Policy = OrderPolocy::Weak>
struct equal;

template <>
struct equal<OrderPolocy::Weak> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return weak_equal(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &a,
                            interval<R> const &b) const noexcept {
    return weak_equal(a, b);
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &a,
                            typename interval<L>::value_type const &b) const
      noexcept {
    return weak_equal(a, b);
  }
};

template <OrderPolocy Policy = OrderPolocy::Partial>
struct undorderesd {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return partial_unordered(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &,
                            interval<R> const &) const noexcept {
    return false;
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &,
                            typename interval<L>::value_type const &) const
      noexcept {
    return false;
  }
};
template <>
struct undorderesd<OrderPolocy::Interval> {
  template <typename L, typename R>
  constexpr bool operator()(interval<L> const &a, interval<R> const &b) const
      noexcept {
    return interval_unordered(a, b);
  }
  template <typename R>
  constexpr bool operator()(typename interval<R>::value_type const &,
                            interval<R> const &) const noexcept {
    return false;
  }
  template <typename L>
  constexpr bool operator()(interval<L> const &,
                            typename interval<L>::value_type const &) const
      noexcept {
    return false;
  }
};

}  // namespace compare
}  // namespace cranberries
#endif
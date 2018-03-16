#ifndef CRANBRIIES_INTERVAL_LIB_COMPARE_ADVANCE_HPP
#define CRANBRIIES_INTERVAL_LIB_COMPARE_ADVANCE_HPP

#include "../interval.hpp"
#include "./interval_order.hpp"
#include "./order_symbol.hpp"
#include "./partial_order.hpp"
#include "./set_order.hpp"
#include "./total_order.hpp"
#include "./weak_order.hpp"

namespace cranberries {
namespace compare {

/*  Advanced Interval Ordering  */
enum class interval_relation {
  equal,
  interval_less,
  interval_greater,
  partial_less,
  partial_greater,
  weak_less,
  weak_greater,
  total_less,
  total_greater,
  contain,
  part_of,
  niether,
};
/*  Interval Relational Discriminator Function  */

template <typename L, typename R>
inline interval_relation relational(interval<L> const &x,
                                    interval<R> const &y) noexcept {
  return (total_equal(x, y))
             ? interval_relation::equal
             : (x.lower() == y.lower() && x.upper() < y.upper())
                   ? interval_relation::total_less
                   : (x.lower() == y.lower() && x.upper() > y.upper())
                         ? interval_relation::total_greater
                         : (x.lower() < y.lower() && x.upper() == y.upper())
                               ? interval_relation::weak_less
                               : (x.lower() > y.lower() &&
                                  x.upper() == y.upper())
                                     ? interval_relation::weak_greater
                                     : (x.lower() < y.lower() &&
                                        x.upper() < y.upper())
                                           ? interval_relation::partial_less
                                           : (x.lower() > y.lower() &&
                                              x.upper() > y.upper())
                                                 ? interval_relation::
                                                       partial_less
                                                 : (x.lower() <= y.lower() &&
                                                    x.upper() >= y.upper())
                                                       ? interval_relation::
                                                             contain
                                                       : (x.lower() >=
                                                              y.lower() &&
                                                          x.upper() <=
                                                              y.upper())
                                                             ? interval_relation::
                                                                   part_of
                                                             : interval_relation::
                                                                   niether;
}
//--------------------------//
/*  Ordering Discriminator  */
//--------------------------//

/*  Interval Ordering  */

template <typename L, typename R>
inline compare::interval_ordering interval_order(
    interval<L> const &x, interval<R> const &y) noexcept {
  return (interval_less(x, y))
             ? compare::interval_ordering::less
             : (interval_greater(x, y)) ? compare::interval_ordering::greater
                                        : compare::interval_ordering::unordered;
}

template <typename L, typename R>
inline compare::interval_ordering interval_order(
    R const &x, interval<L> const &y) noexcept {
  return (interval_less(x, y))
             ? compare::interval_ordering::less
             : (interval_greater(x, y)) ? compare::interval_ordering::greater
                                        : compare::interval_ordering::unordered;
}

template <typename L, typename R>
inline compare::interval_ordering interval_order(interval<L> const &x,
                                                 R const &y) noexcept {
  return (interval_less(x, y))
             ? compare::interval_ordering::less
             : (interval_greater(x, y)) ? compare::interval_ordering::greater
                                        : compare::interval_ordering::unordered;
}

/*  Partial Ordering  */

template <typename L, typename R>
inline compare::partial_ordering partial_order(interval<L> const &x,
                                               interval<R> const &y) noexcept {
  return (partial_less(x, y))
             ? compare::partial_ordering::less
             : (partial_greater(x, y)) ? compare::partial_ordering::greater
                                       : compare::partial_ordering::unordered;
}

template <typename L, typename R>
inline compare::partial_ordering partial_order(R const &x,
                                               interval<L> const &y) noexcept {
  return (partial_less(x, y))
             ? compare::partial_ordering::less
             : (partial_greater(x, y)) ? compare::partial_ordering::greater
                                       : compare::partial_ordering::unordered;
}

template <typename L, typename R>
inline compare::partial_ordering partial_order(interval<L> const &x,
                                               R const &y) noexcept {
  return (partial_less(x, y))
             ? compare::partial_ordering::less
             : (partial_greater(x, y)) ? compare::partial_ordering::greater
                                       : compare::partial_ordering::unordered;
}

/*  Weak ordering  */

template <typename L, typename R>
inline compare::weak_ordering weak_order(interval<L> const &x,
                                         interval<R> const &y) noexcept {
  return (weak_less(x, y))
             ? compare::weak_ordering::less
             : (weak_greater(x, y)) ? compare::weak_ordering::greater
                                    : compare::weak_ordering::equivalent;
}

template <typename L, typename R>
inline compare::weak_ordering weak_order(R const &x,
                                         interval<L> const &y) noexcept {
  return (weak_less(x, y))
             ? compare::weak_ordering::less
             : (weak_greater(x, y)) ? compare::weak_ordering::greater
                                    : compare::weak_ordering::equivalent;
}

template <typename L, typename R>
inline compare::weak_ordering weak_order(interval<L> const &x,
                                         R const &y) noexcept {
  return (weak_less(x, y))
             ? compare::weak_ordering::less
             : (weak_greater(x, y)) ? compare::weak_ordering::greater
                                    : compare::weak_ordering::equivalent;
}

/*  Total Ordering  */

template <typename L, typename R>
inline compare::total_ordering total_order(interval<L> const &x,
                                           interval<R> const &y) noexcept {
  return (total_less(x, y))
             ? compare::total_ordering::less
             : (total_greater(x, y)) ? compare::total_ordering::greater
                                     : compare::total_ordering::equal;
}

template <typename L, typename R>
inline compare::total_ordering total_order(R const &x,
                                           interval<L> const &y) noexcept {
  return (total_less(x, y))
             ? compare::total_ordering::less
             : (total_greater(x, y)) ? compare::total_ordering::greater
                                     : compare::total_ordering::equal;
}

template <typename L, typename R>
inline compare::total_ordering total_order(interval<L> const &x,
                                           R const &y) noexcept {
  return (total_less(x, y))
             ? compare::total_ordering::less
             : (total_greater(x, y)) ? compare::total_ordering::greater
                                     : compare::total_ordering::equal;
}

}  // namespace compare
}  // namespace cranberries
#endif

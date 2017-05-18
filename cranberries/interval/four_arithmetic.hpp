#ifndef CRANBRIIES_INTERVAL_LIB_FOUR_ARITHMETIC_HPP
#define CRANBRIIES_INTERVAL_LIB_FOUR_ARITHMETIC_HPP

#include "interval.hpp"
#include "rounding_control.hpp"
#include "cranberries_magic/detail.hpp"
#include "constants.hpp"
#include "rounding_control.hpp"

namespace cranberries {
  template < typename L, typename R, typename T = std::common_type_t<L, R> >
  inline constexpr interval<T> operator+(interval<L> const& x, interval<R> const& y)
  {
    auto x_low = x.lower();
    auto x_up = x.upper();
    auto y_low = y.lower();
    auto y_up = y.upper();

    return CRANBERRIES_MAKE_INTERVAL(T, x_low + y_low, x_up + y_up);
  }
  template < typename L, typename R, typename T = std::common_type_t<L, R> >
  inline constexpr interval<T> operator-(interval<L> const& x, interval<R> const& y)
  {
    return CRANBERRIES_MAKE_INTERVAL(T, x.lower() - x.upper(), y.upper() - y.lower());
  }
  template < typename T >
  inline constexpr interval<T> operator-(interval<T> const& x, interval<T> const& y)
  {
    return &x == &y ? interval<T>{} : CRANBERRIES_MAKE_INTERVAL(T, x.lower() - x.upper(), y.upper() - y.lower());
  }
  template < typename L, typename R, typename T = std::common_type_t<L, R> >
  inline constexpr interval<T> operator*(interval<L> const& x, interval<R> const& y)
  {
    auto x_lower = x.lower();
    auto x_upper = x.upper();
    auto y_lower = y.lower();
    auto y_upper = y.upper();

    return (x_lower >= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_upper, x_lower * y_lower)
      : [&] {
      auto lower = x_lower * y_upper < x_upper * y_lower ? cranberries_magic::downward_multiply(x_lower, y_upper) : cranberries_magic::downward_multiply(x_upper, y_lower);
      auto upper = x_lower*y_lower < x_upper*y_upper ? cranberries_magic::upward_multiply(x_upper, y_upper) : cranberries_magic::upward_multiply(x_lower, y_lower);
      return CRANBERRIES_MAKE_INTERVAL(T, lower, upper);
    }();
  }
  template < typename T >
  inline constexpr interval<T> operator*(interval<T> const& x, interval<T> const& y)
  {
    if (&x == &y) return pow(x, 2);

    auto x_lower = x.lower();
    auto x_upper = x.upper();
    auto y_lower = y.lower();
    auto y_upper = y.upper();

    return (x_lower >= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_upper, x_lower * y_lower)
      : [&] {
      auto lower = x_lower * y_upper < x_upper * y_lower ? cranberries_magic::downward_multiply(x_lower, y_upper) : cranberries_magic::downward_multiply(x_upper, y_lower);
      auto upper = x_lower*y_lower < x_upper*y_upper ? cranberries_magic::upward_multiply(x_upper, y_upper) : cranberries_magic::upward_multiply(x_lower, y_lower);
      return CRANBERRIES_MAKE_INTERVAL(T, lower, upper);
    }();
  }
  template < typename L, typename R, typename T = std::common_type_t<L, R> >
  inline constexpr interval<T> operator/(interval<L> const& x, interval<R> const& y)
  {
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL(y.lower() <= interval_constants::zero<R> && y.upper() >= interval_constants::zero<R>)
      : operator*(x, CRANBERRIES_MAKE_INTERVAL(T, interval_constants::one<R> / y.upper(), interval_constants::one<R> / y.lower()));
  }
  template < typename T >
  inline constexpr interval<T> operator/(interval<T> const& x, interval<T> const& y)
  {
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL(y.lower() <= interval_constants::zero<T> && y.upper() >= interval_constants::zero<T>)
      : &x == &y
      ? interval<T>{1}
    : operator*(x, CRANBERRIES_MAKE_INTERVAL(T, interval_constants::one<T> / y.upper(), interval_constants::one<T> / y.lower()));
  }
  template < typename T, typename R >
  inline constexpr interval<T>& operator+=(interval<T>& x, interval<R> const& y)
  {
    return x = CRANBERRIES_MAKE_INTERVAL(T, x.lower() + y.lower(), x.upper() + y.upper());
  }
  template < typename T, typename R >
  inline constexpr interval<T>& operator-=(interval<T>& x, interval<R> const& y)
  {
    return x = CRANBERRIES_MAKE_INTERVAL(T, x.lower() - y.upper(), x.upper() - y.lower());
  }
  template < typename T >
  inline constexpr interval<T>& operator-=(interval<T>& x, interval<T> const& y)
  {
    return x = (&x == &y ? interval<T>{} : CRANBERRIES_MAKE_INTERVAL(T, x.lower() - y.upper(), x.upper() - y.lower()));
  }
  template < typename T, typename R >
  inline constexpr interval<T>& operator*=(interval<T>& x, interval<R> const& y)
  {
    auto x_lower = x.lower();
    auto x_upper = x.upper();
    auto y_lower = y.lower();
    auto y_upper = y.upper();

    return x = (x_lower >= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_upper, x_lower * y_lower)
      : [&] {
      auto lower = x_lower * y_upper < x_upper * y_lower ? cranberries_magic::downward_multiply(x_lower, y_upper) : cranberries_magic::downward_multiply(x_upper, y_lower);
      auto upper = x_lower*y_lower < x_upper*y_upper ? cranberries_magic::upward_multiply(x_upper, y_upper) : cranberries_magic::upward_multiply(x_lower, y_lower);
      return CRANBERRIES_MAKE_INTERVAL(T, lower, upper);
    }();
  }
  template < typename T >
  inline constexpr interval<T>& operator*=(interval<T>& x, interval<T> const& y)
  {
    if (&x == &y) return x = pow(x, 2);

    auto x_lower = x.lower();
    auto x_upper = x.upper();
    auto y_lower = y.lower();
    auto y_upper = y.upper();

    return x = (x_lower >= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_upper * y_upper)
      : (x_lower >= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_upper)
      : (x_lower < interval_constants::zero<T> && x_upper > interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_lower, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower >= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_upper * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_lower < interval_constants::zero<T> && y_upper > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_lower * y_upper, x_lower * y_lower)
      : (x_upper <= interval_constants::zero<T> && y_upper <= interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x_upper * y_upper, x_lower * y_lower)
      : [&] {
      auto lower = x_lower * y_upper < x_upper * y_lower ? cranberries_magic::downward_multiply(x_lower, y_upper) : cranberries_magic::downward_multiply(x_upper, y_lower);
      auto upper = x_lower*y_lower < x_upper*y_upper ? cranberries_magic::upward_multiply(x_upper, y_upper) : cranberries_magic::upward_multiply(x_lower, y_lower);
      return CRANBERRIES_MAKE_INTERVAL(T, lower, upper);
    }();
  }
  template < typename T, typename R >
  inline constexpr interval<T>& operator/=(interval<T>& x, interval<R> const& y)
  {
    CRANBERRIES_RANGE_ERROR_THROW_IF(y.lower() <= interval_constants::zero<R> && interval_constants::zero<R> <= y.upper());
    return operator*=(x, CRANBERRIES_MAKE_INTERVAL(T, interval_constants::one<R> / y.upper(), interval_constants::one<R> / y.lower()));
  }
  template < typename T >
  inline constexpr interval<T>& operator/=(interval<T>& x, interval<T> const& y)
  {
    CRANBERRIES_RANGE_ERROR_THROW_IF(y.lower() <= interval_constants::zero<T> && interval_constants::zero<T> <= y.upper());
    if (&x == &y) return x = interval<T>(1, 1);
    return operator*=(x, CRANBERRIES_MAKE_INTERVAL(T, interval_constants::one<T> / y.upper(), interval_constants::one<T> / y.lower()));
  }
  template < typename T >
  inline constexpr interval<T> operator-(typename interval<T>::value_type const& x, interval<T> const& y)
  {
    return CRANBERRIES_MAKE_INTERVAL(T, x - y.upper(), x - y.lower());
  }

  template < typename T >
  inline constexpr interval<T> operator-(interval<T> const& x, typename interval<T>::value_type const& y)
  {
    return CRANBERRIES_MAKE_INTERVAL(T, x.lower() - y, x.upper() - y);
  }

  template < typename T >
  inline constexpr interval<T> operator+(typename interval<T>::value_type const& x, interval<T> const& y)
  {
    return CRANBERRIES_MAKE_INTERVAL(T, x + y.lower(), x + y.upper());
  }

  template < typename T >
  inline constexpr interval<T> operator+(interval<T> const& x, typename interval<T>::value_type const& y)
  {
    return CRANBERRIES_MAKE_INTERVAL(T, x.lower() + y, x.upper() + y);
  }

  template < typename T >
  inline constexpr interval<T> operator*(typename interval<T>::value_type const& x, interval<T> const& y)
  {
    return (x < interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x * y.upper(), x * y.lower())
      : CRANBERRIES_MAKE_INTERVAL(T, x * y.lower(), x * y.upper());
  }

  template < typename T >
  inline constexpr interval<T> operator*(interval<T> const& x, typename interval<T>::value_type const& y)
  {
    return  (y < interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x.upper() * y, x.lower() * y)
      : CRANBERRIES_MAKE_INTERVAL(T, x.lower() * y, x.upper() * y);
  }

  template < typename T >
  inline constexpr interval<T> operator/(interval<T> const& x, typename interval<T>::value_type const& y)
  {
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL(y == interval_constants::zero<T>)
      : (y < interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x.upper() / y, x.lower() / y)
      : CRANBERRIES_MAKE_INTERVAL(T, x.lower() / y, x.upper() / y);
  }

  template < typename T >
  inline constexpr interval<T> operator/(typename interval<T>::value_type const& x, interval<T> const& y)
  {
    return CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL(y.lower() < interval_constants::zero<T> && interval_constants::zero<T> <= y.upper())
      : (y.lower() > interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x / y.upper(), x / y.lower())
      : CRANBERRIES_MAKE_INTERVAL(T, x / y.lower(), x / y.upper());
  }

  template < typename T >
  inline constexpr interval<T>& operator+=(interval<T>& x, typename interval<T>::value_type const& y)
  {
    return x = CRANBERRIES_MAKE_INTERVAL(T, x.lower() + y, x.upper() + y);
  }

  template < typename T >
  inline constexpr interval<T>& operator-=(interval<T>& x, typename interval<T>::value_type const& y)
  {
    return x = CRANBERRIES_MAKE_INTERVAL(T, x.lower() - y, x.upper() - y);
  }

  template < typename T >
  inline constexpr interval<T>& operator*=(interval<T>& x, typename interval<T>::value_type const& y)
  {
    return x = (y < interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x.upper() * y, x.lower() * y)
      : CRANBERRIES_MAKE_INTERVAL(T, x.lower() * y, x.upper() * y);
  }
  template < typename T >
  inline constexpr interval<T>& operator/=(interval<T>& x, typename interval<T>::value_type const& y)
  {
    return x = CRANBERRIES_OVERFLOW_ERROR_THROW_CONDITIONAL(y == interval_constants::zero<T>)
      : (y < interval_constants::zero<T>)
      ? CRANBERRIES_MAKE_INTERVAL(T, x.upper() / y, x.lower() / y)
      : CRANBERRIES_MAKE_INTERVAL(T, x.lower() / y, x.upper() / y);
  }


} // ! cranberries
#endif // ! CRANBRIIES_INTERVAL_LIB_ARITHMETIC_HPP

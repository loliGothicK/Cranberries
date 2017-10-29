/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBRIIES_INTERVAL_LIB_COMPARE_WEAK_ORDER_HPP
#define CRANBRIIES_INTERVAL_LIB_COMPARE_WEAK_ORDER_HPP

#include <utility>
#include <type_traits>

namespace cranberries {
  template < typename T, typename U >
  inline bool operator<(interval<T> const& a, interval<U> const& b) noexcept {
    return weak_less(b, a);
  }
  template < typename T >
  inline bool operator<(interval<T> const& a, typename interval<T>::value_type const& b) noexcept {
    return weak_less(a, b);
  }
  template < typename T >
  inline bool operator<(typename interval<T>::value_type const& a, interval<T> const& b) noexcept {
    return weak_less(a, b);
  }
  template < typename T, typename U >
  inline bool operator<=(interval<T> const& a, interval<U> const& b) noexcept {
    return weak_less_or_equal(b, a);
  }
  template < typename T >
  inline bool operator<=(interval<T> const& a, typename interval<T>::value_type const& b) noexcept {
    return weak_less_or_equal(a, b);
  }
  template < typename T >
  inline bool operator<=(typename interval<T>::value_type const& a, interval<T> const& b) noexcept {
    return weak_less_or_equal(a, b);
  }
  template < typename T, typename U >
  inline bool operator>(interval<T> const& a, interval<U> const& b) noexcept {
    return weak_greater(b, a);
  }
  template < typename T >
  inline bool operator>(interval<T> const& a, typename interval<T>::value_type const& b) noexcept {
    return weak_greater(a, b);
  }
  template < typename T >
  inline bool operator>(typename interval<T>::value_type const& a, interval<T> const& b) noexcept {
    return weak_greater(a, b);
  }
  template < typename T, typename U >
  inline bool operator>=(interval<T> const& a, interval<U> const& b) noexcept {
    return weak_greater_or_equal(b, a);
  }
  template < typename T >
  inline bool operator>=(interval<T> const& a, typename interval<T>::value_type const& b) noexcept {
    return weak_greater_or_equal(a, b);
  }
  template < typename T >
  inline bool operator>=(typename interval<T>::value_type const& a, interval<T> const& b) noexcept {
    return weak_greater_or_equal(a, b);
  }
  template < typename T, typename U >
  inline bool operator==(interval<T> const& a, interval<U> const& b) noexcept {
    return weak_equal(b, a);
  }
  template < typename T >
  inline bool operator==(interval<T> const& a, typename interval<T>::value_type const& b) noexcept {
    return weak_equal(a, b);
  }
  template < typename T >
  inline bool operator==(typename interval<T>::value_type const& a, interval<T> const& b) noexcept {
    return weak_equal(a, b);
  }
  template < typename T, typename U >
  inline bool operator!=(interval<T> const& a, interval<U> const& b) noexcept {
    return !weak_equal(b, a);
  }
  template < typename T >
  inline bool operator!=(interval<T> const& a, typename interval<T>::value_type const& b) noexcept {
    return !weak_equal(a, b);
  }
  template < typename T >
  inline bool operator!=(typename interval<T>::value_type const& a, interval<T> const& b) noexcept {
    return !weak_equal(a, b);
  }


} // ! cranberries

#endif
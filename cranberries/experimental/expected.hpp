#ifndef CRANBERRIES_EXPECTED_HPP
#define CRANBERRIES_EXPECTED_HPP
#include <type_traits>
#include <utility>
#include <string>
#include "../optinal.hpp"

namespace cranberries {
namespace exprimental {

  template < class T, class E > class expected;

  template < class E >
  struct unexpected {
    E error;
    template < class T >
    operator expected<T, E>() { return { *this }; }
  };

  template < class E >
  unexpected<std::decay_t<E>> make_unexpected(E&& e) { return { std::forward<E>(e) }; }

  template < class T, class E = std::string >
  class expected {
    optional<T> opt_;
    E error_;
  public:
    expected() = default;
    expected(const T& value) : opt_{ value }, error_{} {}
    template < class Err, std::enable_if_t<std::is_constructible<E, Err>::value,std::nullptr_t> = nullptr >
    expected(const unexpected<Err>& e) : opt_{ nullopt }, error_{ e.error } {}
    expected(const expected&) = default;
    expected(expected&&) = default;
    expected& operator=(const expected&) = default;
    expected& operator=(expected&&) = default;

    T value() const { return opt_ ? *opt_ : throw error_; }
    T value_or(const T& v = T{}) const { return opt_.value_or(v); }
    E error() const { return error_; }
    bool valid() const { return opt_; }

    template < class F >
    expected<std::result_of_t<F(T)>, E> map(F&& f) const {
      if (opt_) return std::forward<F>(f)(*opt_);
      else return make_unexpected(error_);
    }
    template < class F >
    expected<std::result_of_t<F(T)>, E> map_or(const std::result_of_t<F(T)>& v, F&& f) const {
      return opt_ ? std::forward<F>(f)(*opt_) : v;
    }
    template < class F, class G >
    expected<std::common_type_t<std::result_of_t<F(E)>, std::result_of_t<G(T)>>, E>
      map_or_else(F&& f, G&& g) const {
      if (opt_) return std::forward<G>(g)(*opt_);
      else return std::forward<F>(f)(error_);
    }
    template < class U >
    expected<std::decay_t<U>, E> and_if(U&& v) const {
      if (opt_) return v; else return make_unexpected(error_);
    }
    template < class F >
    decltype(auto) then(F&& f) const {
      return std::forward<F>(f)(*this);
    }

  };

}
}
#endif
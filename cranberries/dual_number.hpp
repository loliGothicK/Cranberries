/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_MATH_DUAL_NUMBER_HPP
#define CRANBERRIES_MATH_DUAL_NUMBER_HPP

#include <iostream>
#include <cmath>
#include <type_traits>
#include "type_traits.hpp"
#include "utility/utility.hpp"
#include "./common/exception.hpp"

namespace cranberries
{
  namespace cranberries_magic {
    class dual_number_base{};
    
    template < typename T >
    constexpr T sqrt(T s){
      T x = s / 2.0;
      T prev = 0.0;

      while(x != prev){
        x = (x + s / x) / 2.0;
      }
      return x;
    }
  }

  template < typename T >
  constexpr bool is_dual_v = std::is_base_of<cranberries_magic::dual_number_base,T>::value;

  template < typename T >
  struct is_dual {
    static constexpr bool value = is_dual_v<T>;
  };

  template < typename T >
  static constexpr T ln2 = static_cast<T>(0.693147180559945309417232121458176568075500134360255254120);

  template < typename T >
  static constexpr T ln2_inv = static_cast<T>(1) / ln2<T>;

  template  < typename T >
  static constexpr T  ln10 = static_cast<T>(2.302585092994045684017991454684364207601101488628772976033);

  template < typename T >
  static constexpr T ln10_inv = static_cast<T>(1) / ln10<T>;

  template < typename T >
  static constexpr T pi = static_cast<T>(3.1415926535237);

  template < typename T >
  static constexpr T pi_root = cranberries::cranberries_magic::sqrt(pi<T>);

  template < typename T >
  static constexpr T pi_root_inv = static_cast<T>(1) / pi_root<T>;

  template < typename T = double >
  class dual_number
    : private cranberries_magic::dual_number_base
  {
  public:
    using value_type = T;
    constexpr dual_number( value_type const& a, value_type b ) noexcept : real_{ a }, basis_{ b } {}
    constexpr dual_number( value_type const& a ) noexcept : real_{ a }, basis_{} {}
    constexpr dual_number() : dual_number{ T{}, T{} } {}
    constexpr dual_number(dual_number const& a) : dual_number{ a.real_, a.basis_ } {}
    constexpr dual_number(dual_number&& a) : dual_number{ a.real_, a.basis_ } {}

    constexpr auto deconstruct() { return std::make_tuple( real_, basis_ ); }

    constexpr dual_number operator -() const noexcept {
      return dual_number( -real_, -basis_ );
    }

    friend std::ostream& operator << ( std::ostream& out, dual_number const& rhs ) noexcept {
      return out << '[' << rhs.real_ << ", " << rhs.basis_ << ']';
    }

    constexpr value_type& real() noexcept { return real_; }
    constexpr value_type& basis() noexcept { return basis_; }
    constexpr const value_type&  real() const noexcept { return real_; }
    constexpr const value_type&  basis() const noexcept { return basis_; }

  private:
    value_type real_;
    value_type basis_;
  };

    

  template < typename T >
  constexpr dual_number<T> make_dual( T&& x, T&& y ) noexcept {
    return { std::forward<T>( x ), std::forward<T>( y )};
  }
    
  template < typename T >
  constexpr dual_number<T> make_dual( T&& x ) noexcept {
    return { std::forward<T>(x) };
  }


  // four arithmetic operators
    
  template < typename L, typename R >
  dual_number<L>& operator += ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
    lhs.real() += rhs.real();
    lhs.basis() += rhs.basis();
    return lhs;
  }
    
  template < typename L, typename R >
  dual_number<L>& operator -= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
    lhs.real() -= rhs.real();
    lhs.basis() -= rhs.basis();
    return lhs;
  }
    
  template < typename L, typename R >
  dual_number<L>& operator *= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
    lhs.basis() = ( lhs.basis() * rhs.real() + lhs.real() * rhs.basis() );
    lhs.real() *= rhs.real();
    return lhs;
  }
    
  template < typename L, typename R >
  dual_number<L>& operator /= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
    lhs.basis() = ( lhs.basis() * rhs.real() - lhs.real() * rhs.basis() ) / ( rhs.real() * rhs.real() );
    lhs.real() /= rhs.real();
    return lhs;
  }
    
  template < typename T >
  auto& operator += ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs += dual_number<T>{ rhs };
  }
    
  template < typename T >
  auto& operator -= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs -= dual_number<T>{ rhs };
  }
    
  template < typename T >
  auto& operator *= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs *= dual_number<T>{ rhs };
  }
    
  template < typename T >
  auto& operator /= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs /= dual_number<T>{ rhs };
  }
    
  template < typename L, typename R, typename T = decltype( std::declval<L>() + std::declval<R>() ) >
  dual_number<T> operator + ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
    return { lhs.real() + rhs.real(), lhs.basis() + rhs.basis() };
  }
    
  template < typename T >
  auto operator + ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs + dual_number<T>( rhs );
  }
    
  template < typename T >
  auto operator + ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) noexcept {
    return dual_number<T>( lhs ) + rhs;
  }
    
  template < typename L, typename R, typename T = decltype( std::declval<L>() - std::declval<R>() ) >
  dual_number<T> operator - ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
    return { lhs.real() - rhs.real(), lhs.basis() - rhs.basis() };
  }
    
  template < typename T >
  auto operator - ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs - dual_number<T>( rhs );
  }
    
  template < typename T >
  auto operator - ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) noexcept {
    return dual_number<T>( lhs ) - rhs;
  }
    
  template < typename L, typename R, typename T = decltype( std::declval<L>() * std::declval<R>() ) >
  dual_number<T> operator * ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
    return { lhs.real()*rhs.real(), lhs.basis() * rhs.real() + lhs.real() * rhs.basis() };
  }
    
  template < typename T >
  auto operator * ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
    return lhs * dual_number<T>( rhs );
  }
    
  template < typename T >
  auto operator * ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) noexcept {
    return dual_number<T>( lhs ) * rhs;
  }
    
  template < typename L, typename R, typename T = decltype( std::declval<L>() / std::declval<R>() ) >
  dual_number<T> operator / ( dual_number<L> const& lhs, dual_number<R> const& rhs ) {
    return { lhs.real() / rhs.real(), ( lhs.basis() * rhs.real() - lhs.real() * rhs.basis() ) / ( rhs.real() * rhs.real() ) };
  }
    
  template < typename T >
  auto operator / ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) {
    return lhs / dual_number<T>( rhs );
  }
    
  template < typename T >
  auto operator / ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) {
    return dual_number<T>( lhs ) / rhs;
  }

  // math functoins

    
  template <typename T>
  dual_number<T> sin( dual_number<T> const& x ) noexcept {
    using std::sin; using std::cos;
    return { sin( x.real() ), cos( x.real() ) * x.basis() };
  }
    
  template <typename T>
  dual_number<T> cos( dual_number<T> const& x ) noexcept {
    using std::sin; using std::cos;
    return { cos( x.real() ), -sin( x.real() ) * x.basis() };
  }
    
  template <typename T>
  dual_number<T> tan( dual_number<T> const& x ) {
    using std::tan; using std::cos;
    auto c = cos( x.real() );
    return { tan( x.real() ), x.basis() / ( c * c ) };
  }
    
  template <typename T>
  dual_number<T> sec( dual_number<T> const& x ) {
    using std::cos; using std::tan;
    return { 1.0 / cos( x.real() ), x.basis()*tan( x.real() ) / cos( x.real() ) };
  }
    
  template <typename T>
  dual_number<T> csc( dual_number<T> const& x ) {
    using std::sin; using std::tan;
    return { 1.0 / sin( x.real() ), -x.basis() / ( tan( x.real() ) * sin( x.real() ) ) };
  }
    
  template <typename T>
  dual_number<T> cot( dual_number<T> const& x ) {
    using std::sin; using std::tan;
    auto c = sin( x.real() );
    return { 1.0 / tan( x.real() ), -x.basis() / ( c*c ) };
  }
    
  template <typename T>
  dual_number<T> asin( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < -1.0 || 1.0 < x.real() );
    using std::asin; using std::sqrt;
    return { asin( x.real() ), x.basis() / sqrt( 1.0 - x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> acos( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < -1.0 || 1.0 < x.real() );
    using std::acos; using std::sqrt;
    return { acos( x.real() ), -x.basis() / sqrt( 1.0 - x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> atan( dual_number<T> const& x ) noexcept {
    using std::atan;
    return { atan( x.real() ), x.basis() / ( 1.0 + x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> asec( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
    using std::acos; using std::sqrt; using std::pow;
    return { acos( 1.0 / x.real() ), x.basis() / sqrt( pow( x.real(), 4 ) - x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> acsc( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
    using std::asin; using std::sqrt; using std::pow;
    return { asin( 1.0 / x.real() ), -x.basis() / sqrt( pow( x.real(), 4 ) - x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> acot( dual_number<T> const& x ) {
    using std::atan; using std::sqrt;
    return { atan( 1.0 / x.real() ), -x.basis() / ( 1.0 + x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> sinh( dual_number<T> const& x ) {
    using std::sinh; using std::cosh;
    return { sinh( x.real() ), cosh( x.real() )*x.basis() };
  }
    
  template <typename T>
  dual_number<T> cosh( dual_number<T> const& x ) {
    using std::sinh; using std::cosh;
    return { cosh( x.real() ), sinh( x.real() )*x.basis() };
  }
    
  template <typename T>
  dual_number<T> tanh( dual_number<T> const& x ) {
    using std::tanh;
    auto a = tanh( x.real() );
    return { a, ( 1.0 - a*a )*x.basis() };
  }
    
  template <typename T>
  dual_number<T> sech( dual_number<T> const& x ) {
    using std::cosh; using std::tanh;
    auto c = 1.0 / cosh( x.real() );
    return { c, -tanh( x.real() ) * c * x.basis() };
  }
    
  template <typename T>
  dual_number<T> csch( dual_number<T> const& x ) {
    using std::tanh; using std::sinh;
    auto c = 1.0 / sinh( x.real() );
    return { c, -c*x.basis()/ tanh( x.real() ) };
  }
    
  template <typename T>
  dual_number<T> coth( dual_number<T> const& x ) {
    using std::sinh; using std::tanh;
    auto c = sinh( x.real() );
    return { 1.0 / tanh( x.real() ), -x.basis() / ( c*c ) };
  }
    
  template <typename T>
  dual_number<T> asinh( dual_number<T> const& x ) {
    using std::asinh; using std::sqrt;
    return { asinh( x.real() ), x.basis() / sqrt( 1.0 + x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> acosh( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() <= 1.0 );
    using std::acosh; using std::sqrt;
    return { acosh( x.real() ), x.basis() / sqrt( x.real()*x.real() - 1.0 ) };
  }
    
  template <typename T>
  dual_number<T> atanh( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < -1.0 || 1.0 < x.real() );
    using std::atanh;
    return { atanh( x.real() ), x.basis() / ( 1.0 - x.real()*x.real() ) };
  }
    
  template <typename T>
  dual_number<T> asech( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( 0.0 < x.real() && x.real() <= 1.0 ) );
    using std::acosh; using std::pow; using std::sqrt;
    return { acosh( 1.0 / x.real() ), -x.basis() / sqrt( pow( x.real(), 2 )*( 1.0 - pow( x.real(), 2 ) ) ) };
  }
    
  template <typename T>
  dual_number<T> acsch( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
    using std::asinh; using std::pow; using std::sqrt;
    return { asinh( 1.0 / x.real() ), -x.basis() / sqrt( pow( x.real(), 2 )*( 1.0 + pow( x.real(), 2 ) ) ) };
  }
    
  template <typename T>
  dual_number<T> acoth( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
    using std::atanh;
    return { atanh( 1.0 / x.real() ), x.basis() / ( 1.0 - x.real()*x.real() ) };
  }

    
  template < typename T >
  dual_number<T> abs( dual_number<T> const& x ) noexcept {
    using std::abs;
    return { abs( x.real() ), abs( x.basis() ) };
  }

    
  template <typename T>
  dual_number<T> log( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 );
    using std::log;
    return { log( x.real() ), x.basis() / x.real() };
  }


  template <typename T>
  dual_number<T> log1p(dual_number<T> const& x) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF(x.real() < 0.0);
    using std::log1p;
    return { log1p(x.real()), x.basis() / (x.real()+1) };
  }


  template <typename T>
  dual_number<T> log2(dual_number<T> const& x) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF(x.real() < 0.0);
    using std::log2;
    return { log2(x.real()), x.basis() / x.real() * ln2_inv<T> };
  }

    
  template <typename T>
  dual_number<T> log10(dual_number<T> const& x) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF(x.real() < 0.0);
    using std::log10;
    return { log10(x.real()), x.basis() / x.real() * ln10_inv<T> };
  }


  template <typename T>
  dual_number<T> exp( dual_number<T> const& x ) noexcept {
    using std::exp;
    auto a = exp( x.real() );
    return { a, a * x.basis() };
  }

    
  template <typename T>
  dual_number<T> expm1(dual_number<T> const& x) noexcept {
    using std::exp;
    using std::expm1;
    return { expm1(x.real()), exp(x.real()) * x.basis() };
  }


  template <typename T>
  dual_number<T> exp2(dual_number<T> const& x) noexcept {
    using std::exp2;
    auto a = exp2(x.real());
    return { a, a * x.basis() * ln2<T> };
  }

    
  template < typename T >
  dual_number<T> pow( dual_number<T> const& x, double n ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 && std::fmod( n,1 ) != 0 );
    using std::pow;
    return { pow( x.real(), n ), n * pow( x.real(), n - 1 )*x.basis() };
  }

    
  template < typename T >
  dual_number<T> sqrt( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 );
    using std::sqrt;
    return { sqrt( x.real() ), 0.5 * x.basis() / sqrt( x.real() ) };
  }

    
  template < typename T >
  dual_number<T> cbrt( dual_number<T> const& x ) {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 );
    using std::cbrt; using std::pow;
    return { cbrt( x.real() ), x.basis() / ( cbrt( pow( x.real(), 2 ) ) * 3.0 ) };
  }

  template < typename T >
  dual_number<T> erf(dual_number<T> const& x) noexcept {
    using std::erf; using std::exp;
    return { erf(x.real()), x.basis() * 2 * exp(-x.real()*x.real()) * pi_root_inv<T> };
  }

  template < typename T >
  dual_number<T> erfc(dual_number<T> const& x) noexcept {
    using std::erfc; using std::exp;
    return { erfc(x.real()), -x.basis() * 2 * exp(-x.real()*x.real()) * pi_root_inv<T> };
  }
  
  template < typename DualNumber >
  std::enable_if_t<is_dual_v<DualNumber>>
  operator<<
  (
    generate_tuple_t<remove_cvr_t<element_type_of_t<DualNumber>>&, 2>&& t,
    DualNumber&& d
  ) noexcept {
    t = std::forward_as_tuple(d.real(), d.basis());
  }

  template < typename T >
  void
  operator>>
  (
    generate_tuple_t<remove_cvr_t<T>, 2>&& t,
    dual_number<T>& d    
  ) noexcept {
    std::forward_as_tuple( d.real(), d.basis() ) = t;
  }


namespace literals {
inline namespace dual_number_literal {
  cranberries::dual_number<double> operator "" _dual(long double x) { return cranberries::make_dual(static_cast<double>(x), 0.0); }
}}

} // ! namespace cranberries

#endif

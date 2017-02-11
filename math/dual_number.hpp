#ifndef CRANBERRIES_MATH_DUAL_NUMBER_HPP
#define CRANBERRIES_MATH_DUAL_NUMBER_HPP

#include <iostream>
#include <cmath>
#include <type_traits>
#include "../except/exception.hpp"

namespace cranberries
{
  namespace cranberries_magic {
    class dual_number_base{};
  }

  template < typename T >
  constexpr bool is_dual_v = std::is_base_of<cranberries_magic::dual_number_base,T>::value;

  template < typename T >
  struct is_dual {
    static constexpr bool value = is_dual_v<T>;
  };

  namespace math
  {
    template < typename T = double >
    class dual_number
      : private cranberries_magic::dual_number_base
    {
    public:
      using value_type = T;
      constexpr dual_number( value_type const& a, value_type b ) noexcept : real_{ a }, basis_{ b } {}
      constexpr dual_number( value_type const& a ) noexcept : real_{ a }, basis_{} {}
      dual_number() = default;
      dual_number(dual_number const&) = default;
      dual_number(dual_number&&) = default;

      dual_number operator -() const noexcept {
        return dual_number( -real_, -basis_ );
      }

      friend std::ostream& operator << ( std::ostream& out, dual_number const& rhs ) noexcept {
        return out << '[' << rhs.real_ << ", " << rhs.basis_ << ']';
      }

      value_type& real() noexcept { return real_; }
      value_type& basis() noexcept { return basis_; }
      value_type const& real() const noexcept { return real_; }
      value_type const& basis() const noexcept { return basis_; }

    private:
      value_type real_;
      value_type basis_;
    };

    /* Passed */
    template < typename T >
    inline constexpr dual_number<T> make_dual( T&& x, T&& y ) noexcept {
      return { std::forward<T>( x ), std::forward<T>( y )};
    }
    /* Passed */
    template < typename T >
    inline constexpr dual_number<T> make_dual( T&& x ) noexcept {
      return { std::forward<T>(x) };
    }



    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator += ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.real() += rhs.real();
      lhs.basis() += rhs.basis();
      return lhs;
    }
    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator -= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.real() -= rhs.real();
      lhs.basis() -= rhs.basis();
      return lhs;
    }
    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator *= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.basis() = ( lhs.basis() * rhs.real() + lhs.real() * rhs.basis() );
      lhs.real() *= rhs.real();
      return lhs;
    }
    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator /= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.basis() = ( lhs.basis() * rhs.real() - lhs.real() * rhs.basis() ) / ( rhs.real() * rhs.real() );
      lhs.real() /= rhs.real();
      return lhs;
    }
    /* Passed */
    template < typename T >
    inline auto& operator += ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs += dual_number<T>{ rhs };
    }
    /* Passed */
    template < typename T >
    inline auto& operator -= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs -= dual_number<T>{ rhs };
    }
    /* Passed */
    template < typename T >
    inline auto& operator *= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs *= dual_number<T>{ rhs };
    }
    /* Passed */
    template < typename T >
    inline auto& operator /= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs /= dual_number<T>{ rhs };
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() + std::declval<R>() ) >
    inline dual_number<T> operator + ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return { lhs.real() + rhs.real(), lhs.basis() + rhs.basis() };
    }
    /* Passed */
    template < typename T >
    inline auto operator + ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs + dual_number<T>( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator + ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) noexcept {
      return dual_number<T>( lhs ) + rhs;
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() - std::declval<R>() ) >
    inline dual_number<T> operator - ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return { lhs.real() - rhs.real(), lhs.basis() - rhs.basis() };
    }
    /* Passed */
    template < typename T >
    inline auto operator - ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs - dual_number<T>( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator - ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) noexcept {
      return dual_number<T>( lhs ) - rhs;
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() * std::declval<R>() ) >
    inline dual_number<T> operator * ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return { lhs.real()*rhs.real(), lhs.basis() * rhs.real() + lhs.real() * rhs.basis() };
    }
    /* Passed */
    template < typename T >
    inline auto operator * ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs * dual_number<T>( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator * ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) noexcept {
      return dual_number<T>( lhs ) * rhs;
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() / std::declval<R>() ) >
    inline dual_number<T> operator / ( dual_number<L> const& lhs, dual_number<R> const& rhs ) {
      return { lhs.real() / rhs.real(), ( lhs.basis() * rhs.real() - lhs.real() * rhs.basis() ) / ( rhs.real() * rhs.real() ) };
    }
    /* Passed */
    template < typename T >
    inline auto operator / ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) {
      return lhs / dual_number<T>( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator / ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) {
      return dual_number<T>( lhs ) / rhs;
    }

    /* Passed */
    template <typename T>
    inline dual_number<T> sin( dual_number<T> const& x ) noexcept {
      using std::sin; using std::cos;
      return { sin( x.real() ), cos( x.real() ) * x.basis() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> cos( dual_number<T> const& x ) noexcept {
      using std::sin; using std::cos;
      return { cos( x.real() ), -sin( x.real() ) * x.basis() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> tan( dual_number<T> const& x ) {
      using std::tan; using std::cos;
      auto c = cos( x.real() );
      return { tan( x.real() ), x.basis() / ( c * c ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> sec( dual_number<T> const& x ) {
      using std::cos; using std::tan;
      return { 1.0 / cos( x.real() ), x.basis()*tan( x.real() ) / cos( x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> csc( dual_number<T> const& x ) {
      using std::sin; using std::tan;
      return { 1.0 / sin( x.real() ), -x.basis() / ( tan( x.real() ) * sin( x.real() ) ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> cot( dual_number<T> const& x ) {
      using std::sin; using std::tan;
      auto c = sin( x.real() );
      return { 1.0 / tan( x.real() ), -x.basis() / ( c*c ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asin( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < -1.0 || 1.0 < x.real() );
      using std::asin; using std::sqrt;
      return { asin( x.real() ), x.basis() / sqrt( 1.0 - x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acos( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < -1.0 || 1.0 < x.real() );
      using std::acos; using std::sqrt;
      return { acos( x.real() ), -x.basis() / sqrt( 1.0 - x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> atan( dual_number<T> const& x ) noexcept {
      using std::atan;
      return { atan( x.real() ), x.basis() / ( 1.0 + x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asec( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
      using std::acos; using std::sqrt; using std::pow;
      return { acos( 1.0 / x.real() ), x.basis() / sqrt( pow( x.real(), 4 ) - x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acsc( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
      using std::asin; using std::sqrt; using std::pow;
      return { asin( 1.0 / x.real() ), -x.basis() / sqrt( pow( x.real(), 4 ) - x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acot( dual_number<T> const& x ) {
      using std::atan; using std::sqrt;
      return { atan( 1.0 / x.real() ), -x.basis() / ( 1.0 + x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> sinh( dual_number<T> const& x ) {
      using std::sinh; using std::cosh;
      return { sinh( x.real() ), cosh( x.real() )*x.basis() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> cosh( dual_number<T> const& x ) {
      using std::sinh; using std::cosh;
      return { cosh( x.real() ), sinh( x.real() )*x.basis() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> tanh( dual_number<T> const& x ) {
      using std::tanh;
      auto a = tanh( x.real() );
      return { a, ( 1.0 - a*a )*x.basis() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> sech( dual_number<T> const& x ) {
      using std::cosh; using std::tanh;
      auto c = 1.0 / cosh( x.real() );
      return { c, -tanh( x.real() ) * c * x.basis() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> csch( dual_number<T> const& x ) {
      using std::tanh; using std::sinh;
      auto c = 1.0 / sinh( x.real() );
      return { c, -c*x.basis()/ tanh( x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> coth( dual_number<T> const& x ) {
      using std::sinh; using std::tanh;
      auto c = sinh( x.real() );
      return { 1.0 / tanh( x.real() ), -x.basis() / ( c*c ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asinh( dual_number<T> const& x ) {
      using std::asinh; using std::sqrt;
      return { asinh( x.real() ), x.basis() / sqrt( 1.0 + x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acosh( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() <= 1.0 );
      using std::acosh; using std::sqrt;
      return { acosh( x.real() ), x.basis() / sqrt( x.real()*x.real() - 1.0 ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> atanh( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < -1.0 || 1.0 < x.real() );
      using std::atanh;
      return { atanh( x.real() ), x.basis() / ( 1.0 - x.real()*x.real() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asech( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( 0.0 < x.real() && x.real() <= 1.0 ) );
      using std::acosh; using std::pow; using std::sqrt;
      return { acosh( 1.0 / x.real() ), -x.basis() / sqrt( pow( x.real(), 2 )*( 1.0 - pow( x.real(), 2 ) ) ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acsch( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
      using std::asinh; using std::pow; using std::sqrt;
      return { asinh( 1.0 / x.real() ), -x.basis() / sqrt( pow( x.real(), 2 )*( 1.0 + pow( x.real(), 2 ) ) ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acoth( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.real() < -1.0 || 1.0 < x.real() ) );
      using std::atanh;
      return { atanh( 1.0 / x.real() ), x.basis() / ( 1.0 - x.real()*x.real() ) };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> abs( dual_number<T> const& x ) {
      using std::abs;
      return { abs( x.real() ), abs( x.basis() ) };
    }

    /* Passed */
    template <typename T>
    inline dual_number<T> log( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 );
      using std::log;
      return { log( x.real() ), x.basis() / x.real() };
    }

    /* Passed */
    template <typename T>
    inline dual_number<T> exp( dual_number<T> const& x ) noexcept {
      using std::exp;
      auto a = exp( x.real() );
      return { a, a * x.basis() };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> pow( dual_number<T> const& x, double n ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 && std::fmod( n,1 ) != 0 );
      using std::pow;
      return { pow( x.real(), n ), n * pow( x.real(), n - 1 )*x.basis() };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> sqrt( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 );
      using std::sqrt;
      return { sqrt( x.real() ), 0.5 * x.basis() / sqrt( x.real() ) };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> cbrt( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.real() < 0.0 );
      using std::cbrt; using std::pow;
      return { cbrt( x.real() ), x.basis() / ( cbrt( pow( x.real(), 2 ) ) * 3.0 ) };
    }

    namespace literals {
      inline cranberries::math::dual_number<double> operator "" _dual(long double x) { return cranberries::math::make_dual(static_cast<double>(x), 0.0); }
    }
  }
}

#endif
#ifndef DUAL_NUMBER_HPP
#define DUAL_NUMBER_HPP

#include <iostream>
#include <cmath>
#include <type_traits>
#include "../../exception/exception.hpp"

namespace Cranberries
{
  namespace math
  {
    struct is_dual_impl
    {
      template < typename T >
      static auto check( T v )->decltype( v.is_dual(), std::true_type{} );
      template < typename T >
      static auto check( ... )->std::false_type;
    };

    template < typename T >
    struct is_dual : public decltype( is_dual_impl::check<T>( std::declval<T>() ) ) {};

    template < typename T = double >
    class dual_number
    {
    public:
      void is_dual() const {}
      using value_type = T;

      inline constexpr dual_number( value_type const& a, value_type const& b = T{} ) noexcept : a_{ a }, b_{ b } {}

      inline dual_number operator - () const noexcept {
        return dual_number( -a_, -b_ );
      }

      inline friend std::ostream& operator << ( std::ostream& out, dual_number const& rhs ) noexcept {
        return out << '[' << rhs.a_ << ',' << rhs.b_ << ']';
      }

      inline T& a() noexcept { return a_; }
      inline T& b() noexcept { return b_; }
      inline T const& a() const noexcept { return a_; }
      inline T const& b() const noexcept { return b_; }

    private:
      T a_;
      T b_;
    };
    /* Passed */
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    auto make_dual( L&& x, R&& y ) noexcept {
      return dual_number<T>{ std::forward<T>( x ), std::forward<T>( y )};
    }
    /* Passed */
    template < typename T >
    auto make_dual( T x ) noexcept {
      return dual_number<T>{ x, T{} };
    }



    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator += ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.a() += rhs.a();
      lhs.b() += rhs.b();
      return lhs;
    }
    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator -= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.a() -= rhs.a();
      lhs.b() -= rhs.b();
      return lhs;
    }
    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator *= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.b() = ( lhs.b() * rhs.a() + lhs.a() * rhs.b() );
      lhs.a() *= rhs.a();
      return lhs;
    }
    /* Passed */
    template < typename L, typename R >
    inline dual_number<L>& operator /= ( dual_number<L>& lhs, dual_number<R> const& rhs ) noexcept {
      lhs.b() = ( lhs.b() * rhs.a() - lhs.a() * rhs.b() ) / ( rhs.a() * rhs.a() );
      lhs.a() /= rhs.a();
      return lhs;
    }
    /* Passed */
    template < typename T >
    inline dual_number<T>& operator += ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs += make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline dual_number<T>& operator -= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs -= make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline dual_number<T>& operator *= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs *= make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline dual_number<T>& operator /= ( dual_number<T>& lhs, typename dual_number<T>::value_type const& rhs ) noexcept {
      return lhs /= make_dual( rhs );
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() + std::declval<R>() ) >
    inline dual_number<T> operator + ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return dual_number<T>{ lhs.a() + rhs.a(), lhs.b() + rhs.b() };
    }
    /* Passed */
    template < typename T >
    inline auto operator + ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) {
      return lhs + make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator + ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) {
      return make_dual( lhs ) + rhs;
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() - std::declval<R>() ) >
    inline dual_number<T> operator - ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return dual_number<T>{ lhs.a() - rhs.a(), lhs.b() - rhs.b() };
    }
    /* Passed */
    template < typename T >
    inline auto operator - ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) {
      return lhs - make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator - ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) {
      return make_dual( lhs ) - rhs;
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() * std::declval<R>() ) >
    inline dual_number<T> operator * ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return dual_number<T>{ lhs.a()*rhs.a(), lhs.b() * rhs.a() + lhs.a() * rhs.b() };
    }
    /* Passed */
    template < typename T >
    inline auto operator * ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) {
      return lhs * make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator * ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) {
      return make_dual( lhs ) * rhs;
    }
    /* Passed */
    template < typename L, typename R, typename T = decltype( std::declval<L>() / std::declval<R>() ) >
    inline dual_number<T> operator / ( dual_number<L> const& lhs, dual_number<R> const& rhs ) noexcept {
      return dual_number<T>{ lhs.a() / rhs.a(), ( lhs.b() * rhs.a() - lhs.a() * rhs.b() ) / ( rhs.a() * rhs.a() ) };
    }
    /* Passed */
    template < typename T >
    inline auto operator / ( dual_number<T> const& lhs, typename dual_number<T>::value_type const& rhs ) {
      return lhs / make_dual( rhs );
    }
    /* Passed */
    template < typename T >
    inline auto operator / ( typename dual_number<T>::value_type const& lhs, dual_number<T> const& rhs ) {
      return make_dual( lhs ) / rhs;
    }

    /* Passed */
    template <typename T>
    inline dual_number<T> sin( dual_number<T> const& x ) noexcept {
      using std::sin; using std::cos;
      return dual_number<T>{ sin( x.a() ), cos( x.a() ) * x.b() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> cos( dual_number<T> const& x ) noexcept {
      using std::sin; using std::cos;
      return dual_number<T>{ cos( x.a() ), -sin( x.a() ) * x.b() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> tan( dual_number<T> const& x ) {
      using std::tan; using std::cos;
      auto c = cos( x.a() );
      return dual_number<T>{ tan( x.a() ), x.b() / ( c * c ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> sec( dual_number<T> const& x ) {
      using std::cos; using std::tan;
      return dual_number<T>{ 1.0 / cos( x.a() ), x.b()*tan( x.a() ) / cos( x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> csc( dual_number<T> const& x ) {
      using std::sin; using std::tan;
      return dual_number<T>{ 1.0 / sin( x.a() ), -x.b() / ( tan( x.a() ) * sin( x.a() ) ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> cot( dual_number<T> const& x ) {
      using std::sin; using std::tan;
      auto c = sin( x.a() );
      return dual_number<T>{ 1.0 / tan( x.a() ), -x.b() / ( c*c ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asin( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < -1.0 || 1.0 < x.a() );
      using std::asin; using std::sqrt;
      return dual_number<T>{ asin( x.a() ), x.b() / sqrt( 1.0 - x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acos( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < -1.0 || 1.0 < x.a() );
      using std::acos; using std::sqrt;
      return dual_number<T>{ acos( x.a() ), -x.b() / sqrt( 1.0 - x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> atan( dual_number<T> const& x ) {
      using std::atan;
      return dual_number<T>{ atan( x.a() ), x.b() / ( 1.0 + x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asec( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.a() < -1.0 || 1.0 < x.a() ) );
      using std::acos; using std::sqrt; using std::pow;
      return dual_number<T>{ acos( 1.0 / x.a() ), x.b() / sqrt( pow( x.a(), 4 ) - x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acsc( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.a() < -1.0 || 1.0 < x.a() ) );
      using std::asin; using std::sqrt; using std::pow;
      return dual_number<T>{ asin( 1.0 / x.a() ), -x.b() / sqrt( pow( x.a(), 4 ) - x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acot( dual_number<T> const& x ) {
      using std::atan; using std::sqrt;
      return dual_number<T>{ atan( 1.0 / x.a() ), -x.b() / ( 1.0 + x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> sinh( dual_number<T> const& x ) {
      using std::sinh; using std::cosh;
      return dual_number<T>{ sinh( x.a() ), cosh( x.a() )*x.b() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> cosh( dual_number<T> const& x ) {
      using std::sinh; using std::cosh;
      return dual_number<T>{ cosh( x.a() ), sinh( x.a() )*x.b() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> tanh( dual_number<T> const& x ) {
      using std::tanh;
      auto a = tanh( x.a() );
      return dual_number<T>{ a, ( 1.0 - a*a )*x.b() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> sech( dual_number<T> const& x ) {
      using std::cosh; using std::tanh;
      auto c = 1.0 / cosh( x.a() );
      return dual_number<T>{ c, -tanh( x.a() ) * c * x.b() };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> csch( dual_number<T> const& x ) {
      using std::tanh; using std::sinh;
      auto c = 1.0 / sinh( x.a() );
      return dual_number<T>{ c, -c*x.b()/ tanh( x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> coth( dual_number<T> const& x ) {
      using std::sinh; using std::tanh;
      auto c = sinh( x.a() );
      return dual_number<T>{ 1.0 / tanh( x.a() ), -x.b() / ( c*c ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asinh( dual_number<T> const& x ) {
      using std::asinh; using std::sqrt;
      return dual_number<T>{ asinh( x.a() ), x.b() / sqrt( 1.0 + x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acosh( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < 1.0 );
      using std::acosh; using std::sqrt;
      return dual_number<T>{ acosh( x.a() ), x.b() / sqrt( x.a()*x.a() - 1.0 ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> atanh( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < -1.0 || 1.0 < x.a() );
      using std::atanh;
      return dual_number<T>{ atanh( x.a() ), x.b() / ( 1.0 - x.a()*x.a() ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> asech( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( 0.0 < x.a() && x.a() <= 1.0 ) );
      using std::acosh; using std::pow; using std::sqrt;
      return dual_number<T>{ acosh( 1.0 / x.a() ), -x.b() / sqrt( pow( x.a(), 2 )*( 1.0 - pow( x.a(), 2 ) ) ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acsch( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.a() < -1.0 || 1.0 < x.a() ) );
      using std::asinh; using std::pow; using std::sqrt;
      return dual_number<T>{ asinh( 1.0 / x.a() ), -x.b() / sqrt( pow( x.a(), 2 )*( 1.0 + pow( x.a(), 2 ) ) ) };
    }
    /* Passed */
    template <typename T>
    inline dual_number<T> acoth( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( x.a() < -1.0 || 1.0 < x.a() ) );
      using std::atanh;
      return dual_number<T>{ atanh( 1.0 / x.a() ), x.b() / ( 1.0 - x.a()*x.a() ) };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> abs( dual_number<T> const& x ) {
      using std::abs;
      return dual_number<T>{ abs( x.a() ), abs( x.b() ) };
    }

    /* Passed */
    template <typename T>
    inline dual_number<T> log( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < 0.0 );
      using std::log;
      return dual_number<T>{ log( x.a() ), x.b() / x.a() };
    }

    /* Passed */
    template <typename T>
    inline dual_number<T> exp( dual_number<T> const& x ) noexcept {
      using std::exp;
      auto a = exp( x.a() );
      return dual_number<T>{ a, a * x.b() };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> pow( dual_number<T> const& x, double n ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < 0.0 && std::fmod( n,1 ) != 0 );
      using std::pow;
      return dual_number<T>{ pow( x.a(), n ), n * pow( x.a(), n - 1 )*x.b() };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> sqrt( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < 0.0 );
      using std::sqrt;
      return dual_number<T>{ sqrt( x.a() ), 0.5 * x.b() / sqrt( x.a() ) };
    }

    /* Passed */
    template < typename T >
    inline dual_number<T> cbrt( dual_number<T> const& x ) {
      CRANBERRIES_DOMAIN_ERROR_THROW_IF( x.a() < 0.0 );
      using std::cbrt; using std::pow;
      return dual_number<T>{ cbrt( x.a() ), x.b() / ( cbrt( pow( x.a(), 2 ) ) * 3.0 ) };
    }
  }
}
inline Cranberries::math::dual_number<long double> operator "" _dual( long double x ) { return Cranberries::math::make_dual( x, 0.0L ); }

#endif
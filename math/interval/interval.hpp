#ifndef CRANBERRIES_INTERVAL_HPP
#define CRANBERRIES_INTERVAL_HPP
#include "exception.hpp"
#include "math_impl.hpp"
#include <limits>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <cfenv>
#include "set_round.hpp"

#define CRANBERRIES_ASSERT(expr) static_assert((expr), #expr)

using std::nextafter;

namespace Cranberries
{
  /*  version  */
  enum class version_tag
  {
    v1_0_0 = 0x01000000,
    v1_0_1,
    v1_0_2,
    v1_0_3,
    v1_0_4,
    v1_0_5,
    v1_0_6,
    v1_0_7,
    v1_0_8,
    v1_1_0 = 0x01010000,
    v1_1_1,
    v1_2_0 = 0x01020000,
    v2_0_0 = 0x02000000,
    v2_0_1,
    v2_0_2,
    v2_1_0 = 0x02010000,
    v3_0_0 = 0x03000000,
    v3_0_1,
    v3_1_0 = 0x03010000,
    v4_0_0 = 0x04000000,
    now = v4_0_0,
  };

  template < typename T >
  struct Val;

  namespace detail
  {
    // meta functions definition
    template < typename T >
    constexpr bool is_arithmetic_v = std::is_arithmetic<T>::value;

    // is_expr Implementation
    struct is_expr_impl
    {
      template< typename T >
      static auto check( T v ) -> decltype( v.is_expr(), std::true_type() );
      template < typename T >
      static std::false_type check( ... );
    };

    // is_expr body
    template < typename T >
    class is_expr : public decltype( is_expr_impl::check<T>( std::declval<T>() ) ) {};

    template < typename T >
    constexpr bool is_expr_v = is_expr<T>::value;

    // si_interval Implementation
    struct is_interval_impl
    {
      template< typename T >
      static auto check( T v ) -> decltype( v.is_interval(), std::true_type() );
      template < typename T >
      static std::false_type check( ... );
    };

    // is_interval body
    template < typename T >
    class is_interval : public decltype( is_interval_impl::check<T>( std::declval<T>() ) ) {};

    template < typename T >
    constexpr bool is_interval_v = is_interval<T>::value;


    template < typename T, std::size_t Label = std::is_class<T>::value >
    struct Expr
    {
      typedef Val<T> Ref;
    };

    template <typename T >
    struct Expr<T, true >
    {
      typedef T Ref;
    };

    template < typename T >
    using Expr_ref = typename Expr<T>::Ref;
  }
  // for Expression Template
  // bind non-interval value
  template < typename T >
  struct Val
  {
    T value;
  public:
    void is_expr() {};
    typedef T type;
    Val() : value{ T{} } {}
    Val( T&& v ) : value{ std::move( v ) } {}

    T operator[]( int )
    {
      return value;
    }
  };

  template < typename T >
  Val<T> val( T&& x )
  {
    return Val<T>( std::forward<T>( x ) );
  }


  template < typename T >
  class interval;

  namespace constants
  {
    template < typename T >
    struct Traits {
      template < typename U >
      static T invoke( U x ) { return static_cast<T>(x); }
    };

    template < typename T >
    struct Traits<interval<T>> {
      template < typename U >
      static interval<T> invoke( U x ) {
        DOWNWARD_POLICY;
        auto l = static_cast<T>( x );
        UPWARD_POLICY;
        auto r = static_cast<T>( x );
        return interval<T>{l, r};
      }
    };

    template < typename T >
    constexpr auto pi()
    {
      return Traits<T>::invoke( 3.141592653589793238462643383279L );
    }

    template < typename T >
    constexpr auto e()
    {
      return Traits<T>::invoke( 2.718281828459045235360287471352L );
    }

    template < typename T >
    constexpr auto ln2()
    {
      return Traits<T>::invoke( 0.693147180559945309417232121458L );
    }

    template < typename T >
    constexpr auto ln10()
    {
      return Traits<T>::invoke( 2.302585092994045684017991454684L );
    }

    template < typename T >
    constexpr auto golden()
    {
      return Traits<T>::invoke( 1.61803398874989484820458683436563811L );
    }

    template < typename T >
    constexpr auto c_0()
    {
      return Traits<T>::invoke( 299792458.0L );
    }

    template < typename T >
    constexpr auto mu_0()
    {
      return Traits<T>::invoke( 4.0L*pi<long double>()*1E-7L );
    }

    template < typename T >
    constexpr auto epsilon_0()
    {
      return Traits<T>::invoke( 8.854187817L );
    }

    template < typename T >
    constexpr auto z_0()
    {
      return Traits<T>::invoke( 376.730313461L );
    }

    template < typename T >
    constexpr auto g()
    {
      return Traits<T>::invoke( 6.67408E-11L );
    }

    template < typename T >
    constexpr auto planck()
    {
      return Traits<T>::invoke( 6.626070040E-34L );
    }

    template < typename T >
    constexpr auto dirac()
    {
      return Traits<T>::invoke( 1.054571800E-34L );
    }

    template < typename T >
    constexpr auto electron()
    {
      return Traits<T>::invoke( 1.6021766208E-19L );
    }

    template < typename T >
    constexpr auto phi_0()
    {
      return Traits<T>::invoke( 2.067833831E-15L );
    }

    template < typename T >
    constexpr auto g_0()
    {
      return Traits<T>::invoke( 7.7480917310E-5L );
    }

    template < typename T >
    constexpr auto r_0()
    {
      return Traits<T>::invoke( 12906.4037278L );
    }

    template < typename T >
    constexpr auto max()
    {
      return std::numeric_limits<T>::max();
    }

    template < typename T >
    constexpr auto zero()
    {
      return Traits<T>::invoke( 0.0L );
    }

    template < typename T >
    constexpr auto one()
    {
      return Traits<T>::invoke( 1.0L );
    }
  }
  template < typename T >
  T upward( T&& x )
  {
    return nextafter( std::forward<T>( x ), constants::max<T>() );
  }

  template < typename T >
  T downward( T&& x )
  {
    return nextafter( std::forward<T>( x ), -constants::max<T>() );
  }

  //---------------------//
  /*   Ordering Symbol   */
  //---------------------//

  enum class order { Total, Weak, Partial, Interval };

  /*  3 Values of Ordering  */
  enum class partial_ordering { less, unordered, greater };
  enum class weak_ordering { less, equivalent, greater };
  enum class total_ordering { less, equal, greater };
  enum class interval_ordering { less, unordered, greater };

  template <class T1, class T2, class T3, class Func >
  class Expr3;

  template < class L, class Op, class R >
  class Expr2;

  template < class A, class Func >
  class Expr1;

  /*  Advanced Interval Ordering  */
  enum class interval_relation
  {
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

  //----------------------------------//
  /*                  */
  /*     Class Declaration      */
  /*                  */
  /*    Interval Class      */
  /*    (Body )           */
  /*                  */
  //----------------------------------//

  template < typename T = double >
  class interval
  {
    CRANBERRIES_ASSERT(detail::is_arithmetic_v<T>);
  public:

    /*  ctor  */
    interval() noexcept : pImpl{ std::make_unique<Impl>() } {};
    interval( T&&, T&& );
    interval( T const&, T const& );

    /*  copy ctor  */
    interval( interval const& ) noexcept;
    /*  move ctor  */
    interval( interval&& ) noexcept;
    /*  Copy Assignment Op  */
    interval& operator=( interval const& ) noexcept;
    /*  Move assignment Op  */
    interval& operator=( interval&& ) noexcept;

    interval& operator=( std::initializer_list<T> );

    /*  increment/decrement Op  */
    interval operator ++() noexcept;
    interval operator ++( int ) noexcept;
    interval operator --() noexcept;
    interval operator --( int ) noexcept;

    /*  numeric functions  */

    interval inverse() const;

    /*  torigonometric functions  */
    interval sin() const noexcept;
    interval cos() const noexcept;
    interval tan() const;
    interval sec() const;
    interval csc() const;
    interval cot() const;
    interval asin() const;
    interval acos() const;
    interval atan() const noexcept;
    interval asec() const;
    interval acsc() const;
    interval acot() const;

    /*  hyperbolic functions  */
    interval sinh() const;
    interval cosh() const;
    interval tanh() const noexcept;
    interval sech() const noexcept;
    interval csch() const;
    interval coth() const;
    interval asinh() const noexcept;
    interval acosh() const;
    interval atanh() const;
    interval asech() const;
    interval acsch() const;
    interval acoth() const;

    /*  expconstants::onential & logarithmic functions  */
    interval log() const;
    interval log10() const;
    interval log2() const;
    interval log1p() const;
    interval exp() const;
    interval exp2() const;
    interval expm1() const;

    /*  power math & absolute functions  */
    interval pow( long double ) const;
    interval sqrt() const;
    interval cbrt() const  noexcept;
    interval abs() const;

    /*  error functions  */
    interval erf() const noexcept;
    interval erfc() const noexcept;

    /*  interval functions  */
    constexpr T mid() const noexcept;
    constexpr T wid() const noexcept;
    constexpr T width() const noexcept;
    constexpr T rad() const noexcept;
    constexpr T middle() const noexcept;
    constexpr T norm() const noexcept;
    constexpr T mig() const noexcept;
    constexpr T mag() const noexcept;
    constexpr bool is_singleton() const noexcept;

    /*  accessors  */
    // getter
    constexpr T lower() const noexcept;
    constexpr T upper() const noexcept;
    // resetter
    void reset_upper( T&& );
    void reset_lower( T&& );
    void reset_upper( T const& );
    void reset_lower( T const& );
    void reset( T&&, T&& );
    void reset( T const&, T const& );
    // swap
    inline void swap( interval& ) noexcept;

    /*  Advanced Relational Op  */
    interval_relation relational( interval const& ) const noexcept;
    bool is_contain( interval const& ) const noexcept;
    bool is_contain( T const& x ) const noexcept;
    bool is_part_of( interval const& ) const noexcept;
    void is_interval() noexcept {};


    template <class T1, class T2, class T3, class Func >
    inline interval& operator=( Expr3<T1, T2, T3, Func> x ) noexcept
    {
      ( *this ) = x.eval();
      return *this;
    }

    template <class L, class Op, class R >
    inline interval& operator=( Expr2<L, Op, R> x ) noexcept
    {
      ( *this ) = x.eval();
      return *this;
    }
    template <class A, class Func >
    inline interval& operator=( Expr1<A, Func> x ) noexcept
    {
      ( *this ) = x.eval();
      return *this;
    }

    template <class T1, class T2, class T3, class Func >
    inline interval& operator+=( Expr3<T1, T2, T3, Func> x ) noexcept
    {
      ( *this ) = *this + x.eval();
      return *this;
    }

    template <class A, class Func >
    inline interval& operator+=( Expr1<A, Func> x ) noexcept
    {
      *this = *this + x.eval();
      return *this;
    }

    template <class L, class Op, class R >
    inline interval& operator+=( Expr2<L, Op, R> x ) noexcept
    {
      *this = *this + x.eval();
      return *this;
    }

    inline interval const& operator[]( const std::size_t ) const& noexcept
    {
      return *this;
    }

    inline interval& operator[]( const std::size_t ) & noexcept
    {
      return *this;
    }

    inline interval&& operator[]( const std::size_t ) && noexcept
    {
      return *this;
    }

    /*  For printf  */
    inline const char* c_str() const noexcept;
    /*  Member value type  */
    typedef T value_type;
  private:
    /*  Inner Class  */
    class Impl;
    /*  Keep lower & upper bound Value   */
    std::unique_ptr<Impl> pImpl;
  };

  //----------------------------------//
  /*                  */
  /*     class declaration      */
  /*                  */
  /*   interval Implementation class  */
  /*   ( internal class of interval)  */
  /*                  */
  //----------------------------------//

  template < typename T >
  class interval<T>::Impl
  {
  public:
    /*  Set Initial Value */
    inline void do_internal_work( T const& low, T const& up ) noexcept
    {
      lower_bound = low;
      upper_bound = up;
    }
    /*  Set Initial Value */
    inline void do_internal_work( T&& low, T&& up ) noexcept
    {
      lower_bound = std::move(low);
      upper_bound = std::move(up);
    }
    /*  Increment/decrement Op Implementation */
    inline const Impl operator ++() noexcept;
    inline const Impl operator ++( int ) noexcept;
    inline const Impl operator --() noexcept;
    inline const Impl operator --( int ) noexcept;

    /*  getter Implementation  */
    inline constexpr T lower() const noexcept;
    inline constexpr T upper() const noexcept;

    /*  resetter Implementation */
    void reset_lower( T&& x )
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( x > upper_bound, "lower_bound must be less than upper_bound!" )
        lower_bound = std::move( x );
    }
    void reset_lower( T const& x )
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( x > upper_bound, "lower_bound must be less than upper_bound!" )
        lower_bound = x;
    }

    void reset_upper( T&& x )
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( x < lower_bound, "upper_bound must be greater than lower_bound!" )
        upper_bound = std::move( x );
    }
    void reset_upper( T const& x )
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( x < lower_bound, "upper_bound must be greater than lower_bound!" )
        upper_bound = x;
    }

    void reset( T&& x, T&& y )
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( y < x, "upper_bound must be greater than lower_bound!" )
        lower_bound = std::move( x );
      upper_bound = std::move( y );
    }
    void reset( T const& x, T const& y )
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( y < x, "upper_bound must be greater than lower_bound!" )
        lower_bound = x;
      upper_bound = y;
    }

  private:
    /*  Lower Bound  */
    T lower_bound = {};

    /*  Upper Bound  */
    T upper_bound = {};
  };


  //------------------------------//
  //                //
  /*  Interval Member Functions   */
  //                //
  //------------------------------//

  /*  Interval Increment/Decrement Operator Definition  */

  /*  Interval Increment Operator  */

  //  prefix increment
  template < typename T >
  inline interval<T> interval<T>::operator ++() noexcept
  {
    ++( *pImpl );
    CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_IF( this->upper() == constants::max<T>() || this->lower() == -constants::max<T>(), "overflow occured." )
    return *this;
  }

  //  postfix increment
  template < typename T >
  inline interval<T> interval<T>::operator ++( int ) noexcept
  {
    auto tmp( *this );
    ++( *pImpl );
    CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_IF( this->upper() == constants::max<T>() || this->lower() == -constants::max<T>(), "overflow occured." )
    return tmp;
  }

  /*  Interval Increment Operator  */

  //  prefix decrement
  template < typename T >
  inline interval<T> interval<T>::operator --() noexcept
  {
    --( *pImpl );
    CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_IF( this->upper() == constants::max<T>() || this->lower() == -constants::max<T>(), "overflow occured." )
    return *this;
  }

  //  postfix decrement
  template < typename T >
  inline interval<T> interval<T>::operator --( int ) noexcept
  {
    auto tmp( *this );
    --( *pImpl );
    CRANBERRIES_OVER_FLOW_THROW_WITH_MSG_IF( this->upper() == constants::max<T>() || this->lower() == -constants::max<T>(), "overflow occured." )
    return tmp;
  }

  //--------------------------------------------//
  //                      //
  /*  Interval Numeric Function Definition  */
  //                      //
  //--------------------------------------------//


  template < typename T >
  inline interval<T> interval<T>::inverse() const
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( pImpl->lower() <= 0.0 && 0.0 <= pImpl->upper() )
  else
    {
      DOWNWARD_POLICY;
      auto&& l = 1.0 / pImpl->upper();
      UPWARD_POLICY;
      auto&& r = 1.0 / pImpl->lower();
      return interval<T>{ l, r };
    }
  }

  /*  interval cosine  */

  template < typename T >
  inline interval<T> interval<T>::cos() const noexcept
  {
    using math_impl::cos_down;
    using math_impl::cos_up;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();

    if ( b - a >= static_cast<T>( 2.0L ) * constants::pi<T>() )
    {
      return interval<T>{ -constants::one<T>(), constants::one<T>() };
    }
    /*  base point reset  */
    auto pi = Cranberries::constants::pi<T>();
    DOWNWARD_POLICY;
    auto base1 = std::ceil( a / ( 2. * pi ) );
    auto base2 = std::ceil( a / ( 2. * pi ) - 0.5 );
    auto x1 = a / ( 2. * pi );
    auto y1 = a / ( 2. * pi ) - 0.5;
    DOWNWARD_POLICY;
    auto x2 = b / ( 2. * pi );
    auto y2 = b / ( 2. * pi ) - 0.5;
    /*  checking phase  */
    if ( x1 < base1 && base1 < x2 )
    {
      if ( y1 < base2 && base2 < y2 )
      {
        return interval<T>{ -constants::one<T>(), constants::one<T>() };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = static_cast<T>( cos_down( a ) );
        auto&& r = static_cast<T>( cos_down( b ) );
        return interval<T>{ std::fmin( l, r ), constants::one<T>() };
      }
    }
    else if ( y1 < base2 && base2 < y2 )
    {
      UPWARD_POLICY;
      auto&& l = static_cast<T>( cos_up( a ) );
      auto&& r = static_cast<T>( cos_up( b ) );
      return interval<T>{ -constants::one<T>(), std::fmax( l, r ) };
    }
    else
    {
      if ( cos_up( a ) < cos_up( b ) )
      {
        DOWNWARD_POLICY;
        auto&& l = static_cast<T>( cos_down( a ) );
        UPWARD_POLICY;
        auto&& r = static_cast<T>( cos_up( b ) );
        return interval<T>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = static_cast<T>( cos_down( b ) );
        UPWARD_POLICY;
        auto&& r = static_cast<T>( cos_up( a ) );
        return interval<T>{ l, r };
      }
    }
  }

  /*  interval sine  */

  template < typename T >
  inline interval<T> interval<T>::sin() const noexcept
  {
    using math_impl::sin_up;
    using math_impl::sin_down;
    auto a = pImpl->lower();
    auto b = pImpl->upper();

    if ( b - a >= static_cast<T>( 2.0L ) * constants::pi<T>() )
    {
      return interval<T>{ -constants::one<T>(), constants::one<T>() };
    }
    /*  base point reset  */
    auto pi = Cranberries::constants::pi<T>();
    DOWNWARD_POLICY;
    auto base1 = std::ceil( a / ( 2. * pi ) - 0.25 );
    auto base2 = std::ceil( a / ( 2. * pi ) - 0.75 );
    auto x1 = a / ( 2. * pi ) - 0.25;
    auto y1 = a / ( 2. * pi ) - 0.75;
    DOWNWARD_POLICY;
    auto x2 = b / ( 2. * pi ) - 0.25;
    auto y2 = b / ( 2. * pi ) - 0.75;
    /*  checking phase  */
    if ( x1 < base1 && base1 < x2 )
    {
      if ( y1 < base2 && base2 < y2 )
      {
        return interval<T>{ -constants::one<T>(), constants::one<T>() };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = static_cast<T>( sin_down( a ) );
        auto&& r = static_cast<T>( sin_down( b ) );
        return interval<T>{ std::fmin( l, r ), constants::one<T>() };
      }
    }
    else if ( y1 < base2 && base2 < y2 )
    {
      UPWARD_POLICY;
      auto&& l = static_cast<T>( sin_up( a ) );
      auto&& r = static_cast<T>( sin_up( b ) );
      return interval<T>{ -constants::one<T>(), std::fmax( l, r ) };
    }
    else
    {
      if ( sin_up( a ) < sin_up( b ) )
      {
        DOWNWARD_POLICY;
        auto&& l = static_cast<T>( sin_down( a ) );
        UPWARD_POLICY;
        auto&& r = static_cast<T>( sin_up( b ) );
        return interval<T>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = static_cast<T>( sin_down( b ) );
        UPWARD_POLICY;
        auto&& r = static_cast<T>( sin_up( a ) );
        return interval<T>{ l, r };
      }
    }
  }

  /*  interval tangent  */

  template < typename T >
  inline interval<T> interval<T>::tan() const
  {
    using math_impl::sin_up; using math_impl::sin_down;
    using math_impl::cos_up; using math_impl::cos_down;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();

    CRANBERRIES_OVER_FLOW_THROW_IF( b - a >= constants::pi<T>() )
    /*  checking phase  */
    CRANBERRIES_OVER_FLOW_THROW_IF( static_cast<int>( b * 2 / constants::pi<T>() ) - static_cast<int>( a * 2 / constants::pi<T>() ) != 0 )
    CRANBERRIES_RANGE_ERROR_THROW_ELSE_IF( downward( sin_down( a ) / cos_up( a ) ) == -constants::max<T>() || downward( sin_up( b ) / cos_down( b ) ) == constants::max<T>() )
    else
    {
      DOWNWARD_POLICY;
      auto&& l = sin_down( a ) / cos_up( a );
      UPWARD_POLICY;
      auto&& r = sin_up( b ) / cos_down( b );
      return interval<T>{ l, r };
    }
  }

  template < typename T >
  interval<T> interval<T>::sec() const
  {
    return this->cos().inverse();
  }

  template < typename T >
  interval<T> interval<T>::csc() const
  {
    return this->sin().inverse();
  }

  template < typename T >
  interval<T> interval<T>::cot() const
  {
    return this->tan().inverse();
  }


  /*  interval arc cosine  */

  template < typename T >
  inline interval<T> interval<T>::acos() const
  {
    using std::acos;
    auto a = pImpl->lower();
    auto b = pImpl->upper();

    CRANBERRIES_DOMAIN_ERROR_THROW_IF( a < -constants::one<T>() || constants::one<T>() < b )
    else
    {
      DOWNWARD_POLICY;
      auto&& l = acos( b );
      UPWARD_POLICY;
      auto&& r = acos( a );
      return interval<T>{ l, r };
    }
  }

  /*  interval arc sine  */

  template < typename T >
  inline interval<T> interval<T>::asin() const
  {
    auto a = pImpl->lower();
    auto b = pImpl->upper();

    CRANBERRIES_DOMAIN_ERROR_THROW_IF( a < -constants::one<T>() || constants::one<T>() < b )
    else
    {
      using std::asin;
      DOWNWARD_POLICY;
      auto&& l = asin( a );
      UPWARD_POLICY;
      auto&& r = asin( b );
      return interval<T>{ l, r };
    }
  }

  /*  interval arc tangent  */

  template < typename T >
  inline interval<T> interval<T>::atan() const noexcept
  {
    using std::atan;
    DOWNWARD_POLICY;
    auto&& l = atan( pImpl->lower() );
    UPWARD_POLICY;
    auto&& r = atan( pImpl->upper() );
    return interval<T>{ l, r };
  }

  template < typename T >
  interval<T> interval<T>::asec() const
  {
    using std::acos;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( pImpl->upper() < -1.0 || 1.0 < pImpl->lower() ) )
    DOWNWARD_POLICY;
    auto&& l = acos( 1.0 / pImpl->lower() );
    UPWARD_POLICY;
    auto&& r = acos( 1.0 / pImpl->upper() );
    return interval<T>{ l, r };
  }

  template < typename T >
  interval<T> interval<T>::acsc() const
  {
    using std::asin;
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( pImpl->upper() < -1.0 || 1.0 < pImpl->lower() ) )
    DOWNWARD_POLICY;
    auto&& l = asin( 1.0 / pImpl->upper() );
    UPWARD_POLICY;
    auto&& r = asin( 1.0 / pImpl->lower() );
    return interval<T>{ l, r };
  }

  template < typename T >
  interval<T> interval<T>::acot() const
  {
    using std::atan;
    using std::abs;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();
    if ( abs( a )<abs( b ) )
    {
      DOWNWARD_POLICY;
      auto&& l = atan( 1.0 / b );
      UPWARD_POLICY;
      auto&& r = atan( 1.0 / a );
      return interval<T>{ l, r };
    }
    else
    {
      DOWNWARD_POLICY;
      auto&& l = atan( 1.0 / a );
      UPWARD_POLICY;
      auto&& r = atan( 1.0 / b );
      return interval<T>{ l, r };
    }
  }

  /*  interval hyperbolic cosine  */

  template < typename T >
  inline interval<T> interval<T>::cosh() const
  {
    using std::cosh;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();

    CRANBERRIES_RANGE_ERROR_THROW_IF( upward( cosh( a ) ) == constants::max<T>() || upward( cosh( b ) ) == constants::max<T>() )
    else if ( b < constants::zero<T>() )
    {
      DOWNWARD_POLICY;
      auto&& l = cosh( b );
      UPWARD_POLICY;
      auto&& r = cosh( a );
      return interval<T>{ l, r };
    }
    else if ( constants::zero<T>() < a )
    {
      DOWNWARD_POLICY;
      auto&& l = cosh( a );
      UPWARD_POLICY;
      auto&& r = cosh( b );
      return interval<T>{ l, r };
    }
    else
    {
      UPWARD_POLICY;
      auto&& l = cosh( a );
      auto&& r = cosh( b );
      return interval<T>{ constants::one<T>(), std::fmax( l, r ) };
    }
}

  /*  interval hyperbolic sine  */

  template < typename T >
  inline interval<T> interval<T>::sinh() const
  {
    using std::sinh;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();

    CRANBERRIES_RANGE_ERROR_THROW_IF( a == -constants::max<T>() || b == constants::max<T>() )
    else
    {
      DOWNWARD_POLICY;
      auto&& l = sinh( a );
      UPWARD_POLICY;
      auto&& r = sinh( b );
      return interval<T>{ l, r };
    }
  }

  /*  interval hyperbolic tangent  */

  template < typename T >
  inline interval<T> interval<T>::tanh() const noexcept
  {
    using std::tanh;
    DOWNWARD_POLICY;
    auto&& l = tanh( pImpl->lower() );
    UPWARD_POLICY;
    auto&& r = tanh( pImpl->upper() );
    return interval<T>{ l, r };
  }

  template < typename T >
  inline interval<T> interval<T>::sech() const noexcept
  {
    return this->cosh().inverse();
  }

  template < typename T >
  inline interval<T> interval<T>::csch() const
  {
    return this->sinh().inverse();
  }

  template < typename T >
  inline interval<T> interval<T>::coth() const
  {
    return this->tanh().inverse();
  }

  /*  interval arc hyperbolic cosine  */

  template < typename T >
  inline interval<T> interval<T>::acosh() const
  {
    using std::acosh;
    auto a = pImpl->lower();
    auto b = pImpl->upper();

    CRANBERRIES_DOMAIN_ERROR_THROW_IF( a < constants::one<T>() )
    else
    {
      DOWNWARD_POLICY;
      auto&& l = acosh( a );
      UPWARD_POLICY;
      auto&& r = acosh( b );
      return interval<T>{ l, r };
    }
  }

  /*  interval arc hyperbolic sine  */

  template < typename T >
  inline interval<T> interval<T>::asinh() const noexcept
  {
    using std::asinh;
    DOWNWARD_POLICY;
    auto&& l = asinh( pImpl->lower() );
    UPWARD_POLICY;
    auto&& r = asinh( pImpl->upper() );
    return interval<T>{ l, r };
  }

  /*  interval arc hyperbolic tangent  */

  template < typename T >
  inline interval<T> interval<T>::atanh() const
  {
    using std::atanh;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( a < -constants::one<T>() || constants::one<T>() < b )
    else
    {
      DOWNWARD_POLICY;
      auto&& l = atanh( a );
      UPWARD_POLICY;
      auto&& r = atanh( b );
      return interval<T>{ l, r };
    }
  }

  template < typename T >
  inline interval<T> interval<T>::asech() const
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( 0.0 < pImpl->lower() && pImpl->upper() <= 1.0 ) )
    return this->inverse().acosh();
  }

  template < typename T >
  inline interval<T> interval<T>::acsch() const
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( pImpl->upper() < -1.0 || 1.0 < pImpl->lower() ) )
    return this->inverse().asinh();
  }

  template < typename T >
  inline interval<T> interval<T>::acoth() const
  {
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( !( pImpl->upper() < -1.0 || 1.0 < pImpl->lower() ) )
    return this->inverse().atanh();
  }



  /*  interval power function  */

  template < typename T >
  inline interval<T> interval<T>::pow( long double n ) const
  {
    using math_impl::pow_up;
    using math_impl::pow_down;
    using std::pow;
    auto&& a = pImpl->lower();
    auto&& b = pImpl->upper();
    if ( n < 0 )
    {
      return this->inverse().pow( -n );
    }
    else if ( fmodl( n, 1 ) == 0 )
    {
      if ( n == 0.0 )
        return interval<T>{ constants::one<T>(), constants::one<T>() };
      else if ( a <= 0.0 && 0.0 <= b )
      {
        T l = pow_up( a, static_cast< std::size_t >(n) );
        T r = pow_up( b, static_cast< std::size_t >( n ) );
        return interval<T>{0, std::fmax( l, r )};
      }
      else
      {
        T l_min = pow_down( a, static_cast< std::size_t >( n ) );
        T r_min = pow_down( b, static_cast< std::size_t >( n ) );
        T l_max = pow_up( a, static_cast< std::size_t >( n ) );
        T r_max = pow_up( b, static_cast< std::size_t >( n ) );
        return interval<T>{ std::fmin( l_min, r_min ), std::fmax( l_max, r_max ) };
      }
    }
    CRANBERRIES_DOMAIN_ERROR_THROW_ELSE_IF( a < 0.0 )
    else if ( a <= 0.0 && 0.0 <= b )
    {
      UPWARD_POLICY;
      T l = pow( a, n );
      T r = pow( b, n );
      return interval<T>{0, std::fmax( l, r )};
    }
    else
    {
      DOWNWARD_POLICY;
      T l_min = pow( a, n );
      T r_min = pow( b, n );
      UPWARD_POLICY;
      T l_max = pow( a, n );
      T r_max = pow( b, n );
      return interval<T>{ std::fmin( l_min, r_min ), std::fmax( l_max, r_max ) };
    }
  }




  /*  interval square root function  */

  template < typename T >
  inline interval<T> interval<T>::sqrt() const
  {
    using std::sqrt;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( low < constants::zero<T>() )
    DOWNWARD_POLICY;
    auto&& l = sqrt( low );
    UPWARD_POLICY;
    auto&& r = sqrt( up );
    return interval<T>{ l, r };
  }

  template < typename T >
  inline interval<T> interval<T>::cbrt() const noexcept
  {
    using std::cbrt;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    DOWNWARD_POLICY;
    auto&& l = cbrt( low );
    UPWARD_POLICY;
    auto&& r = cbrt( up );
    return interval<T>{ l, r };
  }

  /*  interval expconstants::onential function ( base = e )  */

  template < typename T >
  inline interval<T> interval<T>::exp() const
  {
    using std::exp;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_RANGE_ERROR_THROW_IF( upward( exp( up ) ) == constants::max<T>() || downward( exp( low ) ) == -constants::max<T>() )
    DOWNWARD_POLICY;
    auto&& l = exp( low );
    UPWARD_POLICY;
    auto&& r = exp( up );
    return interval<T>{ l, r };
  }

  /*  interval expconstants::onential function ( base = 2 )  */

  template < typename T >
  inline interval<T> interval<T>::exp2() const
  {
    using std::exp2;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_RANGE_ERROR_THROW_IF( upward( exp2( up ) ) == constants::max<T>() || downward( exp2( low ) ) == -constants::max<T>() )
    DOWNWARD_POLICY;
    auto&& l = exp2( low );
    UPWARD_POLICY;
    auto&& r = exp2( up );
    return interval<T>{ l, r };
  }

  /*  interval exp( x ) - 1  */

  template < typename T >
  inline interval<T> interval<T>::expm1() const
  {
    using std::expm1;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_RANGE_ERROR_THROW_IF( upward( expm1( up ) ) == constants::max<T>() || downward( expm1( low ) ) == -constants::max<T>() )
    DOWNWARD_POLICY;
    auto&& l = expm1( low );
    UPWARD_POLICY;
    auto&& r = expm1( up );
    return interval<T>{ l, r };
  }

  /*  interval logarithmic function ( base = e ) */

  template < typename T >
  inline interval<T> interval<T>::log() const
  {
    using std::log;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( low < constants::zero<T>() )
    DOWNWARD_POLICY;
    auto&& l = log( low );
    UPWARD_POLICY;
    auto&& r = log( up );
    return interval<T>{ l, r };
  }

  /*  interval log( x ) + 1  */
  template < typename T >
  inline interval<T> interval<T>::log1p() const
  {
    using std::log1p;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( low < -constants::one<T>() )
    DOWNWARD_POLICY;
    auto&& l = log1p( low );
    UPWARD_POLICY;
    auto&& r = log1p( up );
    return interval<T>{ l, r };
  }

  /*  interval logarithmic function ( base = 10 )  */

  template < typename T >
  inline interval<T> interval<T>::log10() const
  {
    using std::log10;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( low < constants::zero<T>() )
    DOWNWARD_POLICY;
    auto&& l = log10( low );
    UPWARD_POLICY;
    auto&& r = log10( up );
    return interval<T>{ l, r };
  }

  /*  interval logarithmic function ( base = 2 )  */
  template < typename T >
  inline interval<T> interval<T>::log2() const
  {
    using std::log2;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    CRANBERRIES_DOMAIN_ERROR_THROW_IF( low < constants::zero<T>() )
    DOWNWARD_POLICY;
    auto&& l = log2( low );
    UPWARD_POLICY;
    auto&& r = log2( up );
    return interval<T>{ l, r };
  }

  /*  interval absolute funtion  */

  template < typename T >
  inline interval<T> interval<T>::abs() const
  {
    using std::abs;
    auto&& low = pImpl->lower();
    auto&& up = pImpl->upper();
    if ( low < -constants::zero<T>() && constants::zero<T>() <= up )
    {
      UPWARD_POLICY;
      auto&& l = abs( low );
      auto&& r = abs( up );
      return interval<T>{ constants::zero<T>(), std::fmax( l, r ) };
    }
    else if ( up < constants::zero<T>() )
    {
      DOWNWARD_POLICY;
      auto&& l = abs( up );
      UPWARD_POLICY;
      auto&& r = abs( low );
      return interval<T>{ l, r };
    }
    else
    {
      return interval<T>( *this );
    }
}


  /*  interval fused multiply-add function fma( interval<T>, interval<T>, interval<T>)  */
  template < typename T1, typename T2, typename T3, typename T = std::common_type_t<T1,std::common_type_t<T2,T3>> >
  inline interval<T> fma( interval<T1> const& x, interval<T2> const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;
    auto x_low = x.lower();
    auto y_low = y.lower();
    auto z_low = z.lower();
    auto x_up = x.upper();
    auto y_up = y.upper();
    auto z_up = z.upper();

    if ( x_low >= constants::zero<T1>() && y_low >= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_low, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_up, y_up, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_low >= constants::zero<T1>() && y_low < constants::zero<T2>() && y_up > constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_low, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_up, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_low >= constants::zero<T1>() && y_up <= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_low, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_up, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_low < constants::zero<T1>() && x_up > constants::zero<T1>() && y_low >= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_up, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_up, y_up, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_low < constants::zero<T1>() && x_up > constants::zero<T1>() && y_low <= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_low, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_low, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_up <= constants::zero<T1>() && y_low >= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_up, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_up, y_low, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_up <= constants::zero<T>() && y_low < constants::zero<T>() && y_up > constants::zero<T>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_up, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_low, z_up );
      return interval<T>{ l, r };
    }
    else if ( x_up <= constants::zero<T>() && y_up <= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_up, z_low );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_low, z_up );
      return interval<T>{ l, r };
    }
    else
    {
      if ( x_up*y_low < x_low*y_up )
      {
        DOWNWARD_POLICY;
        auto&& l = fma( x_up, y_low, z_low );
        UPWARD_POLICY;
        auto&& r = fma( x_low, y_up, z_up );
        return interval<T>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = fma( x_low, y_up, z_low );
        UPWARD_POLICY;
        auto&& r = fma( x_up, y_low, z_up );
        return interval<T>{ l, r };
      }
    }
}

  /*  interval fused multiply-add function fma( interval<T>, interval<T>, T)  */

  template < typename T1, typename T2, typename T3, typename T = std::common_type_t<T1,T2> >
  inline interval<T> fma( interval<T1> const& x, interval<T2> const& y, T const& z ) noexcept
  {
    using std::fma;
    auto x_low = x.lower();
    auto y_low = y.lower();
    auto y_up = y.upper();
    auto x_up = x.upper();

    if ( x_low >= constants::zero<T1>() && y_low >= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_low, z );
      UPWARD_POLICY;
      auto&& r = fma( x_up, y_up, z );
      return interval<T>{ l, r };
    }
    else if ( x_low >= constants::zero<T1>() && y_low < constants::zero<T2>() && y_up > constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_low, z );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_up, z );
      return interval<T>{ l, r };
    }
    else if ( x_low >= constants::zero<T1>() && y_up <= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_low, z );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_up, z );
      return interval<T>{ l, r };
    }
    else if ( x_low < constants::zero<T1>() && x_up > constants::zero<T1>() && y_low >= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_up, z );
      UPWARD_POLICY;
      auto&& r = fma( x_up, y_up, z );
      return interval<T>{ l, r };
    }
    else if ( x_low < constants::zero<T1>() && x_up > constants::zero<T1>() && y_low <= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_low, z );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_low, z );
      return interval<T>{ l, r };
    }
    else if ( x_up <= constants::zero<T1>() && y_low >= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_up, z );
      UPWARD_POLICY;
      auto&& r = fma( x_up, y_low, z );
      return interval<T>{ l, r };
    }
    else if ( x_up <= constants::zero<T>() && y_low < constants::zero<T>() && y_up > constants::zero<T>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_low, y_up, z );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_low, z );
      return interval<T>{ l, r };
    }
    else if ( x_up <= constants::zero<T>() && y_up <= constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x_up, y_up, z );
      UPWARD_POLICY;
      auto&& r = fma( x_low, y_low, z );
      return interval<T>{ l, r };
    }
    else
    {
      if ( x_up*y_low < x_low*y_up )
      {
        DOWNWARD_POLICY;
        auto&& l = fma( x_up, y_low, z );
        UPWARD_POLICY;
        auto&& r = fma( x_low, y_up, z );
        return interval<T>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = fma( x_low, y_up, z );
        UPWARD_POLICY;
        auto&& r = fma( x_up, y_low, z );
        return interval<T>{ l, r };
      }
    }
}

  /*  interval fused multiply-add function fma( interval<T>, T, interval<T>)  */

  template < typename T1, typename T2, typename T3, typename T = std::common_type_t<T1,T3> >
  inline interval<T> fma( interval<T1> const& x, T const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;

    if ( y < constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x.upper(), y, z.lower() );
      UPWARD_POLICY;
      auto&& r = fma( x.lower(), y, z.upper() );
      return interval<T>{ l, r };
    }
    else
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x.lower(), y, z.lower() );
      UPWARD_POLICY;
      auto&& r = fma( x.upper(), y, z.upper() );
      return interval<T>{ l, r };
    }
}

  /*  interval fused multiply-add function fma( T, interval<T>, interval<T>)  */

  template < typename T1, typename T2, typename T3, typename T = std::common_type_t<T2,T3> >
  inline interval<T> fma( T const& x, interval<T2> const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;

    if ( x < constants::zero<T1>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x, y.upper(), z.lower() );
      UPWARD_POLICY;
      auto&& r = fma( x, y.lower(), z.upper() );
      return interval<T>{ l, r };
    }
    else
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x, y.lower(), z.lower() );
      UPWARD_POLICY;
      auto&& r = fma( x, y.upper(), z.upper() );
      return interval<T>{ l, r };
    }
}

  /*  interval fused multiply-add function fma( interval<T>, T, T)  */

  template < typename T1 >
  inline interval<T1> fma( interval<T1> const& x, typename interval<T1>::value_type const& y, typename interval<T1>::value_type const& z ) noexcept
  {
    using std::fma;

    if ( y < constants::zero<T1>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x.upper(), y, z );
      UPWARD_POLICY;
      auto&& r = fma( x.lower(), y, z );
      return interval<T1>{ l, r };
    }
    else
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x.lower(), y, z );
      UPWARD_POLICY;
      auto&& r = fma( x.upper(), y, z );
      return interval<T1>{ l, r };
    }
}

  /*  interval fused multiply-add function fma( T, interval<T>, T)  */

  template < typename T2 >
  inline interval<T2> fma( typename interval<T2>::value_type const& x, interval<T2> const& y, typename interval<T2>::value_type const& z ) noexcept
  {
    using std::fma;

    if ( x < constants::zero<T2>() )
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x, y.upper(), z );
      UPWARD_POLICY;
      auto&& r = fma( x, y.lower(), z );
      return interval<T2>{ l, r };
    }
    else
    {
      DOWNWARD_POLICY;
      auto&& l = fma( x, y.lower(), z );
      UPWARD_POLICY;
      auto&& r = fma( x, y.upper(), z );
      return interval<T2>{ l, r };
    }
}

  /*  interval fused multiply-add function fma( T, T, interval<T>)  */

  template < typename T3 >
  inline interval<T3> fma( typename interval<T3>::value_type const& x, typename interval<T3>::value_type const& y, interval<T3> const& z ) noexcept
  {
    using std::fma;

    DOWNWARD_POLICY;
    auto&& l = fma( x, y, z.lower() );
    UPWARD_POLICY;
    auto&& r = fma( x, y, z.upper() );
    return interval<T3>{ l, r };
  }

  /*  interval error function  */

  template < typename T >
  inline interval<T> interval<T>::erf() const noexcept
  {
    using std::erf;
    DOWNWARD_POLICY;
    auto&& l = erf( pImpl->lower() );
    UPWARD_POLICY;
    auto&& r = erf( pImpl->upper() );
    return interval<T>{ l, r };
  }

  /*  interval complementary error function  */

  template < typename T >
  inline interval<T> interval<T>::erfc() const noexcept
  {
    using std::erfc;
    DOWNWARD_POLICY;
    auto&& l = erfc( pImpl->upper() );
    UPWARD_POLICY;
    auto&& r = erfc( pImpl->lower() );
    return interval<T>{ l, r };
  }

  /*  is_singleton  */

  template < typename T >
  inline constexpr bool interval<T>::is_singleton() const noexcept
  {
    return ( pImpl->lower() == pImpl->upper() ) ? true : false;
  }

  /*  middle point  */

  template < typename T >
  inline constexpr T interval<T>::mid() const noexcept
  {
    return ( pImpl->upper() + pImpl->lower() ) / static_cast<T>( 2.0L );
  }

  template < typename T >
  inline constexpr T interval<T>::middle() const noexcept
  {
    return ( pImpl->upper() + pImpl->lower() ) / static_cast<T>( 2.0L );
  }

  /*  width  */

  template < typename T >
  inline constexpr T interval<T>::wid() const noexcept
  {
    return pImpl->upper() - pImpl->lower();
  }

  template < typename T >
  inline constexpr T interval<T>::width() const noexcept
  {
    return pImpl->upper() - pImpl->lower();
  }

  template < typename T >
  inline constexpr T interval<T>::rad() const noexcept
  {
    return pImpl->upper() - pImpl->lower() / static_cast<T>( 2.0L );
  }

  template < typename T >
  inline constexpr T interval<T>::norm() const noexcept
  {
    using std::abs;
    auto&& l = abs( pImpl->lower() );
    auto&& r = abs( pImpl->upper() );
    return l < r ? r : l;
  }

  template < typename T >
  inline constexpr T interval<T>::mag() const noexcept
  {
    using std::abs;
    auto&& l = abs( pImpl->lower() );
    auto&& r = abs( pImpl->upper() );
    return l < r ? r : l;
  }

  template < typename T >
  inline constexpr T interval<T>::mig() const noexcept
  {
    using std::abs;
    auto&& l = abs( pImpl->lower() );
    auto&& r = abs( pImpl->upper() );
    return l < r ? l : r;
  }

  template < typename T >
  inline constexpr bool in( T&& x, interval<T> const& y ) noexcept
  {
    return y.lower() < x && x < y.upper() ? true : false;
  }

  template < typename T >
  inline constexpr bool subset( interval<T> const& x, interval<T> const& y ) noexcept
  {
    return y.lower() <= x.lower() && x.lower() <= y.upper() ? true : false;
  }

  template < typename T >
  inline constexpr bool proper_subset( interval<T> const& x, interval<T> const& y ) noexcept
  {
    return y.lower() < x.lower() && x.lower() < y.upper() ? true : false;
  }

  template < typename T >
  inline constexpr bool overlap( interval<T> const& x, interval<T> const& y ) noexcept
  {
    return x.lower() < y.upper() || y.lower() < x.upper() ? true : false;
  }

  template < typename T >
  inline constexpr interval<T> intersect( interval<T> const& x, interval<T> const& y ) noexcept
  {
    if ( x.lower() < y.lower() && y.upper() < x.upper() )
    {
      return interval<T>( y );
    }
    else if ( y.lower() < x.lower() && x.upper() < y.upper() )
    {
      return interval<T>( x );
    }
    else if ( x.lower() < y.upper() )
    {
      return interval<T>{ x.lower(), y.upper() };
    }
    else if ( y.lower() < x.upper() )
    {
      return interval<T>{ y.lower(), x.upper() };
    }
    else
    {
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG( "x and y do not overlap each other" )
    }
  }
  /*  Interval Accessors  */

  template < typename T >
  inline constexpr T interval<T>::lower() const noexcept
  {
    return pImpl->lower();
  }

  template < typename T >
  inline constexpr T interval<T>::upper() const noexcept
  {
    return pImpl->upper();
  }

  template < typename T >
  void interval<T>::reset_upper( T&& x )
  {
    pImpl->reset_upper( std::move( x ) );
  }

  template < typename T >
  void interval<T>::reset_upper( T const& x )
  {
    pImpl->reset_upper( x );
  }


  template < typename T >
  void interval<T>::reset_lower( T&& x )
  {
    pImpl->reset_lower( std::move( x ) );
  }

  template < typename T >
  void interval<T>::reset_lower( T const& x )
  {
    pImpl->reset_lower( x );
  }

  template < typename T >
  void interval<T>::reset( T&& x, T&& y )
  {
    pImpl->reset( std::move( x ), std::move( y ) );
  }
  template < typename T >
  void interval<T>::reset( T const& x, T const& y )
  {
    pImpl->reset( x, y );
  }

  template < typename T >
  void interval<T>::swap( interval& x ) noexcept
  {
    std::swap( this->pImpl, x.pImpl );
  }

  /*  Interval Relational Discriminator Function  */

  template < typename T >
  inline interval_relation interval<T>::relational( interval<T> const& x ) const noexcept
  {
    if ( total_equal( *this, x ) )
    {
      return interval_relation::equal;
    }
    else if ( pImpl->lower() == ( x.pImpl )->lower() && pImpl->upper() < ( x.pImpl )->upper() )
    {
      return interval_relation::total_less;
    }
    else if ( pImpl->lower() == ( x.pImpl )->lower() && pImpl->upper() > ( x.pImpl )->upper() )
    {
      return interval_relation::total_greater;
    }
    else if ( pImpl->lower() < ( x.pImpl )->lower() && pImpl->upper() == ( x.pImpl )->upper() )
    {
      return interval_relation::weak_less;
    }
    else if ( pImpl->lower() > ( x.pImpl )->lower() && pImpl->upper() == ( x.pImpl )->upper() )
    {
      return interval_relation::weak_greater;
    }
    else if ( pImpl->lower() < ( x.pImpl )->lower() && pImpl->upper() < ( x.pImpl )->upper() )
    {
      return interval_relation::partial_less;
    }
    else if ( pImpl->lower() > ( x.pImpl )->lower() && pImpl->upper() > ( x.pImpl )->upper() )
    {
      return interval_relation::partial_less;
    }
    else if ( pImpl->lower() <= ( x.pImpl )->lower() && pImpl->upper() >= ( x.pImpl )->upper() )
    {
      return interval_relation::contain;
    }
    else if ( pImpl->lower() >= ( x.pImpl )->lower() && pImpl->upper() <= ( x.pImpl )->upper() )
    {
      return interval_relation::part_of;
    }
    else
    {
      return interval_relation::niether;
    }
  }

  /*  Interval Relational function Definition  */

  /*
  [ Note : There are four type ordering oplicy.
  total ordering, weak ordering, partial ordering and interval ordering.
  Default ordering is total ordering. - end note ]
  */

  /*
  [ Example : Let interval<T> a,b ;
  "a < b" equals "total_less( a, b )".
  - end example ]
  */

  /*
  [ Note : Ordering Policy can switch using namespace inside a scape block.
  Three namespace are defined ( weak_ordering_policy, partial_ordering and interval_ordering_policy ).
  - end note ]
  */

  /*
  [ Example :
  auto x = hull( 1.0L, 2.0L ) ;
  auto y = hull( 1.2, 1.8 ) ;
  x < y ; // OK! default ordering_policy is total_ordering_policy.
  {
  using namespace Cranberries::weak_ordering::policy; // valid. only constants::one ordering_policy.
  y < x ; // OK! equals weal_less( y, x ) ;
  }
  {
  using namespace Cranberries::weak_ordering::policy;
  using namespace Cranberries::weak_ordering::policy; // invalid! Two or more ordering_policy.
  x < y ; // overload conflict occur.
  }
  - end example ]
  */
  template < typename T, typename U >
  struct is_available_total_less
  {
  private:
    template < typename L, typename R >
    static auto check( L x, R y ) -> decltype( total_less( x, y ), std::true_type() );
    static std::false_type check( ... );

  public:
    static bool const value = decltype( check( std::declval<T>(), std::declval<U>() ) )::value;
  };

  template < typename T, typename U >
  struct is_available_total_less_or_equal
  {
  private:
    template < typename L, typename R >
    static auto check( L x, R y ) -> decltype( total_less_or_equal( x, y ), std::true_type() );
    static std::false_type check( ... );

  public:
    static bool const value = decltype( check( std::declval<T>(), std::declval<U>() ) )::value;
  };

  template < typename T, typename U >
  struct is_available_total_greater
  {
  private:
    template < typename L, typename R >
    static auto check( L x, R y ) -> decltype( total_less_greater( x, y ), std::true_type() );
    static std::false_type check( ... );

  public:
    static bool const value = decltype( check( std::declval<T>(), std::declval<U>() ) )::value;
  };

  template < typename T, typename U >
  struct is_available_total_greater_or_equal
  {
  private:
    template < typename L, typename R >
    static auto check( L x, R y ) -> decltype( total_less_greater_or_equal( x, y ), std::true_type() );
    static std::false_type check( ... );

  public:
    static bool const value = decltype( check( std::declval<T>(), std::declval<U>() ) )::value;
  };

  template < typename T, typename U >
  struct is_available_total_equal
  {
  private:
    template < typename L, typename R >
    static auto check( L x, R y ) -> decltype( total_equal( x, y ), std::true_type() );
    static std::false_type check( ... );

  public:
    static bool const value = decltype( check( std::declval<T>(), std::declval<U>() ) )::value;
  };


  inline namespace total_ordering_policy
  {
    template < typename T, typename U >
    constexpr auto operator<( T&& x, U&& y )->decltype( total_less( x, y ) )
    {
      return total_less( std::forward<T>( x ), std::forward<U>( y ) );
    }
    template < typename T, typename U >
    constexpr auto operator<=( T&& x, U&& y )->decltype( total_less_or_equal( x, y ) )
    {
      return total_less_or_equal( std::forward<T>( x ), std::forward<U>( y ) );
    }
    template < typename T, typename U >
    constexpr auto operator>( T&& x, U&& y )->decltype( total_greater( x, y ) )
    {
      return total_greater( std::forward<T>( x ), std::forward<U>( y ) );
    }
    template < typename T, typename U >
    constexpr auto operator>=( T&& x, U&& y )->decltype( total_greater_or_equal( x, y ) )
    {
      return total_greater_or_equal( std::forward<T>( x ), std::forward<U>( y ) );
    }
    template < typename T, typename U >
    constexpr auto operator==( T&& x, U&& y )->decltype( total_equal( x, y ) )
    {
      return total_eqaul( std::forward<T>( x ), std::forward<U>( y ) );
    }
    template < typename T, typename U >
    constexpr auto operator!=( T&& x, U&& y )->decltype( total_equal( x, y ) )
    {
      return !total_equal( std::forward<T>( x ), std::forward<U>( y ) );
    }
    template < typename T, typename U, std::enable_if_t< !is_available_total_less<T, U>::value, std::nullptr_t>*& = nullptr >
    constexpr auto operator<( T&& x, U&& y ) -> bool
    {
      return x.operator<( y );
    }
    template < typename T, typename U, std::enable_if_t< !is_available_total_less_or_equal<T, U>::value, std::nullptr_t>*& = nullptr >
    constexpr auto operator<=( T&& x, U&& y )->bool
    {
      return x.operator<=( y );
    }
    template < typename T, typename U, std::enable_if_t< !is_available_total_greater<T, U>::value, std::nullptr_t>*& = nullptr >
    constexpr auto operator>( T&& x, U&& y )->bool
    {
      return x.operator>( y );
    }
    template < typename T, typename U, std::enable_if_t< !is_available_total_greater_or_equal<T, U>::value, std::nullptr_t>*& = nullptr >
    constexpr auto operator>=( T&& x, U&& y )->bool
    {
      return x.operator>=( y );
    }
    template < typename T, typename U, std::enable_if_t< !is_available_total_equal<T, U>::value, std::nullptr_t>*& = nullptr >
    constexpr auto operator==( T&& x, U&& y )->bool
    {
      return x.operator==( y );
    }
    template < typename T, typename U, std::enable_if_t< !is_available_total_equal<T, U>::value, std::nullptr_t>*& = nullptr >
    constexpr auto operator!=( T&& x, U&& y )->bool
    {
      return x.operator!=( y );
    }

  }

  namespace weak_ordering_policy
  {
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_less( x, y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( interval<L>& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( interval<L> const& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( interval<L>&& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( interval<L> const&& x, R&& y )
    {
      return weak_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( L&& x, interval<R>& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( L&& x, interval<R> const& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( L&& x, interval<R>&& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<( L&& x, interval<R> const&& y )
    {
      return weak_less( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_less_or_equal( x, y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( interval<L>& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( interval<L> const& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( interval<L>&& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( interval<L> const&& x, R&& y )
    {
      return weak_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R>& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R> const& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R>&& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R, std::enable_if_t<detail::is_interval_v<L>, std::nullptr_t>*& = nullptr >
    inline constexpr bool operator<=( L&& x, interval<R> const&& y )
    {
      return weak_less_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, R&& y )
    {
      return weak_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>&& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const&& y )
    {
      return weak_greater( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, R&& y )
    {
      return weak_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>&& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const&& y )
    {
      return weak_greater_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R>& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R> const& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R>&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, interval<R> const&& y )
    {
      return weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L>&& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( interval<L> const&& x, R&& y )
    {
      return weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R>& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R> const& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R>&& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator==( L&& x, interval<R> const&& y )
    {
      return weak_equal( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const&& y )
    {
      return !weak_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, R&& y )
    {
      return !weak_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>&& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const&& y )
    {
      return !weak_equal( std::forward<L>( x ), y );
    }
  }

  namespace partial_ordering_policy
  {
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const&& y )
    {
      return partial_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, R&& y )
    {
      return partial_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, R&& y )
    {
      return partial_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, R&& y )
    {
      return partial_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, R&& y )
    {
      return partial_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R>& y )
    {
      return partial_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R> const& y )
    {
      return partial_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R>&& y )
    {
      return partial_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R> const&& y )
    {
      return partial_less( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const&& y )
    {
      return partial_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, R&& y )
    {
      return partial_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, R&& y )
    {
      return partial_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, R&& y )
    {
      return partial_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, R&& y )
    {
      return partial_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R>& y )
    {
      return partial_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R> const& y )
    {
      return partial_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R>&& y )
    {
      return partial_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R> const&& y )
    {
      return partial_less_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const&& y )
    {
      return partial_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, R&& y )
    {
      return partial_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, R&& y )
    {
      return partial_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, R&& y )
    {
      return partial_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, R&& y )
    {
      return partial_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>& y )
    {
      return partial_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const& y )
    {
      return partial_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>&& y )
    {
      return partial_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const&& y )
    {
      return partial_greater( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const&& y )
    {
      return partial_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, R&& y )
    {
      return partial_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, R&& y )
    {
      return partial_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, R&& y )
    {
      return partial_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, R&& y )
    {
      return partial_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>& y )
    {
      return partial_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const& y )
    {
      return partial_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>&& y )
    {
      return partial_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const&& y )
    {
      return partial_greater_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const&& y )
    {
      return partial_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, R&& y )
    {
      return partial_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, R&& y )
    {
      return partial_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, R&& y )
    {
      return partial_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, R&& y )
    {
      return partial_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>& y )
    {
      return partial_unordered( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const& y )
    {
      return partial_unordered( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>&& y )
    {
      return partial_unordered( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const&& y )
    {
      return partial_unordered( std::forward<L>( x ), y );
    }
  }

  namespace interval_ordering_policy
  {
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R>&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, interval<R> const&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R>&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, interval<R> const&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R>&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, interval<R> const&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R>&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, interval<R> const&& y )
    {
      return interval_less( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>& x, R&& y )
    {
      return interval_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const& x, R&& y )
    {
      return interval_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L>&& x, R&& y )
    {
      return interval_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( interval<L> const&& x, R&& y )
    {
      return interval_less( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R>& y )
    {
      return interval_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R> const& y )
    {
      return interval_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R>&& y )
    {
      return interval_less( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<( L&& x, interval<R> const&& y )
    {
      return interval_less( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R>&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, interval<R> const&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R>&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, interval<R> const&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R>&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, interval<R> const&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R>&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, interval<R> const&& y )
    {
      return interval_less_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>& x, R&& y )
    {
      return interval_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const& x, R&& y )
    {
      return interval_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L>&& x, R&& y )
    {
      return interval_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( interval<L> const&& x, R&& y )
    {
      return interval_less_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R>& y )
    {
      return interval_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R> const& y )
    {
      return interval_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R>&& y )
    {
      return interval_less_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator<=( L&& x, interval<R> const&& y )
    {
      return interval_less_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R>&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, interval<R> const&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R>&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, interval<R> const&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R>&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, interval<R> const&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R>&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, interval<R> const&& y )
    {
      return interval_greater( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>& x, R&& y )
    {
      return interval_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const& x, R&& y )
    {
      return interval_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L>&& x, R&& y )
    {
      return interval_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( interval<L> const&& x, R&& y )
    {
      return interval_greater( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>& y )
    {
      return interval_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const& y )
    {
      return interval_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R>&& y )
    {
      return interval_greater( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>( L&& x, interval<R> const&& y )
    {
      return interval_greater( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R>&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, interval<R> const&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R>&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, interval<R> const&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R>&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, interval<R> const&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R>&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, interval<R> const&& y )
    {
      return interval_greater_or_equal( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>& x, R&& y )
    {
      return interval_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const& x, R&& y )
    {
      return interval_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L>&& x, R&& y )
    {
      return interval_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( interval<L> const&& x, R&& y )
    {
      return interval_greater_or_equal( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>& y )
    {
      return interval_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const& y )
    {
      return interval_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R>&& y )
    {
      return interval_greater_or_equal( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator>=( L&& x, interval<R> const&& y )
    {
      return interval_greater_or_equal( std::forward<L>( x ), y );
    }


    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R>&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, interval<R> const&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R>&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, interval<R> const&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R>&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, interval<R> const&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R>&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, interval<R> const&& y )
    {
      return interval_unordered( x, y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>& x, R&& y )
    {
      return interval_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const& x, R&& y )
    {
      return interval_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L>&& x, R&& y )
    {
      return interval_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( interval<L> const&& x, R&& y )
    {
      return interval_unordered( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>& y )
    {
      return interval_unordered( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const& y )
    {
      return interval_unordered( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R>&& y )
    {
      return interval_unordered( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr bool operator!=( L&& x, interval<R> const&& y )
    {
      return interval_unordered( std::forward<L>( x ), y );
    }
  }

  //-----------------------------//
  /*               */
  /*    Interval Ctor/Dtor   */
  /*               */
  //-----------------------------//


  /*  Two value Arguments Ctor  */

  template < typename T >
  interval<T>::interval( T&& low, T&& up )
    : pImpl{ std::make_unique<Impl>() }
  {
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( up < low, "upper_bound less than lower_bound!" )
    pImpl->do_internal_work( std::move( low ), std::move( up ) );
  }

  template < typename T >
  interval<T>::interval( T const& low, T const& up )
    : pImpl{ std::make_unique<Impl>() }
  {
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( up < low, "upper_bound less than lower_bound!" )
    pImpl->do_internal_work( low, up );
  }


  /*  Move Ctor  */

  template < typename T >
  interval<T>::interval( interval&& x ) noexcept
    : pImpl(std::make_unique<Impl>())
  {
    pImpl = std::move(x.pImpl);
    x.pImpl = std::make_unique<Impl>();
  }

  /*  Copy Ctor  */

  template < typename T >
  interval<T>::interval( interval const& x ) noexcept
    : pImpl{ std::make_unique<Impl>() }
  {
    *pImpl = *x.pImpl;
  }

  /*  Copy Assignment Op  */

  template < typename T >
  interval<T>& interval<T>::operator=( interval<T> const& x ) noexcept
  {
    *pImpl = *x.pImpl;
    return *this;
  }

  /*  Move Assignment Op  */

  template < typename T >
  interval<T>& interval<T>::operator=( interval<T>&& x ) noexcept
  {
    std::swap( this->pImpl, x.pImpl );
    x.pImpl = std::make_unique<Impl>();
    return *this;
  }

  template < typename T >
  interval<T>& interval<T>::operator=( std::initializer_list<T> list )
  {
    if ( list.size() == 0 )
    {
      this->reset( constants::zero<T>(), constants::zero<T>() );
    }
    else if ( list.size() == 2 )
    {
      if ( *( list.begin() ) < *( list.begin() + 1 ) )
      {
        this->reset( std::move( *( list.begin() ) ), std::move( *( list.begin() + 1 ) ) );
      }
      CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_ELSE( "arguments number is abnormal." )
    }
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_ELSE( "arguments number is abnormal." )
    return *this;
  }



  //------------------------------//
  /*                */
  /*   Impl Member Function   */
  /*                */
  //------------------------------//

  /*  Increment/Decrement Operators  */

  /*  Impl Prefix Increment Operator  */

  template < typename T >
  inline const typename interval<T>::Impl interval<T>::Impl::operator ++() noexcept
  {
    DOWNWARD_POLICY;
    ++lower_bound;
    UPWARD_POLICY;
    ++upper_bound;
    return *this;
  }

  /*  Impl Postfix Increment Operator  */

  template < typename T >
  inline const typename interval<T>::Impl interval<T>::Impl::operator ++( int ) noexcept
  {
    auto tmp( *this );
    DOWNWARD_POLICY;
    ++lower_bound;
    UPWARD_POLICY;
    ++upper_bound;
    return tmp;
  }

  /*  Impl Prefix Decrement Operator  */

  template < typename T >
  inline const typename interval<T>::Impl interval<T>::Impl::operator --() noexcept
  {
    DOWNWARD_POLICY;
    --lower_bound;
    UPWARD_POLICY;
    --upper_bound;
    return *this;
  }

  /*  Impl Postfix Increment Operator  */

  template < typename T >
  inline const typename interval<T>::Impl interval<T>::Impl::operator --( int ) noexcept
  {
    auto tmp( *this );
    DOWNWARD_POLICY;
    --lower_bound;
    UPWARD_POLICY;
    --upper_bound;
    return tmp;
  }

  /*  Accessors  */

  /*  Interval Low_Bound Getter  */

  template < typename T >
  inline constexpr T interval<T>::Impl::lower() const noexcept
  {
    return lower_bound;
  }

  /*  Interval Upper_Bound Getter  */

  template < typename T >
  inline constexpr T interval<T>::Impl::upper() const noexcept
  {
    return upper_bound;
  }

  /*  Interval Max  */

  /*  Two interval Argument Max  */

  template < typename T >
  inline interval<T> max( interval<T> const& a, interval<T> const& b ) noexcept
  {
    return total_less( a, b ) ? b : a;
  }

  /*  iitializer_list<interval> Argument Max  */

  template < typename T >
  inline interval<T> max( std::initializer_list< interval<T> > list ) noexcept
  {
    std::vector< interval<T> > tmp( list );
    std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T> const& a, interval<T> const& b ) { return total_greater( a, b ); } );
    return *( tmp.begin() );
  }

  /*  initializer_list<interval> and Predicate Argument Max   */

  template < typename T, class Compare >
  inline interval<T> max( std::initializer_list< interval<T> > list, Compare comp ) noexcept
  {
    std::vector< interval<T> > tmp( list );
    std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return comp( a, b ); } );
    return *( tmp.begin() );
  }

  /*  Tow interval and Predicate Argument Max   */
  template < typename T, typename Compare, std::enable_if_t<!detail::is_interval_v<Compare>, std::nullptr_t>*& = nullptr >
  inline interval<T> max( interval<T> const& a, interval<T> const& b, Compare comp ) noexcept
  {
    return comp( a, b ) ? a : b;
  }

  template < typename First, typename Second >
  inline constexpr First max( First&& a, Second&& b )
  {
    return total_less( a, b ) ? a : b;
  }

  /*  Variadic arguments Max  */

  template < typename First, typename Second, typename ...Tail >
  inline constexpr First max( First&& a, Second&& b, Tail&& ... args )
  {
    return min( min( std::forward<First>( a ), std::forward<Second>( b ) ), std::forward<Tail>( args )... );
  }


  /*  Interval Min  */

  /*  Two interval Argument Min  */

  template < typename T >
  inline interval<T> min( interval<T> const& a, interval<T> const& b ) noexcept
  {
    return total_less( a, b ) ? a : b;
  }

  /*  iitializer_list<interval> Argument Min  */

  template < typename T >
  inline interval<T> min( std::initializer_list< interval<T> > list ) noexcept
  {
    std::vector< interval<T> > tmp( list );
    std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return total_less( a, b ); } );
    return *( tmp.begin() );
  }

  /*  initializer_list<interval> and Predicate Argument Min   */

  template < typename T, class Compare >
  inline interval<T> min( std::initializer_list< interval<T> > list, Compare comp ) noexcept
  {
    std::vector< interval<T> > tmp( list );
    std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return comp( a, b ); } );
    return *( tmp.begin() );
  }

  /*  Two intervals and Predicate Argument Max   */

  template < typename T, typename Compare, std::enable_if_t<!detail::is_interval_v<Compare>, std::nullptr_t>*& = nullptr >
  inline interval<T> min( interval<T> const& a, interval<T> const& b, Compare comp ) noexcept
  {
    return comp( a, b ) ? a : b;
  }

  template < typename First, typename Second >
  inline constexpr First min(First&& a, Second&& b)
  {
    return total_less( a, b ) ? a : b;
  }

  /*  Variadic arguments Max  */

  template < typename First, typename Second, typename ...Tail >
  inline constexpr First min( First&& a, Second&& b,Tail&& ... args )
  {
    return min( min(std::forward<First>( a ), std::forward<Second>(b)), std::forward<Tail>( args )... );
  }

  template < typename T >
  inline constexpr std::pair< interval<T> const&, interval<T> const& > minmax( interval<T> const& a, interval<T> const& b ) noexcept
  {
    return a < b ? std::make_pair( a, b ) : std::make_pair( b, a );
  }

  template < typename T, typename Compare >
  inline constexpr std::pair< interval<T> const&, interval<T> const& > minmax( interval<T> const& a, interval<T> const& b, Compare comp ) noexcept
  {
    return comp( a, b ) ? std::make_pair( a, b ) : std::make_pair( b, a );
  }

  template < typename T >
  inline const std::pair< interval<T>, interval<T> > minmax( std::initializer_list< interval<T> > list ) noexcept
  {
    std::vector< interval<T> > v( list );
    auto hold = std::minmax_element( v.begin(), v.end() );
    return std::make_pair( *( hold.first ), *( hold.second ) );
  }

  template < typename T, typename Compare >
  inline const std::pair< interval<T>, interval<T> > minmax( std::initializer_list< interval<T> > list, Compare comp ) noexcept
  {
    std::vector< interval<T> > v( list );
    auto hold = std::minmax_element( v.begin(), v.end(), [&]( interval<T> const& a, interval<T> const& b ) { return comp( a, b ); } );
    return std::make_pair( *( hold.first ), *( hold.second ) );
  }

  //---------------------------------------------------------//
  /*                             */
  /*  Generic Formed Interval Numeric Function Overloading   */
  /*                             */
  //---------------------------------------------------------//

  /*  Generic Formed Interval Numeric Function  */
  template < typename T >
  inline constexpr auto inverse( interval<T> const& a ) { return a.inverse(); }

  template < typename T >
  inline constexpr auto sin( interval<T> const& a ) noexcept { return a.sin(); }

  template < typename T >
  inline constexpr auto cos( interval<T> const& a ) noexcept { return a.cos(); }

  template < typename T >
  inline constexpr auto tan( interval<T> const& a ) { return a.tan(); }

  template < typename T >
  inline constexpr auto sec( interval<T> const& a ) { return a.sec(); }

  template < typename T >
  inline constexpr auto csc( interval<T> const& a ) { return a.csc(); }

  template < typename T >
  inline constexpr auto cot( interval<T> const& a ) { return a.cot(); }

  template < typename T >
  inline constexpr auto asin( interval<T> const& a ) { return a.asin(); }

  template < typename T >
  inline constexpr auto acos( interval<T> const& a ) { return a.acos(); }

  template < typename T >
  inline constexpr auto atan( interval<T> const& a ) { return a.atan(); }

  template < typename T >
  inline constexpr auto asec( interval<T> const& a ) { return a.asec(); }

  template < typename T >
  inline constexpr auto acsc( interval<T> const& a ) { return a.acsc(); }

  template < typename T >
  inline constexpr auto acot( interval<T> const& a ) { return a.acot(); }

  template < typename T >
  inline constexpr auto sinh( interval<T> const& a ) { return a.sinh(); }

  template < typename T >
  inline constexpr auto cosh( interval<T> const& a ) { return a.cosh(); }

  template < typename T >
  inline constexpr auto tanh( interval<T> const& a ) noexcept { return a.tanh(); }

  template < typename T >
  inline constexpr auto sech( interval<T> const& a ) noexcept { return a.sech(); }

  template < typename T >
  inline constexpr auto csch( interval<T> const& a ) { return a.csch(); }

  template < typename T >
  inline constexpr auto coth( interval<T> const& a ) { return a.coth(); }

  template < typename T >
  inline constexpr auto asinh( interval<T> const& a ) noexcept { return a.asinh(); }

  template < typename T >
  inline constexpr auto acosh( interval<T> const& a ) { return a.acosh(); }

  template < typename T >
  inline constexpr auto atanh( interval<T> const& a ) { return a.atanh(); }

  template < typename T >
  inline constexpr auto asech( interval<T> const& a ) { return a.asech(); }

  template < typename T >
  inline constexpr auto acsch( interval<T> const& a ) { return a.acsch(); }

  template < typename T >
  inline constexpr auto acoth( interval<T> const& a ) { return a.acoth(); }

  template < typename T >
  inline constexpr auto exp( interval<T> const& a ) { return a.exp(); }

  template < typename T >
  inline constexpr auto exp2( interval<T> const& a ) { return a.exp2(); }

  template < typename T >
  inline constexpr auto expm1( interval<T> const& a ) { return a.expm1(); }

  template < typename T >
  inline constexpr auto pow( interval<T> const& a, double n ) { return a.pow( n ); }

  template < typename T >
  inline constexpr auto abs( interval<T> const& a ) { return a.abs(); }

  template < typename T >
  inline constexpr auto log( interval<T> const& a ) { return a.log(); }

  template < typename T >
  inline constexpr auto log10( interval<T> const& a ) { return a.log10(); }

  template < typename T >
  inline constexpr auto log2( interval<T> const& a ) { return a.log2(); }

  template < typename T >
  inline constexpr auto log1p( interval<T> const& a ) { return a.log1p(); }

  template < typename T >
  inline constexpr auto sqrt( interval<T> const& a ) { return a.sqrt(); }

  template < typename T >
  inline constexpr auto cbrt( interval<T> const& a ) { return a.cbrt(); }

  template < typename T >
  inline constexpr auto erf( interval<T> const& a ) { return a.erf(); }

  template < typename T >
  inline constexpr auto erfc( interval<T> const& a ) { return a.erfc(); }

  template < typename T >
  inline constexpr T wid( interval<T> const& a ) { return a.wid(); }

  template < typename T >
  inline constexpr T mid( interval<T> const& a ) { return a.mid(); }

  template < typename T >
  inline constexpr T width( interval<T> const& a ) { return a.width(); }

  template < typename T >
  inline constexpr T rad( interval<T> const& a ) { return a.rad(); }

  template < typename T >
  inline constexpr T middle( interval<T> const& a ) { return a.middle(); }

  template < typename T >
  inline constexpr T norm( interval<T> const& a ) { return a.norm(); }

  template < typename T >
  inline constexpr T mig( interval<T> const& a ) { return a.mig(); }

  template < typename T >
  inline constexpr T mag( interval<T> const& a ) { return a.mag(); }

  template < typename T >
  inline constexpr T lower( interval<T> const& a ) { return a.lower(); }

  template < typename T >
  inline constexpr T upper( interval<T> const& a ) { return a.upper(); }

  template < typename T >
  inline constexpr bool is_singleton( interval<T> const& a ) { return a.is_singleton(); }

  template < typename T >
  inline constexpr auto inverse( T&& a )->decltype( a.eval() ) { return a.eval().inverse(); }

  template < typename T >
  inline constexpr auto sin( T&& a )->decltype( a.eval() ) { return a.eval().sin(); }

  template < typename T >
  inline constexpr auto cos( T&& a )->decltype( a.eval() ) { return a.eval().cos(); }

  template < typename T >
  inline constexpr auto tan( T&& a )->decltype( a.eval() ) { return a.eval().tan(); }

  template < typename T >
  inline constexpr auto asin( T&& a )->decltype( a.eval() ) { return a.eval().asin(); }

  template < typename T >
  inline constexpr auto acos( T&& a )->decltype( a.eval() ) { return a.eval().acos(); }

  template < typename T >
  inline constexpr auto atan( T&& a )->decltype( a.eval() ) { return a.eval().atan(); }

  template < typename T >
  inline constexpr auto sinh( T&& a )->decltype( a.eval() ) { return a.eval().sinh(); }

  template < typename T >
  inline constexpr auto cosh( T&& a )->decltype( a.eval() ) { return a.eval().cosh(); }

  template < typename T >
  inline constexpr auto tanh( T&& a )->decltype( a.eval() ) { return a.eval().tanh(); }

  template < typename T >
  inline constexpr auto asinh( T&& a )->decltype( a.eval() ) { return a.eval().asinh(); }

  template < typename T >
  inline constexpr auto acosh( T&& a )->decltype( a.eval() ) { return a.eval().acosh(); }

  template < typename T >
  inline constexpr auto atanh( T&& a )->decltype( a.eval() ) { return a.eval().atanh(); }

  template < typename T >
  inline constexpr auto exp( T&& a )->decltype( a.eval() ) { return a.eval().exp(); }

  template < typename T >
  inline constexpr auto exp2( T&& a )->decltype( a.eval() ) { return a.eval().exp2(); }

  template < typename T >
  inline constexpr auto expm1( T&& a )->decltype( a.eval() ) { return a.eval().expm1(); }

  template < typename T >
  inline constexpr auto pow( T&& a, int const& n )->decltype( a.eval() ) { return a.eval().pow( n ); }

  template < typename T >
  inline constexpr auto abs( T&& a )->decltype( a.eval() ) { return a.eval().abs(); }

  template < typename T >
  inline constexpr auto log( T&& a )->decltype( a.eval() ) { return a.eval().log(); }

  template < typename T >
  inline constexpr auto log10( T&& a )->decltype( a.eval() ) { return a.eval().log10(); }

  template < typename T >
  inline constexpr auto log2( T&& a )->decltype( a.eval() ) { return a.eval().log2(); }

  template < typename T >
  inline constexpr auto log1p( T&& a )->decltype( a.eval() ) { return a.eval().log1p(); }

  template < typename T >
  inline constexpr auto sqrt( T&& a )->decltype( a.eval() ) { return a.eval().sqrt(); }

  template < typename T >
  inline constexpr auto cbrt( T&& a )->decltype( a.eval() ) { return a.eval().cbrt(); }

  template < typename T >
  inline constexpr auto erf( T&& a )->decltype( a.eval() ) { return a.eval().erf(); }

  template < typename T >
  inline constexpr auto erfc( T&& a )->decltype( a.eval() ) { return a.eval().erfc(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto wid( T&& a ) { return a.eval().wid(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto mid( T&& a ) { return a.eval().mid(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto width( T&& a ) { return a.eval().width(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto rad( T&& a ) { return a.eval().rad(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto middle( T&& a ) { return a.eval().middle(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto norm( T&& a ) { return a.eval().norm(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto mig( T&& a ) { return a.eval().mig(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto mag( T&& a ) { return a.eval().mag(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto lower( T&& a ) { return a.eval().lower(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr auto upper( T&& a ) { return a.eval().upper(); }

  template < typename T, std::enable_if_t<!detail::is_expr_v<T>, std::nullptr_t>*& = nullptr >
  inline constexpr bool is_singleton( T&& a ) { return a.is_singleton(); }

  template < typename T >
  inline void swap( interval<T>& x, interval<T>& y ) noexcept
  {
    x.swap( y );
  }
  /*  Generic numeric function for primitive dispatch  */


  /*  for enum output with string  */

  //interval_ordering to string
  const std::string enum2str( interval_ordering const& x )
  {
    switch ( x )
    {
    case Cranberries::interval_ordering::less:
      return std::string( "less" );
    case Cranberries::interval_ordering::greater:
      return std::string( "greater" );
    case Cranberries::interval_ordering::unordered:
      return std::string( "unordered" );
    default:
      return std::string( "niether" );
    }
  }

  //partial_ordering to string
  const std::string enum2str( partial_ordering const& x )
  {
    switch ( x )
    {
    case Cranberries::partial_ordering::less:
      return std::string( "less" );
    case Cranberries::partial_ordering::greater:
      return std::string( "greater" );
    case Cranberries::partial_ordering::unordered:
      return std::string( "unordered" );
    default:
      return std::string( "niether" );
    }
  }

  //weak_ordering to string
  const std::string enum2str( weak_ordering const& x )
  {
    switch ( x )
    {
    case Cranberries::weak_ordering::less:
      return std::string( "less" );
    case Cranberries::weak_ordering::greater:
      return std::string( "greater" );
    case Cranberries::weak_ordering::equivalent:
      return std::string( "equivalent" );
    default:
      return std::string( "niether" );
    }
  }

  //total_ordering to string
  const std::string enum2str( total_ordering const& x )
  {
    switch ( x )
    {
    case Cranberries::total_ordering::less:
      return std::string( "less" );
    case Cranberries::total_ordering::greater:
      return std::string( "greater" );
    case Cranberries::total_ordering::equal:
      return std::string( "equal" );
    default:
      return std::string( "niether" );
    }
  }

  //interval_relation to string to string
  const std::string enum2str( interval_relation const& x )
  {
    switch ( x )
    {
    case Cranberries::interval_relation::interval_less:
      return std::string( "interval_less" );
    case Cranberries::interval_relation::interval_greater:
      return std::string( "interval_greater" );
    case Cranberries::interval_relation::partial_less:
      return std::string( "partial_less" );
    case Cranberries::interval_relation::partial_greater:
      return std::string( "partial_greater" );
    case Cranberries::interval_relation::weak_less:
      return std::string( "weak_less" );
    case Cranberries::interval_relation::weak_greater:
      return std::string( "weak_greater" );
    case Cranberries::interval_relation::total_less:
      return std::string( "total_less" );
    case Cranberries::interval_relation::total_greater:
      return std::string( "total_greater" );
    case Cranberries::interval_relation::contain:
      return std::string( "contain" );
    case Cranberries::interval_relation::part_of:
      return std::string( "part_of" );
    case Cranberries::interval_relation::equal:
      return std::string( "equal" );
    case Cranberries::interval_relation::niether:
      return std::string( "niether" );
    }
    CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG( "unknown enumerator." )
}

  /*  ostream << enum overload  */

  //ostream << interval_ordering
  std::ostream& operator<<( std::ostream& s, interval_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

  //ostream << partial_ordering
  std::ostream& operator<<( std::ostream& s, partial_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

  //ostream << weak_ordering
  std::ostream& operator<<( std::ostream& s, weak_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

  //ostream << total_ordering
  std::ostream& operator<<( std::ostream& s, total_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

  //ostream << interval_relation
  std::ostream& operator<<( std::ostream& s, interval_relation const& x )
  {
    return ( s << enum2str( x ) );
  }

  /*  Generic operator<< for class whitch has menber function print( ostream& )  */

  template < typename T >
  decltype( auto ) operator<<( std::ostream& s, interval<T> const& x )
  {
    return s << "[ " << x.lower() << ", " << x.upper() << " ]";
  }

  /*  output for version  */

  std::ostream& operator<<( std::ostream& s, version_tag const& v )
  {
    return s << ( static_cast<unsigned>( v ) / 0x01000000 )
      << "." << ( ( static_cast<unsigned>( v ) % 0x01000000 ) / 0x00010000 )
      << "." << ( static_cast<unsigned>( v ) % 0x00010000 );
  }

  /*
  [ Note : Four arithmetic operator has three ovreload,
  interval<L> @ interval<R>, L @ interval<R> and interval<L> @ R .
  High accuracy arithmetic operation is default. - end note ]
  */

  /*
  [ Example : High accuracy operation is available default operator-, * and / .
  auto x = hull( 1.0L, 2.0L ) ;
  auto y = hull( -1.0L, 1.0L ) ;
  x - x //  returns [ 0.0L, 0.0L ]
  y * y //  returns [ 0.0L, 1.0L ]
  x / x //  returns [ 1.0L, 1.0L ]
  - end example ]
  */

  /*
  [ Example : You can switch high accuracy arithmetic into nomal arithmetic using namespace inside a scope block.
  auto x = hull( 1.0L, 2.0L ) ;
  auto y = hull( -1.0L, 1.0L ) ;
  {
  using namespace Cranberries::nomal_accuracy;
  x - x //  returns [ -1.0L, 1.0L ]
  y - y //  returns [ -1.0L, 1.0L ]
  x / x //  returns [ 0.5L, 2.0L ]
  }
  - end example ]
  */


  /*  Interval Addition Op  */

  namespace normal_accuracy
  {
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> add( interval<L> const& x, interval<R> const& y )
    {

      auto&& x_low = x.lower();
      auto&& x_up = x.upper();
      auto&& y_low = y.lower();
      auto&& y_up = y.upper();

      CRANBERRIES_RANGE_ERROR_THROW_IF( x_low + y_low == -constants::max<T>() || x_up + y_up == constants::max<T>() )
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x_low + y_low;
        UPWARD_POLICY;
        auto&& r = x_up + y_up;
        return interval<T>{ l, r };
      }
    }

    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> sub( interval<L> const& x, interval<R> const& y )
    {
      auto&& x_low = x.lower();
      auto&& x_up = x.upper();
      auto&& y_low = y.lower();
      auto&& y_up = y.upper();

      CRANBERRIES_RANGE_ERROR_THROW_IF( x_low - y_up == -constants::max<T>() || x_up - y_low == constants::max<T>() )
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x.lower() - y.upper();
        UPWARD_POLICY;
        auto&& r = x.upper() - y.lower();
        return interval<T>{ l, r };
      }
    }

    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> multi( interval<L> const& x, interval<R> const& y )
    {
      auto&& x_lower = x.lower();
      auto&& x_upper = x.upper();
      auto&& y_lower = y.lower();
      auto&& y_upper = y.upper();

      if ( x_lower >= constants::zero<T>() && y_lower >= constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_lower == -constants::max<T>() || x_upper * y_upper == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_lower * y_lower;
          UPWARD_POLICY;
          auto&& r = x_upper * y_upper;
          return interval<T>{ l, r };
        }
      }
      else if ( x_lower >= constants::zero<T>() && y_lower < constants::zero<T>() && y_upper > constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_lower == -constants::max<T>() || x_upper * y_upper == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_upper * y_lower;
          UPWARD_POLICY;
          auto&& r = x_upper * y_upper;
          return interval<T>{ l, r };
        }
      }
      else if ( x_lower >= constants::zero<T>() && y_upper <= constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_lower == -constants::max<T>() || x_lower * y_upper == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_upper * y_lower;
          UPWARD_POLICY;
          auto&& r = x_lower * y_upper;
          return interval<T>{ l, r };
        }
      }
      else if ( x_lower < constants::zero<T>() && x_upper > constants::zero<T>() && y_lower >= constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_upper == -constants::max<T>() || x_upper * y_upper == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_lower * y_upper;
          UPWARD_POLICY;
          auto&& r = x_upper * y_upper;
          return interval<T>{ l, r };
        }
      }
      else if ( x_lower < constants::zero<T>() && x_upper > constants::zero<T>() && y_upper <= constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_lower == -constants::max<T>() || x_lower * y_lower == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_upper * y_lower;
          UPWARD_POLICY;
          auto&& r = x_lower * y_lower;
          return interval<T>{ l, r };
        }
      }
      else if ( x_upper <= constants::zero<T>() && y_lower >= constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_upper == -constants::max<T>() || x_upper * y_lower == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_lower * y_upper;
          UPWARD_POLICY;
          auto&& r = x_upper * y_lower;
          return interval<T>{ l, r };
        }
      }
      else if ( x_upper <= constants::zero<T>() && y_lower < constants::zero<T>() && y_upper > constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_upper == -constants::max<T>() || x_lower * y_lower == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_lower * y_upper;
          UPWARD_POLICY;
          auto&& r = x_lower * y_lower;
          return interval<T>{ l, r };
        }
      }
      else if ( x_upper <= constants::zero<T>() && y_upper <= constants::zero<T>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_upper == -constants::max<T>() || x_lower * y_lower == constants::max<T>() )
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x_upper * y_upper;
          UPWARD_POLICY;
          auto&& r = x_lower * y_lower;
          return interval<T>{ l, r };
        }
      }
      else
      {
        T l;
        T r;
        if ( x_lower * y_upper < x_upper * y_lower )
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower*y_upper == -constants::max<T>() )
          else
          {
            DOWNWARD_POLICY;
            l = x_lower * y_upper;
          }
        }
        else
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper*y_lower == -constants::max<T>() )
          else
          {
            DOWNWARD_POLICY;
            l = x_upper * y_lower;
          }
        }
        if ( x_lower*y_lower < x_upper*y_upper )
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper*y_upper == constants::max<T>() )
          else
          {
            UPWARD_POLICY;
            r = x_upper * y_upper;
          }
        }
        else
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower*y_lower == constants::max<T>() )
          else
          {
            UPWARD_POLICY;
            r = x_lower * y_lower;
          }
        }
        return interval<T>{ l, r };
      }
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> div( interval<L> const& x, interval<R> const& y )
    {
      auto&& y_lower = y.lower();
      auto&& y_upper = y.upper();

      CRANBERRIES_OVER_FLOW_THROW_IF( y_lower <= constants::zero<R>() && y_upper >= constants::zero<R>() )
      else
      {
        DOWNWARD_POLICY;
        auto&& l = constants::one<R>() / y_upper;
        UPWARD_POLICY;
        auto&& r = constants::one<R>() / y_lower;
        auto&& tmp = interval<R>{ l, r };
        return normal_accuracy::multi( x, tmp );
      }
    }
    template < typename L, typename R >
    inline interval<L>& add_assign( interval<L>& x, interval<R> const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x.lower() + y.lower();
      UPWARD_POLICY;
      auto&& r = x.upper() + y.upper();
      x.reset( std::move( l ), std::move( r ) );

      return x;
    }

    template < typename L, typename R >
    inline interval<L>& sub_assign( interval<L>& x, interval<R> const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x.lower() - y.upper();
      UPWARD_POLICY;
      auto&& r = x.upper() - y.lower();
      x.reset( std::move( l ), std::move( r ) );

      return x;
    }

    template < typename L, typename R >
    inline interval<L>& multi_assign( interval<L>& x, interval<R> const& y )
    {
      auto&& x_lower = x.lower();
      auto&& x_upper = x.upper();
      auto&& y_lower = y.lower();
      auto&& y_upper = y.upper();

      if ( x_lower >= constants::zero<L>() && y_lower >= constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_lower == -constants::max<L>() || x_upper * y_upper == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_lower * y_lower;
          UPWARD_POLICY;
          L&& r = x_upper * y_upper;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_lower >= constants::zero<L>() && y_lower < constants::zero<R>() && y_upper > constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_lower == -constants::max<L>() || x_upper * y_upper == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_upper * y_lower;
          UPWARD_POLICY;
          L&& r = x_upper * y_upper;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_lower >= constants::zero<L>() && y_upper <= constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_lower == -constants::max<L>() || x_lower * y_upper == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_upper * y_lower;
          UPWARD_POLICY;
          L&& r = x_lower * y_upper;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_lower < constants::zero<L>() && x_upper > constants::zero<L>() && y_lower >= constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_upper == -constants::max<L>() || x_upper * y_upper == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_lower * y_upper;
          UPWARD_POLICY;
          L&& r = x_upper * y_upper;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_lower < constants::zero<L>() && x_upper > constants::zero<L>() && y_upper <= constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_lower == -constants::max<L>() || x_lower * y_lower == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_upper * y_lower;
          UPWARD_POLICY;
          L&& r = x_lower * y_lower;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_upper <= constants::zero<L>() && y_lower >= constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_upper == -constants::max<L>() || x_upper * y_lower == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_lower * y_upper;
          UPWARD_POLICY;
          L&& r = x_upper * y_lower;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_upper <= constants::zero<L>() && y_lower < constants::zero<R>() && y_upper > constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower * y_upper == -constants::max<L>() || x_lower * y_lower == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_lower * y_upper;
          UPWARD_POLICY;
          L&& r = x_lower * y_lower;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else if ( x_upper <= constants::zero<L>() && y_upper <= constants::zero<R>() )
      {
        CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper * y_upper == -constants::max<L>() || x_lower * y_lower == constants::max<L>() )
        else
        {
          DOWNWARD_POLICY;
          L&& l = x_upper * y_upper;
          UPWARD_POLICY;
          L&& r = x_lower * y_lower;
          x.reset( std::move( l ), std::move( r ) );
          return x;
        }
      }
      else
      {
        L l;
        L r;
        if ( x_lower * y_upper < x_upper * y_lower )
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower*y_upper == -constants::max<L>() )
          else
          {
            DOWNWARD_POLICY;
            l = x_lower * y_upper;
          }
        }
        else
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper*y_lower == -constants::max<L>() )
          else
          {
            DOWNWARD_POLICY;
            l = x_upper * y_lower;
          }
        }
        if ( x_lower*y_lower < x_upper*y_upper )
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_upper*y_upper == constants::max<L>() )
          else
          {
            UPWARD_POLICY;
            r = x_upper * y_upper;
          }
        }
        else
        {
          CRANBERRIES_RANGE_ERROR_THROW_IF( x_lower*y_lower == constants::max<L>() )
          else
          {
            UPWARD_POLICY;
            r = x_lower * y_lower;
          }
        }
        x.reset( std::move( l ), std::move( r ) );
        return x;
      }
    }
    template < typename L, typename R >
    inline interval<L>& div_assign( interval<L>& x, interval<R> const& y )
    {
      auto&& y_lower = y.lower();
      auto&& y_upper = y.upper();

      CRANBERRIES_RANGE_ERROR_THROW_IF( y_lower <= constants::zero<R>() && y_upper >= constants::zero<R>() )
      else
      {
        DOWNWARD_POLICY;
        auto&& l = constants::one<R>() / y_upper;
        UPWARD_POLICY;
        auto&& r = constants::one<R>() / y_lower;
        auto&& tmp = interval<R>{ l, r };
        return normal_accuracy::multi_assign( x, tmp );
      }
  }
  }

  inline namespace high_accuracy
  {
    template < typename T >
    inline interval<T> add( interval<T> const& x, interval<T> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> add( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename T >
    inline interval<T> sub( interval<T> const& x, interval<T> const& y )
    {
      if ( &x == &y )
      {
        return interval<T>{};
      }
      else
      {
        return normal_accuracy::sub( x, y );
      }
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    interval<T> sub( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename T >
    inline interval<T> multi( interval<T> const& x, interval<T> const& y )
    {
      if ( &x == &y )
      {
        return pow( x, 2 );
      }
      else
      {
        return normal_accuracy::multi( x, y );
      }
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> multi( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename T >
    inline interval<T> div( interval<T> const& x, interval<T> const& y )
    {
      if ( &x == &y )
      {
        return interval<T>{ constants::one<T>(), constants::one<T>() };
      }
      CRANBERRIES_OVER_FLOW_THROW_ELSE_IF( y.lower() <= constants::zero<T>() && y.upper() >= constants::zero<T>() )
      else
      {
        return normal_accuracy::div( x, y );
      }
    }
    template < typename L, typename R, typename T = std::common_type_t<L, R> >
    inline interval<T> div( interval<L> const& x, interval<R> const& y )
    {
      auto&& y_lower = y.lower();
      auto&& y_upper = y.upper();

      CRANBERRIES_OVER_FLOW_THROW_IF( y_lower() <= constants::zero<R>() && y_upper() >= constants::zero<R>() )
      else
      {
        DOWNWARD_POLICY;
        auto&& l = constants::one<R>() / y_upper;
        UPWARD_POLICY;
        auto&& r = constants::one<R>() / y_lower;
        auto&& tmp = interval<R>{ l, r };
        return normal_accuracy::multi( x, tmp );
      }
    }

    template < typename L, typename R >
    inline interval<L>& add_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::add_assign( x, y );
    }

    template < typename L, typename R >
    inline interval<L>& sub_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::sub_assign( x, y );
    }

    template < typename T >
    inline interval<T>& sub_assign( interval<T>& x, interval<T> const& y )
    {
      if ( &x == &y )
      {
        x.reset( constants::zero<T>(), constants::zero<T>() );
        return x;
      }
      else
      {
        return normal_accuracy::sub_assign( x, y );
      }
    }

    template < typename T >
    inline interval<T>& multi_assign( interval<T>& x, interval<T> const& y )
    {
      using std::pow;
      if ( &x == &y )
      {
        if ( x.lower() > constants::zero<T>() )
        {
          DOWNWARD_POLICY;
          auto&& l = pow( x.lower(), 2 );
          UPWARD_POLICY;
          auto&& r = pow( x.upper(), 2 );
          x.reset( l, r );
        }
        else if ( x.upper() < constants::zero<T>() )
        {
          DOWNWARD_POLICY;
          auto&& l = pow( x.upper(), 2 );
          UPWARD_POLICY;
          auto&& r = pow( x.lower(), 2 );
          x.reset( l, r );
        }
        else
        {
          UPWARD_POLICY;
          auto&& l = pow( x.upper(), 2 );
          auto&& r = pow( x.lower(), 2 );
          x.reset( constants::zero<T>(), std::fmax( l, r ) );
        }
        return x;
      }
      else
      {
        return normal_accuracy::multi_assign( x, y );
      }
    }
    template < typename L, typename R >
    inline interval<L>& multi_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::multi_assign( x, y );
    }

    template < typename L, typename R >
    inline interval<L>& div_assign( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::div_assign( x, y );
    }
    template < typename T >
    inline interval<T>& div_assign( interval<T>& x, interval<T> const& y )
    {
      CRANBERRIES_OVER_FLOW_THROW_IF( y.lower() <= constants::zero<T>() && y.upper() >= constants::zero<T>() )
      else
      {
        if ( &x == &y )
        {
          x.reset( constants::one<T>(), constants::one<T>() );
          return x;
        }
        return normal_accuracy::div_assign( x, y );
      }
    }
  }

  inline namespace
  {
    template < typename R >
    inline interval<R> sub( typename interval<R>::value_type const& x, interval<R> const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x - y.upper();
      UPWARD_POLICY;
      auto&& r = x - y.lower();
      return interval<R>{ l, r };
    }

    template < typename L >
    inline interval<L> sub( interval<L> const& x, typename interval<L>::value_type const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x.lower() - y;
      UPWARD_POLICY;
      auto&& r = x.upper() - y;
      return interval<L>{ l, r };
    }

    template < typename R >
    inline interval<R> add( typename interval<R>::value_type const& x, interval<R> const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x + y.lower();
      UPWARD_POLICY;
      auto&& r = x + y.upper();
      return interval<R>{ l, r };
    }

    template < typename L >
    inline interval<L> add( interval<L> const& x, typename interval<L>::value_type const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x.lower() + y;
      UPWARD_POLICY;
      auto&& r = x.upper() + y;
      return interval<L>{ l, r };
    }

    template < typename R >
    inline interval<R> multi( typename interval<R>::value_type const& x, interval<R> const& y )
    {
      if ( x < constants::zero<R>() )
      {
        DOWNWARD_POLICY;
        auto&& l = x * y.upper();
        UPWARD_POLICY;
        auto&& r = x * y.lower();
        return interval<R>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x * y.lower();
        UPWARD_POLICY;
        auto&& r = x * y.upper();
        return interval<R>{ l, r };
      }
    }

    template < typename L >
    inline interval<L> multi( interval<L> const& x, typename interval<L>::value_type const& y )
    {
      if ( y < constants::zero<L>() )
      {
        DOWNWARD_POLICY;
        auto&& l = x.upper() * y;
        UPWARD_POLICY;
        auto&& r = x.lower() * y;
        return interval<L>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x.lower() * y;
        UPWARD_POLICY;
        auto&& r = x.upper() * y;
        return interval<L>{ l, r };
      }
    }

    template < typename L >
    inline interval<L> div( interval<L> const& x, typename interval<L>::value_type const& y )
    {
      CRANBERRIES_OVER_FLOW_THROW_IF( y == constants::zero<L>() )
      else if ( y < constants::zero<L>() )
      {
        DOWNWARD_POLICY;
        auto&& l = x.upper() / y;
        UPWARD_POLICY;
        auto&& r = x.lower() / y;
        return interval<L>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x.lower() / y;
        UPWARD_POLICY;
        auto&& r = x.upper() / y;
        return interval<L>{ l, r };
      }
    }

    template < typename R >
    inline interval<R> div( typename interval<R>::value_type const& x, interval<R> const& y )
    {
      CRANBERRIES_OVER_FLOW_THROW_IF( y.lower() < constants::zero<R>() && y.upper() > constants::zero<R>() )
      else if ( y.lower() > constants::zero<R>() )
      {
        DOWNWARD_POLICY;
        auto&& l = x / y.upper();
        UPWARD_POLICY;
        auto&& r = x / y.lower();
        return interval<R>{ l, r };
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x / y.lower();
        UPWARD_POLICY;
        auto&& r = x / y.upper();
        return interval<R>{ l, r };
      }
    }

    template < typename L >
    inline interval<L>& add_assign( interval<L>& x, typename interval<L>::value_type const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x.lower() + y;
      UPWARD_POLICY;
      auto&& r = x.upper() + y;
      x.reset( std::move( l ), std::move( r ) );
      return x;
    }

    template < typename L >
    inline interval<L>& sub_assign( interval<L>& x, typename interval<L>::value_type const& y )
    {
      DOWNWARD_POLICY;
      auto&& l = x.lower() - y;
      UPWARD_POLICY;
      auto&& r = x.upper() - y;
      x.reset( std::move( l ), std::move( r ) );
      return x;
    }

    template < typename L >
    inline interval<L>& multi_assign( interval<L>& x, typename interval<L>::value_type const& y )
    {
      if ( y < constants::zero<L>() )
      {
        DOWNWARD_POLICY;
        auto&& l = x.upper() * y;
        UPWARD_POLICY;
        auto&& r = x.lower() * y;
        x.reset( std::move( l ), std::move( r ) );
      }
      else
      {
        DOWNWARD_POLICY;
        auto&& l = x.lower() * y;
        UPWARD_POLICY;
        auto&& r = x.upper() * y;
        x.reset( std::move( l ), std::move( r ) );
      }
      return x;
    }
    template < typename L >
    inline interval<L>& div_assign( interval<L>& x, typename interval<L>::value_type const& y )
    {
      CRANBERRIES_OVER_FLOW_THROW_IF( y == constants::zero<L>() )
      else
      {
        if ( y < constants::zero<L>() )
        {
          DOWNWARD_POLICY;
          auto&& l = x.upper() / y;
          UPWARD_POLICY;
          auto&& r = x.lower() / y;
          x.reset( std::move( l ), std::move( r ) );
        }
        else
        {
          DOWNWARD_POLICY;
          auto&& l = x.lower() / y;
          UPWARD_POLICY;
          auto&& r = x.upper() / y;
          x.reset( std::move( l ), std::move( r ) );
        }
        return x;
      }
    }
  }

  namespace normal_accurate_policy
  {
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::add( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L>&& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( interval<L> const&& x, R&& y )
    {
      return add( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R>& y )
    {
      return add( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R> const& y )
    {
      return add( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R>&& y )
    {
      return add( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, interval<R> const&& y )
    {
      return add( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::sub( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>& x, R&& y )
    {
      return sub( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const& x, R&& y )
    {
      return sub( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L>&& x, R&& y )
    {
      return sub( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( interval<L> const&& x, R&& y )
    {
      return sub( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R>& y )
    {
      return sub( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R> const& y )
    {
      return sub( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R>&& y )
    {
      return sub( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, interval<R> const&& y )
    {
      return sub( std::forward<L>( x ), y );
    }

    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::multi( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R>& y )
    {
      return multi( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R> const& y )
    {
      return multi( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R>&& y )
    {
      return multi( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, interval<R> const&& y )
    {
      return multi( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>& x, R&& y )
    {
      return multi( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const& x, R&& y )
    {
      return multi( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L>&& x, R&& y )
    {
      return multi( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( interval<L> const&& x, R&& y )
    {
      return multi( x, std::forward<R>( y ) );
    }

    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R>& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R> const& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R>&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, interval<R> const&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R>& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R> const& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R>&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, interval<R> const&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R>& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R> const& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R>&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, interval<R> const&& y )
    {
      return normal_accuracy::div( x, y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R>& y )
    {
      return div( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R> const& y )
    {
      return div( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R>&& y )
    {
      return div( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, interval<R> const&& y )
    {
      return div( std::forward<L>( x ), y );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>& x, R&& y )
    {
      return div( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const& x, R&& y )
    {
      return div( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L>&& x, R&& y )
    {
      return div( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( interval<L> const&& x, R&& y )
    {
      return div( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator+=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::add_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::sub_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::sub_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::sub_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator-=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::sub_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::multi_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::multi_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::multi_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator*=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::multi_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R>& y )
    {
      return normal_accuracy::div_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R> const& y )
    {
      return normal_accuracy::div_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R>&& y )
    {
      return normal_accuracy::div_assign( x, y );
    }
    template < typename L, typename R >
    inline interval<L>& operator/=( interval<L>& x, interval<R> const&& y )
    {
      return normal_accuracy::div_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return add_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return add_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return add_assign( x, y );
    }
    template < typename L >
    inline interval<L>& operator+=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return add_assign( x, y );
    }

    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return sub_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return sub_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return sub_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator-=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return sub_assign( x, std::forward<L>( y ) );
    }

    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return multi_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return multi_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return multi_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator*=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return multi_assign( x, std::forward<L>( y ) );
    }

    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type& y )
    {
      return div_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type const& y )
    {
      return div_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type&& y )
    {
      return div_assign( x, std::forward<L>( y ) );
    }
    template < typename L >
    inline interval<L>& operator/=( interval<L>& x, typename interval<L>::value_type const&& y )
    {
      return div_assign( x, std::forward<L>( y ) );
    }
  }

  inline namespace high_accurate_policy
  {
    template < typename L, typename R >
    inline constexpr auto operator+( L&& x, R&& y )->decltype( add( x, y ) )
    {
      CRANBERRIES_ASSERT( !( detail::is_expr_v<L> || detail::is_expr_v<R> ) );
      return add( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-( L&& x, R&& y )->decltype( sub( x, y ) )
    {
      CRANBERRIES_ASSERT( !( detail::is_expr_v<L> || detail::is_expr_v<R> ) );
      return sub( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*( L&& x, R&& y )->decltype( multi( x, y ) )
    {
      CRANBERRIES_ASSERT( !( detail::is_expr_v<L> || detail::is_expr_v<R> ) );
      return multi( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/( L&& x, R&& y )->decltype( div( x, y ) )
    {
      CRANBERRIES_ASSERT( !( detail::is_expr_v<L> || detail::is_expr_v<R> ) );
      return div( std::forward<L>( x ), std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator+=( interval<L>& x, R&& y )->decltype( add_assign( x, y ) )
    {
      return add_assign( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator-=( interval<L>& x, R&& y )->decltype( sub_assign( x, y ) )
    {
      return sub_assign( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator*=( interval<L>& x, R&& y )->decltype( multi_assign( x, y ) )
    {
      return multi_assign( x, std::forward<R>( y ) );
    }
    template < typename L, typename R >
    inline constexpr auto operator/=( interval<L>& x, R&& y )->decltype( div_assign( x, y ) )
    {
      return div_assign( x, std::forward<R>( y ) );
    }

  }

  //-------------------//
  /*  interval output  */
  //-------------------//

  /*  Generic Output Stream  */

  template < typename T >
  inline std::ostream& operator<<( std::ostream& s, std::unique_ptr<T> x ) noexcept
  {
    s << ( *x );
    return ( s );
  }

  /*  function returns interval into C style string  */

  template < typename T >
  inline const char* Cranberries::interval<T>::c_str() const noexcept
  {
    auto s = new std::string( "[ " + std::to_string( this->lower() ) + ", " + std::to_string( this->upper() ) + " ]" );
    return s->c_str();
  }

  //------------------------------------------------------------------------//
  /*  It is cumbersome to write the type every declaration.         */
  /*  Like ' auto x = interval<double>( 1, 3) '.              */
  /*  And it become source of mistakes.                   */
  /*  Therefore, you can use factory method 'hull( Low, Up )'.           */
  /*  It allows infer to type from arguments and returns interval object.   */
  //------------------------------------------------------------------------//

  template < typename T >
  inline interval<T> hull( T&& low, T&& up )
  {
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( low > up, "upper_bound less than lower_bound!" )
      return interval<T>{ std::forward<T>( low ), std::forward<T>( up ) };
  }

  template < typename T >
  inline interval<T> hull( T&& x )
  {
    return interval<T>{ std::forward<T>( x ) };
  }


  //-----------------------------------//
  /*   interval relational functions   */
  //-----------------------------------//

  /*  Interval Ordering  */

  // interval less
  template < typename L, typename R >
  inline constexpr bool interval_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.upper() < y.lower() );
  }

  template < typename R >
  inline constexpr bool interval_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.lower() );
  }

  template < typename L >
  inline constexpr bool interval_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() < y );
  }

  template < typename L, typename R >
  inline constexpr bool interval_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.upper() <= y.lower() );
  }
  template < typename R >
  inline constexpr bool interval_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.lower() );
  }

  template < typename L >
  inline constexpr bool interval_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() <= y );
  }

  // interval greater
  template < typename L, typename R >
  inline constexpr bool interval_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() > y.upper() );
  }

  template < typename R >
  inline constexpr bool interval_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.upper() );
  }

  template < typename L >
  inline constexpr bool interval_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool interval_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() >= y.upper() );
  }

  template < typename R >
  inline constexpr bool interval_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x >= y.upper() );
  }

  template < typename L >
  inline constexpr bool interval_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= y );
  }

  // interval unordered
  template < typename L, typename R >
  inline constexpr bool interval_unordered( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( interval_less( x, y ) == false && interval_greater( x, y ) == false );
  }
  template < typename R >
  inline constexpr bool interval_unordered( typename interval<R>::value_type const&, interval<R> const& ) noexcept
  {
    return false;
  }
  template < typename L >
  inline constexpr bool interval_unordered( interval<L> const&, typename interval<L>::value_type const& ) noexcept
  {
    return false;
  }

  /*  Partial Ordering  */

  // partial less
  template < typename L, typename R >
  inline constexpr bool partial_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() < y.lower() && x.upper() < y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.lower() );
  }

  template < typename L >
  inline constexpr bool partial_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() < y );
  }

  template < typename L, typename R >
  inline constexpr bool partial_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() <= y.lower() && x.upper() <= y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.lower() );
  }

  template < typename L >
  inline constexpr bool partial_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() <= y );
  }

  // partial greater
  template < typename L, typename R >
  inline constexpr bool partial_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() > y.lower() && x.upper() > y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.upper() );
  }

  template < typename L >
  inline constexpr bool partial_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool partial_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() >= y.lower() && x.upper() >= y.upper() );
  }

  template < typename R >
  inline constexpr bool partial_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( std::forward<R>( x ) >= y.upper() );
  }

  template < typename L >
  inline constexpr bool partial_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= y );
  }

  // partial unordered
  template < typename L, typename R >
  inline constexpr bool partial_unordered( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( partial_less( x, y ) == false && partial_greater( x, y ) == false );
  }
  template < typename R >
  inline constexpr bool partial_unordered( typename interval<R>::value_type const&, interval<R> const& ) noexcept
  {
    return false;
  }
  template < typename L >
  inline constexpr bool partial_unordered( interval<L> const&, typename interval<L>::value_type const& ) noexcept
  {
    return false;
  }

  /*  Weak Ordering  */

  // weak less
  template < typename L, typename R >
  inline constexpr bool weak_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() < y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool weak_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() <= y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= std::forward<L>( y ) );
  }

  // weak greater
  template < typename L, typename R >
  inline constexpr bool weak_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() > y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() > y );
  }

  template < typename L, typename R >
  inline constexpr bool weak_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() >= y.lower() );
  }

  template < typename R >
  inline constexpr bool weak_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x >= y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() >= y );
  }

  // weak equal
  template < typename L, typename R >
  inline constexpr bool weak_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( weak_less( x, y ) == false && weak_less( y, x ) == false );
  }

  template < typename R >
  inline constexpr bool weak_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x == y.lower() );
  }

  template < typename L >
  inline constexpr bool weak_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() == std::forward<L>( y ) );
  }

  /*  Total Ordering  */

  // total less
  template < typename L, typename R >
  inline constexpr bool total_less( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() < y.upper() : x.lower() < y.lower();
  }

  template < typename R >
  inline constexpr bool total_less( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x < y.upper() );
  }

  template < typename L >
  inline constexpr bool total_less( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() < y );
  }

  template < typename L, typename R >
  inline constexpr bool total_less_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() <= y.upper() : x.lower() <= y.lower();
  }

  template < typename R >
  inline constexpr bool total_less_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x <= y.upper() );
  }

  template < typename L >
  inline constexpr bool total_less_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.lower() <= y );
  }

  // total greater
  template < typename L, typename R >
  inline constexpr bool total_greater( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() > y.upper() : x.lower() > y.lower();
  }

  template < typename R >
  inline constexpr bool total_greater( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x > y.lower() );
  }

  template < typename L >
  inline constexpr bool total_greater( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() > y );
  }

  template < typename L, typename R >
  inline constexpr bool total_greater_or_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return x.lower() == y.lower() ? x.upper() >= y.upper() : x.lower() >= y.lower();
  }

  template < typename R >
  inline constexpr bool total_greater_or_equal( typename interval<R>::value_type const& x, interval<R> const& y ) noexcept
  {
    return ( x >= y.lower() );
  }

  template < typename L >
  inline constexpr bool total_greater_or_equal( interval<L> const& x, typename interval<L>::value_type const& y ) noexcept
  {
    return ( x.upper() >= y );
  }

  // total equal
  template < typename L, typename R >
  inline constexpr bool total_equal( interval<L> const& x, interval<R> const& y ) noexcept
  {
    return ( x.lower() == y.lower() && x.upper() == y.upper() );
  }

  template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
  inline constexpr bool total_equal( R const& x, interval<L> const& y ) noexcept
  {
    return ( x == y.lower() && x == y.upper() );
  }

  template < typename L, typename R, std::enable_if_t<detail::is_interval_v<R>, std::nullptr_t>*& = nullptr >
  inline constexpr bool total_equal( interval<L> const& x, R const& y ) noexcept
  {
    return ( x.lower() == std::forward<L>( y ) && x.upper() == std::forward<L>( y ) );
  }

  //--------------------------//
  /*  Ordering Discriminator  */
  //--------------------------//

  /*  Interval Ordering  */

  template < typename L, typename R >
  inline Cranberries::interval_ordering interval_order( interval<L> const& x, interval<R> const& y ) noexcept
  {
    if ( interval_less( x, y ) )
    {
      return Cranberries::interval_ordering::less;
    }
    else if ( interval_greater( x, y ) )
    {
      return Cranberries::interval_ordering::greater;
    }
    return Cranberries::interval_ordering::unordered;
  }

  template < typename L, typename R >
  inline Cranberries::interval_ordering interval_order( R const& x, interval<L> const& y ) noexcept
  {
    if ( interval_less( x, y ) )
    {
      return Cranberries::interval_ordering::less;
    }
    else if ( interval_greater( x, y ) )
    {
      return Cranberries::interval_ordering::greater;
    }
    return Cranberries::interval_ordering::unordered;
  }

  template < typename L, typename R >
  inline Cranberries::interval_ordering interval_order( interval<L> const& x, R const& y ) noexcept
  {
    if ( interval_less( x, y ) )
    {
      return Cranberries::interval_ordering::less;
    }
    else if ( interval_greater( x, y ) )
    {
      return Cranberries::interval_ordering::greater;
    }
    return Cranberries::interval_ordering::unordered;
  }

  /*  Partial Ordering  */

  template < typename L, typename R >
  inline Cranberries::partial_ordering partial_order( interval<L> const& x, interval<R> const& y ) noexcept
  {
    if ( partial_less( x, y ) )
    {
      return partial_ordering::less;
    }
    else if ( partial_greater( x, y ) )
    {
      return partial_ordering::greater;
    }
    return partial_ordering::unordered;
  }

  template < typename L, typename R >
  inline Cranberries::partial_ordering partial_order( R const& x, interval<L> const& y ) noexcept
  {
    if ( partial_less( x, y ) )
    {
      return partial_ordering::less;
    }
    else if ( partial_greater( x, y ) )
    {
      return partial_ordering::greater;
    }
    return partial_ordering::unordered;
  }

  template < typename L, typename R >
  inline Cranberries::partial_ordering partial_order( interval<L> const& x, R const& y ) noexcept
  {
    if ( partial_less( x, y ) )
    {
      return partial_ordering::less;
    }
    else if ( partial_greater( x, y ) )
    {
      return partial_ordering::greater;
    }
    return partial_ordering::unordered;
  }

  /*  Weak ordering  */

  template < typename L, typename R >
  inline Cranberries::weak_ordering weak_order( interval<L> const& x, interval<R> const& y ) noexcept
  {
    if ( weak_less( x, y ) )
    {
      return weak_ordering::less;
    }
    else if ( weak_greater( x, y ) )
    {
      return weak_ordering::greater;
    }
    return weak_ordering::equivalent;
  }

  template < typename L, typename R >
  inline Cranberries::weak_ordering weak_order( R const& x, interval<L> const& y ) noexcept
  {
    if ( weak_less( x, y ) )
    {
      return weak_ordering::less;
    }
    else if ( weak_greater( x, y ) )
    {
      return weak_ordering::greater;
    }
    return weak_ordering::equivalent;
  }

  template < typename L, typename R >
  inline Cranberries::weak_ordering weak_order( interval<L> const& x, R const& y ) noexcept
  {
    if ( weak_less( x, y ) )
    {
      return weak_ordering::less;
    }
    else if ( weak_greater( x, y ) )
    {
      return weak_ordering::greater;
    }
    return weak_ordering::equivalent;
  }

  /*  Total Ordering  */

  template < typename L, typename R >
  inline Cranberries::total_ordering total_order( interval<L> const& x, interval<R> const& y ) noexcept
  {
    if ( total_less( x, y ) )
    {
      return total_ordering::less;
    }
    else if ( total_greater( x, y ) )
    {
      return total_ordering::greater;
    }
    return total_ordering::equal;
  }

  template < typename L, typename R >
  inline Cranberries::total_ordering total_order( R const& x, interval<L> const& y ) noexcept
  {
    if ( total_less( x, y ) )
    {
      return total_ordering::less;
    }
    else if ( total_greater( x, y ) )
    {
      return total_ordering::greater;
    }
    return total_ordering::equal;
  }

  template < typename L, typename R >
  inline Cranberries::total_ordering total_order( interval<L> const& x, R const& y ) noexcept
  {
    if ( total_less( x, y ) )
    {
      return total_ordering::less;
    }
    else if ( total_greater( x, y ) )
    {
      return total_ordering::greater;
    }
    return total_ordering::equal;
  }

  /*  interval compare function objrct  */
  template < order = order::Total >
  struct less
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return total_less( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return total_less( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return total_less( a, b );
    }
  };

  template < >
  struct less < order::Weak >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return weak_less( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return weak_less( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return weak_less( a, b );
    }
  };

  template < >
  struct less < order::Partial >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return partial_less( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return partial_less( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return partial_less( a, b );
    }
  };

  template < >
  struct less < order::Interval >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<L> const& b ) const noexcept
    {
      return interval_less( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return interval_less( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return interval_less( a, b );
    }
  };

  template < order = order::Total >
  struct less_or_equal
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const
    {
      return total_less_or_equal( a, b );
    }
    template < typename L, typename R >
    constexpr bool operator()( R const a, interval<L> const& b ) const noexcept
    {
      return total_less_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return total_less_or_equal( a, b );
    }
  };

  template < >
  struct less_or_equal < order::Weak >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return weak_less_or_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return weak_less_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return weak_less_or_equal( a, b );
    }
  };

  template < >
  struct less_or_equal < order::Partial >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return partial_less_or_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return partial_less_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return partial_less_or_equal( a, b );
    }
  };

  template < >
  struct less_or_equal < order::Interval >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<L> const& b ) const noexcept
    {
      return interval_less_or_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return interval_less_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return interval_less_or_equal( a, b );
    }
  };

  template < order = order::Total >
  struct greater
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return total_greater( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return total_greater( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return total_greater( a, b );
    }
  };

  template < >
  struct greater < order::Weak >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return weak_greater( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return weak_greater( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return weak_greater( a, b );
    }
  };

  template < >
  struct greater < order::Partial >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return partial_greater( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return partial_greater( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return partial_greater( a, b );
    }
  };

  template < >
  struct greater < order::Interval >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return interval_greater( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return interval_greater( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return interval_greater( a, b );
    }
  };

  template < order = order::Total >
  struct greater_or_equal
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return total_greater_or_equal( a, b );
    }
    template < typename L, typename R >
    constexpr bool operator()( R const a, interval<L> const& b ) const noexcept
    {
      return total_greater_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return total_greater_or_equal( a, b );
    }
  };

  template < >
  struct greater_or_equal < order::Weak >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return weak_greater_or_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return weak_greater_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return weak_greater_or_equal( a, b );
    }
  };

  template < >
  struct greater_or_equal < order::Partial >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return partial_greater_or_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return partial_greater_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return partial_greater_or_equal( a, b );
    }
  };

  template < >
  struct greater_or_equal < order::Interval >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<L> const& b ) const noexcept
    {
      return interval_greater_or_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return interval_greater_or_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return interval_greater_or_equal( a, b );
    }
  };

  template < order = order::Total >
  struct equal
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return total_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return total_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return total_equal( a, b );
    }
  };
  template < >
  struct equal < order::Weak >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return weak_equal( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const& a, interval<R> const& b ) const noexcept
    {
      return weak_equal( a, b );
    }
    template < typename L >
    constexpr bool operator()( interval<L> const& a, typename interval<L>::value_type const& b ) const noexcept
    {
      return weak_equal( a, b );
    }
  };
  template < order = order::Partial >
  struct undorderesd
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return partial_unordered( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const&, interval<R> const& ) const noexcept
    {
      return false;
    }
    template < typename L >
    constexpr bool operator()( interval<L> const&, typename interval<L>::value_type const& ) const noexcept
    {
      return false;
    }
  };
  template < >
  struct undorderesd < order::Interval >
  {
    template < typename L, typename R >
    constexpr bool operator()( interval<L> const& a, interval<R> const& b ) const noexcept
    {
      return interval_unordered( a, b );
    }
    template < typename R >
    constexpr bool operator()( typename interval<R>::value_type const&, interval<R> const& ) const noexcept
    {
      return false;
    }
    template < typename L >
    constexpr bool operator()( interval<L> const&, typename interval<L>::value_type const& ) const noexcept
    {
      return false;
    }
  };

  /*  is contain  */

  template < typename T >
  inline bool interval<T>::is_contain( interval<T> const& x ) const noexcept
  {
    return ( this->relational( x ) == interval_relation::contain );
  }

  template < typename T >
  inline bool interval<T>::is_contain( T const& x ) const noexcept
  {
    return ( this->lower() < x && this->upper() > x );
  }

  /*  is part of  */

  template < typename T >
  inline bool interval<T>::is_part_of( interval<T> const& x ) const noexcept
  {
    return ( this->relational( x ) == interval_relation::part_of );
  }

  /*  This function returns two interval subparts just split middle point as vector<interval<T>> */
  template < class T >
  inline std::pair< interval<T>, interval<T> > subpart( interval<T>& x ) noexcept
  {
    return std::make_pair( interval<T>{ x.lower(), x.mid() }, interval<T>{ x.mid(), x.upper() } );
  }

  template < class InputRange, class BinaryFunction >
  void adjacent_for_each( InputRange range, BinaryFunction f )
  {
    using std::begin;
    using std::end;

    auto& first = begin( range );
    auto& last = end( range );

    if ( first == last )
      return;

    while ( std::next( first ) != last )
    {
      auto const& a = *first;
      ++first;
      auto const& b = *first;
      f( a, b );
    }
  }

  template < class A, class Func >
  class Expr1
  {
    std::decay_t<detail::Expr_ref<A>> arg;
  public:
    void is_expr() {};
    typedef decltype( Func::Apply( arg[0] ) ) type;
    Expr1( A&& a )
      :arg( std::forward<A>( a ) ) {}
    template < typename T >
    constexpr operator interval<T>() const noexcept { return Func::Apply( arg[0] ); }
    auto operator[]( const long long ) noexcept ->decltype( Func::Apply( arg[0] ) )
    {
      return Func::Apply( arg[0] );
    }
    auto eval() noexcept ->decltype( Func::Apply( arg[0] ) ) const
    {
      return Func::Apply( arg[0] );
    }
  };

  template <class L, class Op, class R >
  class Expr2
  {
    std::decay_t<detail::Expr_ref<L>> l_;
    std::decay_t<detail::Expr_ref<R>> r_;
  public:
    void is_expr() {};
    Expr2( L&& l, R&& r )
      : l_( std::forward<L>( l ) ), r_( std::forward<R>( r ) ) {}
    template < typename T >
    operator interval<T>() noexcept { return Op::Apply( l_[0], r_[0] ); }
    auto operator[]( const long long ) noexcept ->decltype( Op::Apply( l_[0], r_[0] ) ) const
    {
      return Op::Apply( l_[0], r_[0] );
    }
    auto eval() noexcept ->decltype( Op::Apply( l_[0], r_[0] ) ) const
    {
      return Op::Apply( l_[0], r_[0] );
    }
  };

  template <class T1, class T2, class T3, class Func >
  class Expr3
  {
    std::decay_t<detail::Expr_ref<T1>> v1_;
    std::decay_t<detail::Expr_ref<T2>> v2_;
    std::decay_t<detail::Expr_ref<T3>> v3_;
  public:
    void is_expr() {};
    Expr3( T1&& v1, T2&& v2, T3&& v3 )
      : v1_( std::forward<T1>( v1 ) ), v2_( std::forward<T2>( v2 ) ), v3_( std::forward<T3>( v3 ) ) {}
    template < typename T >
    operator interval<T>() noexcept { return Func::Apply( v1_[0], v2_[0], v3_[0] ); }
    auto operator[]( const long long ) noexcept ->decltype( Func::Apply( v1_[0], v2_[0], v3_[0] ) ) const
    {
      return Func::Apply( v1_[0], v2_[0], v3_[0] );
    }
    auto eval() noexcept ->decltype( Func::Apply( v1_[0], v2_[0], v3_[0] ) ) const
    {
      return Func::Apply( v1_[0], v2_[0], v3_[0] );
    }
  };


  struct Add
  {
    template < typename L, typename R >
    static constexpr auto Apply( interval<L> const& l, interval<R> const& r ) noexcept
    {
      return add( l, r );
    }
    template < typename L >
    static constexpr auto Apply( interval<L> const& l, typename interval<L>::value_type const& r ) noexcept
    {
      return add( l, r );
    }
    template < typename R >
    static constexpr auto Apply( typename interval<R>::value_type l, interval<R> const& r ) noexcept
    {
      return add( std::forward<R>( l ), r );
    }
  };
  struct Sub
  {
    template < typename L, typename R >
    static constexpr auto Apply( interval<L> const& l, interval<R> const& r ) noexcept
    {
      return sub( l, r );
    }
    template < typename L >
    static constexpr auto Apply( interval<L> const& l, typename interval<L>::value_type const& r ) noexcept
    {
      return sub( l, r );
    }
    template < typename R >
    static constexpr auto Apply( typename interval<R>::value_type const& l, interval<R> const& r ) noexcept
    {
      return sub( l, r );
    }
  };
  struct Multi
  {
    template <typename L, typename R >
    static constexpr auto Apply( interval<L> const& l, interval<R> const& r ) noexcept
    {
      return multi( l, r );
    }
    template <typename L >
    static constexpr auto Apply( interval<L> const& l, typename interval<L>::value_type const& r ) noexcept
    {
      return multi( l, r );
    }
    template < typename R >
    static constexpr auto Apply( typename interval<R>::value_type const& l, interval<R> const& r ) noexcept
    {
      return multi( l, r );
    }
  };

  struct Div
  {
    template <typename L, typename R >
    static constexpr auto Apply( interval<L> const& l, interval<R> const& r ) noexcept
    {
      return div( l, r );
    }
    template <typename L >
    static constexpr auto Apply( interval<L> const& l, typename interval<L>::value_type const& r ) noexcept
    {
      return div( l, r );
    }
    template <typename R >
    static constexpr auto Apply( typename interval<R>::value_type const& l, interval<R> const& r ) noexcept
    {
      return div( l, r );
    }
  };

  struct Sin
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return sin( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::sin( x ) )
    {
      return std::sin( x );
    }

  };
  struct Cos
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return cos( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::cos( x ) )
    {
      return std::cos( x );
    }
  };
  struct Tan
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return tan( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::tan( x ) )
    {
      return std::tan( x );
    }
  };

  struct Asin
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return asin( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::asin( x ) )
    {
      return std::asin( x );
    }
  };

  struct Acos
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return acos( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::acos( x ) )
    {
      return std::acos( x );
    }
  };

  struct Atan
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return atan( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::atan( x ) )
    {
      return std::atan( x );
    }
  };

  struct Sinh
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return sinh( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::sinh( x ) )
    {
      return std::sinh( x );
    }
  };

  struct Cosh
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return cosh( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::cosh( x ) )
    {
      return std::cosh( x );
    }
  };

  struct Tanh
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return tanh( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::tanh( x ) )
    {
      return std::tanh( x );
    }
  };

  struct Asinh
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return asinh( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::asinh( x ) )
    {
      return std::asinh( x );
    }
  };

  struct Acosh
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return acosh( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::acosh( x ) )
    {
      return std::acosh( x );
    }
  };

  struct Atanh
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return atanh( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::atanh( x ) )
    {
      return std::atanh( x );
    }
  };

  struct Sqrt
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return sqrt( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::sqrt( x ) )
    {
      return std::sqrt( x );
    }
  };

  struct Cbrt
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> const& x ) noexcept
    {
      return cbrt( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::cbrt( x ) )
    {
      return std::cbrt( x );
    }
  };

  struct Pow
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x, int const& n ) noexcept
    {
      return pow( x, n );
    }
    template < typename T >
    static constexpr auto Apply( T const& x, int const& n ) noexcept ->decltype( std::pow( x, n ) )
    {
      return std::pow( x, n );
    }
  };

  struct Exp
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return exp( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::exp( x ) )
    {
      return std::exp( x );
    }
  };

  struct Exp2
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return exp2( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::exp2( x ) )
    {
      return std::exp2( x );
    }
  };

  struct Expm1
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return expm1( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::expm1( x ) )
    {
      return std::expm1( x );
    }
  };

  struct Abs
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return abs( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::abs( x ) )
    {
      return std::abs( x );
    }
  };

  struct Log
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return log( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::log( x ) )
    {
      return std::log( x );
    }
  };

  struct Log10
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return log10( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::log10( x ) )
    {
      return std::log10( x );
    }
  };

  struct Log2
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return log2( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::log2( x ) )
    {
      return std::log2( x );
    }
  };

  struct Log1p
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return log1p( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::log1p( x ) )
    {
      return std::log1p( x );
    }
  };

  struct Erf
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return erf( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::erf( x ) )
    {
      return std::erf( x );
    }
  };

  struct Erfc
  {
    template < typename T >
    static constexpr interval<T> Apply( interval<T> x ) noexcept
    {
      return erfc( x );
    }
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept ->decltype( std::erfc( x ) )
    {
      return std::erfc( x );
    }
  };
  struct Fma
  {
    template < typename T1, typename T2, typename T3 >
    static constexpr auto Apply( T1&& v1, T2&& v2, T3&& v3 ) noexcept ->decltype( fma( v1, v2, v3 ) )
    {
      return fma( std::forward<T1>( v1 ), std::forward<T2>( v2 ), std::forward<T3>( v3 ) );
    }
  };

  struct Max
  {
    template < typename L, typename R >
    static constexpr auto Apply( L&& v1, R&& v2 ) noexcept
    {
      return max( std::forward<L>( v1 ), std::forward<R>( v2 ) );
    }
  };


  struct Min
  {
    template < typename L, typename R >
    static constexpr auto Apply( L&& v1, R&& v2 ) noexcept
    {
      return min( std::forward<L>( v1 ), std::forward<R>( v2 ) );
    }
  };

  namespace Expression
  {
    template <class L, class R >
    inline constexpr Expr2<L, Add, R> add_( L&& lhs, R&& rhs ) noexcept
    {
      return Expr2<L, Add, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
    }
    template <class L, class R >
    inline constexpr Expr2<L, Sub, R> sub_( L&& lhs, R&& rhs ) noexcept
    {
      return Expr2<L, Sub, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
    }

    template <class L, class R >
    inline constexpr Expr2<L, Multi, R> multi_( L&& lhs, R&& rhs ) noexcept
    {
      return Expr2<L, Multi, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
    }

    template <class L, class R >
    inline constexpr Expr2<L, Div, R> div_( L&& lhs, R&& rhs ) noexcept
    {
      return Expr2<L, Div, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
    }

    template< class A >
    inline constexpr Expr1<A, Sin> sin_( A&& a ) noexcept
    {
      return Expr1<A, Sin>( std::forward<A>( a ) );
    }

    template< class A >
    inline constexpr Expr1<A, Cos> cos_( A&& a ) noexcept
    {
      return Expr1<A, Cos>( std::forward<A>( a ) );
    }

    template< class A >
    inline constexpr Expr1<A, Tan> tan_( A&& a ) noexcept
    {
      return Expr1<A, Tan>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Asin> asin_( A&& a ) noexcept
    {
      return Expr1<A, Asin>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Acos> acos_( A&& a ) noexcept
    {
      return Expr1<A, Acos>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Atan> atan_( A&& a ) noexcept
    {
      return Expr1<A, Atan>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Sinh> sinh_( A&& a ) noexcept
    {
      return Expr1<A, Sinh>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Cosh> cosh_( A&& a ) noexcept
    {
      return Expr1<A, Cosh>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Tanh> tanh_( A&& a ) noexcept
    {
      return Expr1<A, Tanh>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Asinh> asinh_( A&& a ) noexcept
    {
      return Expr1<A, Asinh>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Acosh> acosh_( A&& a ) noexcept
    {
      return Expr1<A, Acosh>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Atanh> atanh_( A&& a ) noexcept
    {
      return Expr1<A, Atanh>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Exp> exp_( A&& a ) noexcept
    {
      return Expr1<A, Exp>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Exp2> exp2_( A&& a ) noexcept
    {
      return Expr1<A, Exp2>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Expm1> expm1_( A&& a ) noexcept
    {
      return Expr1<A, Expm1>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Log> log_( A&& a ) noexcept
    {
      return Expr1<A, Log>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Log10> log10_( A&& a ) noexcept
    {
      return Expr1<A, Log10>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Log2> log2_( A&& a ) noexcept
    {
      return Expr1<A, Log2>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Log1p> log1p_( A&& a ) noexcept
    {
      return Expr1<A, Log1p>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Abs> abs_( A&& a ) noexcept
    {
      return Expr1<A, Abs>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Erf> erf_( A&& a ) noexcept
    {
      return Expr1<A, Erf>( std::forward<A>( a ) );
    }

    template < class A >
    inline constexpr Expr1<A, Erfc> erfc_( A&& a ) noexcept
    {
      return Expr1<A, Erfc>( std::forward<A>( a ) );
    }

    template< class L, class R >
    inline constexpr Expr2<L, Pow, R> pow_( L&& a, R&& n ) noexcept
    {
      return Expr2<L, Pow, R>( std::forward<L>( a ), std::forward<R>( n ) );
    }

    template< class T1, class T2, class T3 >
    inline constexpr auto fma_( T1&& v1, T2&& v2, T3&& v3 ) noexcept
    {
      return Expr3<T1, T2, T3, Fma>( std::forward<T1>( v1 ), std::forward<T2>( v2 ), std::forward<T3>( v3 ) );
    }

    template < typename L, typename R >
    inline constexpr auto max_( L&& v1, R&& v2 ) noexcept
    {
      return Expr2<L, Max, R>( std::forward<L>( v1 ), std::forward<R>( v2 ) );
    }

    template < typename L, typename R >
    inline constexpr auto min_( L&& v1, R&& v2 ) noexcept
    {
      return Expr2<L, Min, R>( std::forward<L>( v1 ), std::forward<R>( v2 ) );
    }

    template < typename T, typename ... Args >
    inline constexpr auto max_( T&& head, Args&&... args ) noexcept
    {
      return max_( std::forward<T>( head ), max_( std::forward<Args>( args )... ) );
    }

    template < typename T, typename ... Args >
    inline constexpr auto min_( T&& head, Args&&... args ) noexcept
    {
      return min_( std::forward<T>( head ), min_( std::forward<Args>( args )... ) );
    }
  }

}//end namespace Cranberries

#endif //!CRANBERRIES_INTERVAL
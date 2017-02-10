#ifndef CRANBERRIES_INTERVAL_LIB_INTERVAL_HPP
#define CRANBERRIES_INTERVAL_LIB_INTERVAL_HPP


#include <type_traits>
#include "rounding_control.hpp"
#include "./detail/detail.hpp"

namespace cranberries {
namespace interval_lib
{
  // Forward declaratoin for Ref
  template < typename T >
  struct Val;

  template <class T1, class T2, class T3, class Func >
  class Expr3;

  template < class L, class Op, class R >
  class Expr2;

  template < class A, class Func >
  class Expr1;


  //----------------------------------//
  /*                                  */
  /*     Class Declaration            */
  /*                                  */
  /*    Interval Class                */
  /*    (Body )                       */
  /*                                  */
  //----------------------------------//

  template < typename T = double >
  class interval : detail::concept::interval_tag
  {
    CRANBERRIES_CONCEPT_ASSERT( std::is_arithmetic<T>::value && std::is_signed<T>::value );
  public:
    /*  ctor  */
    interval() noexcept : pImpl{ std::make_unique<Impl>() } {};
    interval( T&&, T&& );
    interval( T const&, T const& );
    interval( T const& v ) noexcept : interval{ v, v } {};

    /*  copy ctor  */
    interval( interval const& ) noexcept;
    /*  move ctor  */
    interval( interval&& ) noexcept;
    /*  Copy Assignment Op  */
    interval& operator=( interval const& ) noexcept;
    /*  Move assignment Op  */
    interval& operator=( interval&& ) noexcept;

    /*  increment/decrement Op  */
    interval operator ++() noexcept;
    interval operator ++( int ) noexcept;
    interval operator --() noexcept;
    interval operator --( int ) noexcept;

    /*  numeric functions  */

    interval inverse() const;


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
    T lower() const& noexcept;
    T upper() const& noexcept;
    T& lower() & noexcept;
    T& upper() & noexcept;
    T lower() const&& = delete;
    T upper() const&& = delete;
    T&& lower() && noexcept;
    T&& upper() && noexcept;

    void swap( interval& x ) noexcept;

    friend std::ostream& operator<< (std::ostream& os, interval const& v) {
      return os << "[" << v.lower() << ", " << v.upper() << "]";
    }

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

    inline interval& operator+=( interval&& x ) {
      return add_assign( *this, x );
    }

    inline interval& operator-=( interval&& x ) {
      return subtract_assign( *this, x );
    }

    inline interval& operator*=( interval&& x ) {
      return multiply_assign( *this, x );
    }

    inline interval& operator/=( interval&& x ) {
      return divide_assign( *this, x );
    }
    /*  Member value type  */
    using value_type = T;
  private:
    /*  Inner Class  */
    class Impl;
    /*  Keep lower & upper bound value   */
    std::unique_ptr<Impl> pImpl;
  };

  //----------------------------------//
  /*                                  */
  /*     class declaration            */
  /*                                  */
  /*   interval Implementation class  */
  /*   ( internal class of interval)  */
  /*                                  */
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
      lower_bound = std::move( low );
      upper_bound = std::move( up );
    }
    /*  Increment/decrement Op Implementation */
    inline Impl operator ++() noexcept;
    inline Impl operator ++( int ) noexcept;
    inline Impl operator --() noexcept;
    inline Impl operator --( int ) noexcept;

    /*  getter Implementation  */
    inline T lower() const& noexcept;
    inline T upper() const& noexcept;
    inline T& lower() & noexcept;
    inline T& upper() & noexcept;
    inline T lower() const&& = delete;
    inline T upper() const&& = delete;
    inline T&& lower() && noexcept;
    inline T&& upper() && noexcept;

  private:
    /*  Lower Bound  */
    T lower_bound{};

    /*  Upper Bound  */
    T upper_bound{};
  };





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
  using namespace cranberries::weak_ordering::policy; // valid. only constants::one ordering_policy.
  y < x ; // OK! equals weal_less( y, x ) ;
  }
  {
  using namespace cranberries::weak_ordering::policy;
  using namespace cranberries::weak_ordering::policy; // invalid! Two or more ordering_policy.
  x < y ; // overload conflict occur.
  }
  - end example ]
  */




  //-------------------------//
  /*                         */
  /*   Interval Ctor/Dtor    */
  /*                         */
  //-------------------------//

  /*  Two value Arguments Ctor  */

  template < typename T >
  interval<T>::interval( T&& low, T&& up )
    : pImpl{ std::make_unique<Impl>() }
  {
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( up < low, "upper_bound less than lower_bound!" );
    pImpl->do_internal_work( std::move( low ), std::move( up ) );
  }

  template < typename T >
  interval<T>::interval( T const& low, T const& up )
    : pImpl{ std::make_unique<Impl>() }
  {
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( up < low, "upper_bound less than lower_bound!" );
    pImpl->do_internal_work( low, up );
  }


  /*  Move Ctor  */

  template < typename T >
  interval<T>::interval( interval&& x ) noexcept
    : pImpl( std::make_unique<Impl>() )
  {
    pImpl = std::move( x.pImpl );
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

  //--------------------------//
  /*                          */
  /*   Impl Member Function   */
  /*                          */
  //--------------------------//

  /*  Increment/Decrement Operators  */

  /*  Impl Prefix Increment Operator  */

  template < typename T >
  inline typename interval<T>::Impl interval<T>::Impl::operator ++() noexcept
  {
    ++lower_bound;
    ++upper_bound;
    return *this;
  }

  /*  Impl Postfix Increment Operator  */

  template < typename T >
  inline typename interval<T>::Impl interval<T>::Impl::operator ++( int ) noexcept
  {
    auto tmp( *this );
    ++lower_bound;
    ++upper_bound;
    return tmp;
  }

  /*  Impl Prefix Decrement Operator  */

  template < typename T >
  inline typename interval<T>::Impl interval<T>::Impl::operator --() noexcept
  {
    --lower_bound;
    --upper_bound;
    return *this;
  }

  /*  Impl Postfix Increment Operator  */

  template < typename T >
  inline typename interval<T>::Impl interval<T>::Impl::operator --( int ) noexcept
  {
    auto tmp( *this );
    --lower_bound;
    --upper_bound;
    return tmp;
  }

  /*  Accessors  */

  /*  Interval Low_Bound Getter  */

  template < typename T >
  inline T interval<T>::Impl::lower() const& noexcept
  {
    return lower_bound;
  }

  /*  Interval Upper_Bound Getter  */

  template < typename T >
  inline T interval<T>::Impl::upper() const& noexcept
  {
    return upper_bound;
  }

  template < typename T >
  inline T& interval<T>::Impl::lower() & noexcept
  {
    return lower_bound;
  }

  /*  Interval Upper_Bound Getter  */

  template < typename T >
  inline T& interval<T>::Impl::upper() & noexcept
  {
    return upper_bound;
  }

  template < typename T >
  inline T&& interval<T>::Impl::lower() && noexcept
  {
    return lower_bound;
  }

  template < typename T >
  inline T&& interval<T>::Impl::upper() && noexcept
  {
    return upper_bound;
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
  using namespace cranberries::nomal_accuracy;
  x - x //  returns [ -1.0L, 1.0L ]
  y - y //  returns [ -1.0L, 1.0L ]
  x / x //  returns [ 0.5L, 2.0L ]
  }
  - end example ]
  */




  //-------------------//
  /*  interval output  */
  //-------------------//

  //------------------------------------------------------------------------//
  /*  It is cumbersome to write the type every declaration.         */
  /*  Like ' auto x = interval<double>( 1, 3) '.              */
  /*  And it become source of mistakes.                   */
  /*  Therefore, you can use factory method 'hull( Low, Up )'.           */
  /*  It allows infer to type from arguments and returns interval object.   */
  //------------------------------------------------------------------------//


} // ! interval_lib
} // ! cranberries

#endif //!CRANBERRIES_INTERVAL
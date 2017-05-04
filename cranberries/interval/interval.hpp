#ifndef CRANBERRIES_INTERVAL_LIB_INTERVAL_HPP
#define CRANBERRIES_INTERVAL_LIB_INTERVAL_HPP


#include <type_traits>
#include "rounding_control.hpp"
#include "./cranberries_magic/detail.hpp"

namespace cranberries {
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
  class interval
    : cranberries_magic::interval_base
  {
    CRANBERRIES_CONCEPT_ASSERT( std::is_arithmetic<T>::value && std::is_signed<T>::value );
  public:
    using value_type = T;

    /*  ctor  */
    constexpr interval() noexcept : interval{ T{}, T{} } {};
    constexpr interval( T const&, T const& );
    constexpr interval( T const& v ) noexcept : interval{ v, v } {};
    constexpr interval( std::pair<T, T> p ) : interval{ p.first, p.second } {}

    /*  copy ctor  */
    constexpr interval( interval const& ) noexcept;
    /*  move ctor  */
    constexpr interval( interval&& ) noexcept;
    /*  Copy Assignment Op  */
    constexpr interval& operator=( interval const& ) noexcept;
    /*  Move assignment Op  */
    constexpr interval& operator=( interval&& ) noexcept;

    /*  increment/decrement Op  */
    constexpr interval operator ++() noexcept;
    constexpr interval operator ++( int ) noexcept;
    constexpr interval operator --() noexcept;
    constexpr interval operator --( int ) noexcept;

    /*  numeric functions  */
    constexpr interval inverse() const;

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
    constexpr T lower() const& noexcept;
    constexpr T upper() const& noexcept;
    constexpr T& lower() & noexcept;
    constexpr T& upper() & noexcept;
    constexpr T lower() const&& = delete;
    constexpr T upper() const&& = delete;
    constexpr T&& lower() && noexcept;
    constexpr T&& upper() && noexcept;

    constexpr void swap( interval& x ) noexcept;

    friend std::ostream& operator<< (std::ostream& os, interval const& v) {
      return os << "[" << v.lower_ << ", " << v.upper_ << "]";
    }


    template <class T1, class T2, class T3, class Func >
    inline constexpr interval& operator=( Expr3<T1, T2, T3, Func> x ) noexcept
    {
      ( *this ) = x.eval();
      return *this;
    }

    template <class L, class Op, class R >
    inline constexpr interval& operator=( Expr2<L, Op, R> x ) noexcept
    {
      ( *this ) = x.eval();
      return *this;
    }

    template <class A, class Func >
    inline constexpr interval& operator=( Expr1<A, Func> x ) noexcept
    {
      ( *this ) = x.eval();
      return *this;
    }

    template <class T1, class T2, class T3, class Func >
    inline constexpr interval& operator+=( Expr3<T1, T2, T3, Func> x ) noexcept
    {
      ( *this ) = *this + x.eval();
      return *this;
    }

    template <class A, class Func >
    inline constexpr interval& operator+=( Expr1<A, Func> x ) noexcept
    {
      *this = *this + x.eval();
      return *this;
    }

    template <class L, class Op, class R >
    inline constexpr interval& operator+=( Expr2<L, Op, R> x ) noexcept
    {
      *this = *this + x.eval();
      return *this;
    }

    inline constexpr interval& operator+=( interval&& x ) {
      return add_assign( *this, x );
    }

    inline constexpr interval& operator-=( interval&& x ) {
      return subtract_assign( *this, x );
    }

    inline constexpr interval& operator*=( interval&& x ) {
      return multiply_assign( *this, x );
    }

    inline constexpr interval& operator/=( interval&& x ) {
      return divide_assign( *this, x );
    }

  private:
    T lower_{};
    T upper_{};
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
  inline constexpr interval<T>::interval( T const& low, T const& up )
    : lower_{ low }, upper_{ up }
  {
    CRANBERRIES_INVALID_ARGUMENT_THROW_WITH_MSG_IF( up < low, "upper_bound less than lower_bound!" );
  }


  /*  Move Ctor  */

  template < typename T >
  inline constexpr interval<T>::interval( interval&& x ) noexcept
    : lower_{ std::move(x.lower_) }, upper_{ std::move(x.upper_) }
  {}

  /*  Copy Ctor  */

  template < typename T >
  inline constexpr interval<T>::interval( interval const& x ) noexcept
    : lower_{ x.lower_ }, upper_{ x.upper_ }
  {}

  /*  Copy Assignment Op  */

  template < typename T >
  inline constexpr interval<T>& interval<T>::operator=( interval<T> const& x ) noexcept
  {
    lower_ = x.lower_;
    upper_ = x.upper_;
    return *this;
  }

  /*  Move Assignment Op  */

  template < typename T >
  inline constexpr interval<T>& interval<T>::operator=( interval<T>&& x ) noexcept
  {
    lower_ = std::move(x.lower_);
    upper_ = std::move(x.upper_);
    return *this;
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

} // ! cranberries

#endif //!CRANBERRIES_INTERVAL
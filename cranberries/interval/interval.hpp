/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_INTERVAL_HPP
#define CRANBERRIES_INTERVAL_LIB_INTERVAL_HPP


#include <type_traits>
#include "rounding_control.hpp"
#include "./cranberries_magic/detail.hpp"

namespace cranberries {

  // TEMPLATE CLASS interval
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
    interval( interval const& ) = default;
    /*  move ctor  */
    interval( interval&& ) = default;
    /*  Copy Assignment Op  */
    interval& operator=( interval const& ) = default;
    /*  Move assignment Op  */
    interval& operator=( interval&& ) = default;

    /*  increment/decrement Op  */
    constexpr auto& operator ++() noexcept;
    constexpr auto operator ++( int ) noexcept;
    constexpr auto& operator --() noexcept;
    constexpr auto operator --( int ) noexcept;

    constexpr interval operator-() const noexcept;

    /*  numeric functions  */
    constexpr auto inverse() const;

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
		constexpr T&& lower() && noexcept;
		constexpr T&& upper() && noexcept;
		constexpr const T&& lower() const && noexcept;
		constexpr const T&& upper() const && noexcept;

    constexpr void swap( interval& x ) noexcept;

    friend std::ostream& operator<< (std::ostream& os, interval const& v) {
      return os << "[" << v.lower_ << ", " << v.upper_ << "]";
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
  "a < b" equals "weak_less( a, b )".
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
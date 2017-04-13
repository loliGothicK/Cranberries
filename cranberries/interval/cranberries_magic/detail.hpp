#ifndef CRANBERRIES_INTERVAL_LIB_DETAIL_HPP
#define CRANBERRIES_INTERVAL_LIB_DETAIL_HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <type_traits>



namespace cranberries {
namespace cranberries_magic {
namespace concept {

// For isinterval_v.
// This class is tag for identify that class T is interval.
// Class inheritancing interval_tag is interval.
// expr_tag similar to.
// Empty Base Optimization just works.
class interval_base {};
class expr_base {};

// is_ineterval_v definition
template < typename T >
constexpr bool is_interval_v = std::is_base_of<interval_base, T>::value;

// is_expr_v definition
template < typename T >
constexpr bool is_expr_v = std::is_base_of<expr_base, T>::value;

template < typename L, typename R, class=void>
struct is_available_total_order : std::false_type {};

template < typename L, typename R >
struct is_available_total_order<L,R,
	std::enable_if_t<std::is_same<decltype(total_less(std::declval<const L&>(), std::declval<const R&>())),bool>::value>
> : std::true_type {};

template < typename L, typename R >
constexpr bool is_available_total_order_v = is_available_total_order<L, R>::value;

} // ! concept
} // ! cranberries_magic
} // ! cranberries

namespace cranberries {

// for Expression Template.
// This class bind value.
// Applying eval() for Expression Template evaluation chain.
template < typename T >
struct Val : public cranberries_magic::concept::expr_base
{
  T value;
  Val( T v ) : value{ v } {}
  T eval() const { return value; }
};


namespace cranberries_magic {
  template < typename T >
#ifdef _MSC_VER
  __forceinline
#elif __GNUC__
  __attribute__( ( always_inline ) )
#elif __clang__
  __attribute__( ( always_inline ) )
#else
  inline
#endif
    constexpr bool is_overflow( T&& a ) noexcept
  {
    return std::abs( a ) == std::numeric_limits<std::decay_t<T>>::infinity()
      || std::abs( a ) == std::numeric_limits<std::decay_t<T>>::max();
  }

  template < typename T >
#ifdef _MSC_VER
  __forceinline
#elif __GNUC__
  __attribute__( ( always_inline ) )
#elif __clang__
  __attribute__( ( always_inline ) )
#else
  inline
#endif
    constexpr bool is_overflow( T&& a, T&& b ) noexcept
  {
    return std::abs( a ) == std::numeric_limits<std::decay_t<T>>::infinity()
      || std::abs( b ) == std::numeric_limits<std::decay_t<T>>::infinity()
      || std::abs( a ) == std::numeric_limits<std::decay_t<T>>::max()
      || std::abs( b ) == std::numeric_limits<std::decay_t<T>>::max();
  }

  // Expr primary class template.
  // Equivalent to Expr<T,false>.
  // Top level expression template bind values as wrapper class Val.
  template < typename T, std::size_t Label = concept::is_expr_v<T> >
  struct Expr
  {
    using Ref = Val<T>;
  };

  // Expr specialization template.
  // Expression template bind expression.
  template <typename T >
  struct Expr<T, true >
  {
    using Ref = T;
  };

  // Expr::Ref alias.
  template < typename T >
  using Expr_ref = typename Expr<T>::Ref;


} // ! cranberries_magic 
} // ! cranberries

#endif

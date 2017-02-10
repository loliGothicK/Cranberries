#ifndef CRANBERRIES_INTERVAL_LIB_DETAIL_HPP
#define CRANBERRIES_INTERVAL_LIB_DETAIL_HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <type_traits>


namespace cranberries
{
  namespace interval_lib
  {
    namespace detail
    {
      namespace concept
      {

        // For isinterval_v.
        // This class is tag for identifiy that class T is interval.
        // Class inheritancing interval_tag is interval.
        // expr_tag similar to.
        // Empty Base Optimization just works.
        class interval_tag {};
        class expr_tag {};

        // is_ineterval_v definition
        template < typename T >
        constexpr bool is_interval_v = std::is_base_of<interval_tag, T>::value;

        // is_expr_v definition
        template < typename T >
        constexpr bool is_expr_v = std::is_base_of<expr_tag, T>::value;


        struct is_available_total_order_impl
        {
          template < typename L, typename R >
          static auto check( L&& x, R&& y ) -> decltype( total_less( x, y ), std::true_type() );
          template < typename L, typename R >
          static std::false_type check( ... );
        };
        template < typename L, typename R >
        class is_available_total_order : public decltype( is_available_total_order_impl::check<L, R>( std::declval<L>(), std::declval<R>() ) ) {};

        template < typename L, typename R >
        constexpr bool is_available_total_order_v = is_available_total_order<L, R>::value;

      } // ! concept
    } // ! detail
  } // ! interval_lib
} // ! cranberries

namespace cranberries {
namespace interval_lib {

  // for Expression Template.
  // This class bind value.
  // Applying eval() for Expression Template evaluation chain.
  template < typename T >
  struct Val : public detail::concept::expr_tag
  {
    T value;
    Val( T v ) : value{ v } {}
    T eval() const { return value; }
  };


namespace detail
{
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


} // ! detail 
} // ! interval_lib
} // ! cranberries

#endif
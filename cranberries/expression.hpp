/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBRIIES_EXPRESSION_HPP
#define CRANBRIIES_EXPRESSION_HPP

#include <tuple>
#include <utility>
#include <cmath>
#include <type_traits>

namespace cranberries {
namespace expressions {
  struct expr_base {};

  template < typename T >
  constexpr bool is_expr_v = std::is_base_of<expr_base, T>::value;

  template < typename T >
  struct is_expr {
    static constexpr bool value = is_expr_v<T>;
  };

  // for Expression Template.
  // This class bind value.
  // Applying eval() for Expression Template evaluation chain.
  template < typename T >
  struct ValWrapper_ : public expr_base
  {
    T value;
    ValWrapper_(T v) noexcept : value{ v } {}
    T eval() const noexcept { return value; }
  };

  // expr_bind primary class template.
  // Equivalent to expr_bind<T,false>.
  // Top level expression template bind values as wrapper class ValWrapper_.
  template < typename T >
  struct expr_bind
  {
    using type = std::conditional_t<is_expr<T>::value, T, ValWrapper_<T>>;
  };

  template < typename T >
  using expr_bind_t = typename expr_bind<T>::type;


  template < class Func, class... Args >
  class ExprProxy_ : expr_base
  {
  private:
    template < class Tuple, size_t... I >
    static constexpr auto calc(Tuple&& t, std::index_sequence<I...>) {
      return Func::apply((std::get<I>(t).eval())...);
    }
  public:
    using result_t = decltype(calc(std::declval<std::tuple<expr_bind_t<Args>...>>(), std::index_sequence_for<Args...>{}));
  private:
    std::tuple<expr_bind_t<Args>...> args;

  public:
    template < class... A >
    ExprProxy_(A&& ... a) : args(std::forward<A>(a)...) {}

    constexpr auto eval() const {
      return calc(args, std::index_sequence_for<Args...>{});
    }

    template < class Target = result_t >
    constexpr operator Target() const { return eval(); }
  };


  struct  Add_
  {
    template <typename L, typename R >
    static constexpr auto apply( L&& l, R&& r )
    {
      return std::forward<L>( l ) + std::forward<R>( r );
    }
  };
  struct  Sub_
  {
    template <typename L, typename R >
    static constexpr auto apply( L&& l, R&& r )
    {
      return std::forward<L>( l ) - std::forward<R>( r );
    }
  };
  struct  Mul_
  {
    template <typename L, typename R >
    static constexpr auto apply( L&& l, R&& r )
    {
      return std::forward<L>( l ) * std::forward<R>( r );
    }
  };

  struct  Div_
  {
    template <typename L, typename R >
    static constexpr auto apply( L&& l, R&& r )
    {
      return std::forward<L>( l ) / std::forward<R>( r );
    }
  };

  struct  Sin_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::sin;
      return sin( x );
    }

  };
  struct  Cos_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::cos;
      return cos( x );
    }
  };
  struct  Tan_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::tan;
      return tan( x );
    }
  };

  struct  Asin_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::asin;
      return asin( x );
    }
  };

  struct  Acos_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::acos;
      return acos( x );
    }
  };

  struct  Atan_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::atan;
      return atan( x );
    }
  };

  struct  Sinh_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::sinh;
      return sinh( x );
    }
  };

  struct  Cosh_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::cosh;
      return cosh( x );
    }
  };

  struct  Tanh_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::tanh;
      return tanh( x );
    }
  };

  struct  Asinh_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::asinh;
      return asinh( x );
    }
  };

  struct  Acosh_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::acosh;
      return acosh( x );
    }
  };

  struct  Atanh_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::atanh;
      return atanh( x );
    }
  };

  struct  Sqrt_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::sqrt;
      return sqrt( x );
    }
  };

  struct  Cbrt_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::cbrt;
      return cbrt( x );
    }
  };

  struct  Pow_
  {
    template < typename T >
    static constexpr auto apply( interval<T> x, double n )
    {
      using std::pow;
      return pow( x, n );
    }
  };

  struct  Exp_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::exp;
      return exp( x );
    }
  };

  struct  Exp2_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::exp2;
      return exp2( x );
    }
  };

  struct  Expm1_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::expm1;
      return expm1( x );
    }
  };

  struct  Abs_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::abs;
      return abs( x );
    }
  };

  struct  Log_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::log;
      return log( x );
    }
  };

  struct  Log10_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::log10;
      return log10( x );
    }
  };

  struct  Log2_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::log2;
      return log2( x );
    }
  };

  struct  Log1p_
  {
    template < typename T >
    static constexpr auto apply( T const& x )
    {
      using std::log1p;
      return log1p( x );
    }
  };

  struct  Erf_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::erf;
      return erf( x );
    }
  };

  struct  Erfc_
  {
    template < typename T >
    static constexpr auto apply( T const& x ) noexcept
    {
      using std::erfc;
      return erfc( x );
    }
  };
  struct  Fma_
  {
    template < typename T1, typename T2, typename T3 >
    static constexpr auto apply( T1&& v1, T2&& v2, T3&& v3 ) noexcept
    {
      return fma( std::forward<T1>( v1 ), std::forward<T2>( v2 ), std::forward<T3>( v3 ) );
    }
  };

  struct  Max_
  {
    template < typename L, typename R >
    static constexpr auto apply( L&& v1, R&& v2 ) noexcept
    {
      return max( std::forward<L>( v1 ), std::forward<R>( v2 ) );
    }
  };


  struct  Min_
  {
    template < typename L, typename R >
    static constexpr auto apply( L&& v1, R&& v2 ) noexcept
    {
      return min( std::forward<L>( v1 ), std::forward<R>( v2 ) );
    }
  };

} // ! - end namespace expressionss

  template <class L, class R >
  inline constexpr expressions::ExprProxy_<expressions::Add_, L, R> add_( L&& lhs, R&& rhs ) noexcept
  {
    return { std::forward<L>(lhs), std::forward<R>(rhs) };
  }
  template <class L, class R >
  inline constexpr expressions::ExprProxy_<expressions::Sub_, L, R> sub_( L&& lhs, R&& rhs ) noexcept
  {
    return { std::forward<L>(lhs), std::forward<R>(rhs) };
  }

  template <class L, class R >
  inline constexpr expressions::ExprProxy_<expressions::Mul_, L, R> mul_( L&& lhs, R&& rhs ) noexcept
  {
    return { std::forward<L>(lhs), std::forward<R>(rhs) };
  }

  template <class L, class R >
  inline constexpr expressions::ExprProxy_<expressions::Div_, L, R> div_( L&& lhs, R&& rhs ) noexcept
  {
    return { std::forward<L>(lhs), std::forward<R>(rhs) };
  }

  template< class A >
  inline constexpr expressions::ExprProxy_<expressions::Sin_, A> sin_( A&& a ) noexcept
  {
    return { std::forward<A>(a) };
  }

  template< class A >
  inline constexpr expressions::ExprProxy_<expressions::Cos_, A> cos_( A&& a ) noexcept
  {
    return { std::forward<A>(a) };
  }

  template< class A >
  inline constexpr expressions::ExprProxy_<expressions::Tan_, A> tan_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Asin_, A> asin_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Acos_, A> acos_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Atan_, A> atan_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Sinh_, A> sinh_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Cosh_, A> cosh_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Tanh_, A> tanh_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Asinh_, A> asinh_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Acosh_, A> acosh_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Atanh_, A> atanh_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Exp_, A> exp_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Exp2_, A> exp2_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Expm1_, A> expm1_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Log_, A> log_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Log10_, A> log10_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Log2_, A> log2_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Log1p_, A> log1p_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Abs_, A> abs_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Erf_, A> erf_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template < class A >
  inline constexpr expressions::ExprProxy_<expressions::Erfc_, A> erfc_( A&& a ) noexcept
  {
    return { std::forward<A>( a ) };
  }

  template< class L, class R >
  inline constexpr expressions::ExprProxy_<expressions::Pow_, L, R> pow_( L&& a, R&& n ) noexcept
  {
    return { std::forward<L>( a ), std::forward<R>( n ) };
  }

  template< class T1, class T2, class T3 >
  inline constexpr expressions::ExprProxy_<expressions::Fma_, T1, T2, T3> fma_( T1&& v1, T2&& v2, T3&& v3 ) noexcept
  {
    return { std::forward<T1>( v1 ), std::forward<T2>( v2 ), std::forward<T3>( v3 ) };
  }

  template < typename L, typename R >
  inline constexpr expressions::ExprProxy_<expressions::Max_, L, R> max_( L&& v1, R&& v2 ) noexcept
  {
    return { std::forward<L>( v1 ), std::forward<R>( v2 ) };
  }

  template < typename L, typename R >
  inline constexpr expressions::ExprProxy_<expressions::Min_, L, R> min_( L&& v1, R&& v2 ) noexcept
  {
    return { std::forward<L>( v1 ), std::forward<R>( v2 ) };
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



} // ! namespace cranberries

#endif
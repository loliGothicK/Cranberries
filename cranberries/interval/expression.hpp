#ifndef CRANBRIIES_INTERVAL_LIB_EXPRESSION_HPP
#define CRANBRIIES_INTERVAL_LIB_EXPRESSION_HPP

#include "interval.hpp"
#include "cranberries_magic/detail.hpp"

namespace cranberries {

  template < class A, class Func >
  class Expr1 : cranberries_magic::concept::expr_base
  {
    std::decay_t<cranberries_magic::Expr_ref<A>> arg;
  public:
    Expr1( A const& a )
      :arg{ a } {}
    template < typename T >
    constexpr operator interval<T>() const { return Func::Apply( arg.eval() ); }
    auto eval() const
    {
      return Func::Apply( arg.eval() );
    }
  };

  template <class L, class Op, class R >
  class Expr2 : cranberries_magic::concept::expr_base
  {
    std::decay_t<cranberries_magic::Expr_ref<L>> l_;
    std::decay_t<cranberries_magic::Expr_ref<R>> r_;
  public:
    Expr2( L const& l, R const& r )
      : l_{ l }, r_{ r } {}
    template < typename T >
    operator interval<T>() const { return Op::Apply( l_.eval(), r_.eval() ); }
    auto eval() const
    {
      return Op::Apply( l_.eval(), r_.eval() );
    }
  };

  template <class T1, class T2, class T3, class Func >
  class Expr3 : cranberries_magic::concept::expr_base
  {
    std::decay_t<cranberries_magic::Expr_ref<T1>> v1_;
    std::decay_t<cranberries_magic::Expr_ref<T2>> v2_;
    std::decay_t<cranberries_magic::Expr_ref<T3>> v3_;
  public:
    Expr3( T1 const& v1, T2 const& v2, T3 const& v3 )
      : v1_{ v1 }, v2_{ v2 }, v3_{ v3 } {}
    template < typename T >
    operator interval<T>() const { return Func::Apply( v1_.eval(), v2_.eval(), v3_.eval() ); }
    auto eval() const
    {
      return Func::Apply( v1_.eval(), v2_.eval(), v3_.eval() );
    }
  };


  struct Add
  {
    template <typename L, typename R >
    static constexpr auto Apply( L&& l, R&& r )
    {
      return add( std::forward<L>( l ), std::forward<R>( r ) );
    }
  };
  struct Sub
  {
    template <typename L, typename R >
    static constexpr auto Apply( L&& l, R&& r )
    {
      return subtract( std::forward<L>( l ), std::forward<R>( r ) );
    }
  };
  struct Multi
  {
    template <typename L, typename R >
    static constexpr auto Apply( L&& l, R&& r )
    {
      return multiply( std::forward<L>( l ), std::forward<R>( r ) );
    }
  };

  struct Div
  {
    template <typename L, typename R >
    static constexpr auto Apply( L&& l, R&& r )
    {
      return divide( std::forward<L>( l ), std::forward<R>( r ) );
    }
  };

  struct Sin
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::sin;
      return sin( x );
    }

  };
  struct Cos
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::cos;
      return cos( x );
    }
  };
  struct Tan
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::tan;
      return tan( x );
    }
  };

  struct Asin
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::asin;
      return asin( x );
    }
  };

  struct Acos
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::acos;
      return acos( x );
    }
  };

  struct Atan
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::atan;
      return atan( x );
    }
  };

  struct Sinh
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::sinh;
      return sinh( x );
    }
  };

  struct Cosh
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::cosh;
      return cosh( x );
    }
  };

  struct Tanh
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::tanh;
      return tanh( x );
    }
  };

  struct Asinh
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::asinh;
      return asinh( x );
    }
  };

  struct Acosh
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::acosh;
      return acosh( x );
    }
  };

  struct Atanh
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::atanh;
      return atanh( x );
    }
  };

  struct Sqrt
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::sqrt;
      return sqrt( x );
    }
  };

  struct Cbrt
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::cbrt;
      return cbrt( x );
    }
  };

  struct Pow
  {
    template < typename T >
    static constexpr auto Apply( interval<T> x, double n )
    {
      using std::pow;
      return pow( x, n );
    }
  };

  struct Exp
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::exp;
      return exp( x );
    }
  };

  struct Exp2
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::exp2;
      return exp2( x );
    }
  };

  struct Expm1
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::expm1;
      return expm1( x );
    }
  };

  struct Abs
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::abs;
      return abs( x );
    }
  };

  struct Log
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::log;
      return log( x );
    }
  };

  struct Log10
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::log10;
      return log10( x );
    }
  };

  struct Log2
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::log2;
      return log2( x );
    }
  };

  struct Log1p
  {
    template < typename T >
    static constexpr auto Apply( T const& x )
    {
      using std::log1p;
      return log1p( x );
    }
  };

  struct Erf
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::erf;
      return erf( x );
    }
  };

  struct Erfc
  {
    template < typename T >
    static constexpr auto Apply( T const& x ) noexcept
    {
      using std::erfc;
      return erfc( x );
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

  template <class L, class R >
  inline constexpr Expr2<L, Add, R> add_( L&& lhs, R&& rhs ) noexcept
  {
    return Expr2<L, Add, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
  }
  template <class L, class R >
  inline constexpr Expr2<L, Sub, R> subtract_( L&& lhs, R&& rhs ) noexcept
  {
    return Expr2<L, Sub, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
  }

  template <class L, class R >
  inline constexpr Expr2<L, Multi, R> multiply_( L&& lhs, R&& rhs ) noexcept
  {
    return Expr2<L, Multi, R>( std::forward<L>( lhs ), std::forward<R>( rhs ) );
  }

  template <class L, class R >
  inline constexpr Expr2<L, Div, R> divide_( L&& lhs, R&& rhs ) noexcept
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

} // ! namespace cranberries

#endif
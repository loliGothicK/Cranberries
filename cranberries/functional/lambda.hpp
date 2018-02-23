/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBRIIES_FUNCTIONAL_LAMBDA_EXPRESSION_HPP
#define CRANBRIIES_FUNCTIONAL_LAMBDA_EXPRESSION_HPP
#include <tuple>
#include <utility>
#include <cmath>
#include "../common/concepts.hpp"
#include "./bind.hpp"
#undef min
#undef max
namespace cranberries {

	template < class T, class = void >
	struct is_expr: std::false_type {};

	template < class T >
	struct is_expr<T, cranberries::void_t<typename remove_cvr_t<T>::is_expression>> : std::true_type {};

	template < class T >
	constexpr bool is_expr_v = is_expr<T>::value;

	template < class T >
	using is_expression = cranberries::disjunction<std::is_bind_expression<T>, is_expr<T>, cranberries::is_bind_expression<T>>;

	// for Expression Template.
	// This class bind value.
	// Applying eval() for Expression Template evaluation chain.
	template < typename T >
	struct ValWrapper_
	{
		using is_expression = std::nullptr_t;

		T value;

		ValWrapper_(T v) : value{ v } {}
		constexpr decltype(auto) eval(...) const noexcept { return value; }
	};


	template < size_t N >
	struct ValWrapper_<placeholder_t<N>>
	{
		using is_expression = std::nullptr_t;

		constexpr ValWrapper_(placeholder_t<N>) {}
		template < class Tuple >
		constexpr decltype(auto) eval(Tuple&& t) const noexcept { return std::get<N>(std::forward<Tuple>(t)); }
	};

	template < class T >
	auto value_(T&& v) {
		return ValWrapper_<T>{ std::forward<T>(v) };
	}


	// expr_bind primary class template.
	// Equivalent to expr_bind<T,false>.
	// Top level expression template bind values as wrapper class ValWrapper_.
	template < typename T >
	struct expr_bind
	{
		using type = std::conditional_t<is_expr<T>::value, T, ValWrapper_<std::decay_t<T>>>;
	};

	template < typename T >
	using expr_bind_t = typename expr_bind<T>::type;

	template < class, class=void >
	struct _index: size_constant<0> {};

	template < size_t I >
	struct _index<placeholder_t<I>>: size_constant<I> {};

	template < class T >
	struct _index<T, std::enable_if_t<is_expr_v<remove_cvr_t<T>> && !is_placeholder_v<remove_cvr_t<T>>>>: size_constant<remove_cvr_t<T>::value> {};

	template < class T >
	constexpr size_t _index_v = _index<T>::value;

	template < class Func, class... Args >
	class LambdaExpression
		: public pack_max<size_pack<_index_v<Args>...>>
	{
		template < class Accept, class Tuple, size_t... I >
		static constexpr auto calc(Accept&& ac, Tuple&& t, std::index_sequence<I...>) {
			return Func::apply((std::get<I>(std::forward<Tuple>(t)).eval( std::forward<Accept>(ac) ))...);
		}
		std::tuple<expr_bind_t<Args>...> args;
	public:
		using is_expression = std::nullptr_t;

		constexpr LambdaExpression(expr_bind_t<Args>... a) : args{ a... } {}
		constexpr LambdaExpression(const LambdaExpression&) = default;
		constexpr LambdaExpression& operator=(const LambdaExpression&) = default;
		constexpr LambdaExpression(LambdaExpression&&) = default;
		constexpr LambdaExpression& operator=(LambdaExpression&&) = default;

		template < class AcceptArgs >
		constexpr auto eval(AcceptArgs&& ac) const {
			return calc(std::forward<AcceptArgs>(ac), args, std::index_sequence_for<Args...>{});
		}

		template < class... AcceptArgs >
		constexpr auto operator()(AcceptArgs&&... ac) const {
			return calc(std::forward_as_tuple(std::forward<AcceptArgs>(ac)...), args, std::index_sequence_for<Args...>{});
		}
	};
namespace lambda_operations {
	struct Tuple_
	{
		template <typename... Ts >
		static constexpr auto apply(Ts&&... xs)
		{
			return std::make_tuple(std::forward<Ts>(xs)...);
		}
	};
}

namespace lambda_operations {
	struct RefletSelf_
	{
		template <class Self, typename... Ts >
		static constexpr auto apply(Self&& self, Ts&&... xs)
		{
			return std::forward<Self>(self)( std::forward<Ts>(xs)... );
		}
	};
}


namespace lambda_operations {

	struct Modulo_ {
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) % std::forward<R>(r);
		}
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

	struct  BitShiftLeft_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) >> std::forward<R>(r);
		}
	};

	struct  BitShiftRight_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) << std::forward<R>(r);
		}
	};

	struct  LT_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) < std::forward<R>(r);
		}
	};

	struct  LE_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) <= std::forward<R>(r);
		}
	};

	struct  GT_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) > std::forward<R>(r);
		}
	};

	struct  GE_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) <= std::forward<R>(r);
		}
	};

	struct  EQ_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) == std::forward<R>(r);
		}
	};

	struct  NE_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) != std::forward<R>(r);
		}
	};


	struct  BitAnd_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) & std::forward<R>(r);
		}
	};

	struct  Xor_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) ^ std::forward<R>(r);
		}
	};

	struct  BitOr_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) | std::forward<R>(r);
		}
	};

	struct  LogicalAnd_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) && std::forward<R>(r);
		}
	};


	struct  LogicalOr_
	{
		template <typename L, typename R >
		static constexpr auto apply(L&& l, R&& r)
		{
			return std::forward<L>(l) || std::forward<R>(r);
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
		static constexpr auto apply( T const& x, double n )
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

	struct IfThenElse
	{
		template < typename T1, typename T2 >
		static constexpr auto apply(bool b, T1&& v1, T2&& v2) noexcept
		{
			return b ? std::forward<T1>(v1) : std::forward<T2>(v2);
		}
	};
	
} // ! end namespace lambda_operations

	constexpr decltype(auto) _check(const placeholder_t<0>&, const placeholder_t<0>&) {
		return std::make_tuple(placeholder_t<1>{}, placeholder_t<2>{});
	}

	template < class L,	enabler_t<has_value_field_v<remove_cvr_t<L>>> = nullptr >
	constexpr decltype(auto) _check(L&& l, const placeholder_t<0>&) {
		return std::make_tuple(std::forward<L>(l), placeholder_t<remove_cvr_t<L>::value+1>{});
	}

	template < class L,	enabler_t<!has_value_field_v<remove_cvr_t<L>>> = nullptr >
	constexpr decltype(auto) _check(L&& l, const placeholder_t<0>&) {
		return std::make_tuple(std::forward<L>(l), placeholder_t<1>{});
	}

	template < class R, enabler_t<has_value_field_v<remove_cvr_t<R>>> = nullptr>
	constexpr decltype(auto) _check(const placeholder_t<0>&, R&& r) {
		return std::make_tuple(placeholder_t<remove_cvr_t<R>::value+1>{}, std::forward<R>(r));
	}

	template < class R, enabler_t<!has_value_field_v<remove_cvr_t<R>>> = nullptr>
	constexpr decltype(auto) _check(const placeholder_t<0>&, R&& r) {
		return std::make_tuple(placeholder_t<1>{}, std::forward<R>(r));
	}

	template < class L, class R,
		enabler_t<!std::is_same<remove_cvr_t<L>, placeholder_t<0>>::value> = nullptr,
		enabler_t<!std::is_same<remove_cvr_t<R>, placeholder_t<0>>::value> = nullptr >
	constexpr decltype(auto) _check(L&& l, R&& r) {
		return std::make_tuple(std::forward<L>(l), std::forward<R>(r));
	}

	template < class A >
	constexpr decltype(auto) _check(A&& a) {
		return std::forward<A>(a);
	}

	constexpr decltype(auto) _check(const placeholder_t<0>&) {
		return placeholder_t<1>{};
	}

	template < class Op, class Arg, enabler_t<!is_tuple_v<remove_cvr_t<Arg>>> = nullptr >
	constexpr auto construct_lambda(Arg&& arg) {
		return LambdaExpression<Op, Arg>{ std::forward<Arg>(arg) };
	}

	template < class Op, class... Args >
	constexpr auto construct_lambda_from_tuple(std::tuple<Args...> t) {
		return cranberries::construct_from_tuple<LambdaExpression<Op, Args...>>(t);
	}

	template < class Op, class Tuple, enabler_t<is_tuple_v<remove_cvr_t<Tuple>>> = nullptr >
	constexpr auto construct_lambda(Tuple t) {
		return construct_lambda_from_tuple<Op>(t);
	}

	template <class L, class R >
	constexpr auto mod_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::Modulo_>(
			_check(std::forward<L>(lhs), std::forward<R>(rhs))
			);
	}

	template <class L, class R >
	constexpr auto shiftl_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::BitShiftLeft_>(
			_check(std::forward<L>(lhs), std::forward<R>(rhs))
			);
	}

	template <class L, class R >
	constexpr auto shiftr_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::BitShiftRight_>(
			_check(std::forward<L>(lhs), std::forward<R>(rhs))
			);
	}



	template <class L, class R >
	constexpr auto  lt_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::LT_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  le_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::LE_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  gt_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::GT_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  ge_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::GE_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  eq_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::EQ_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  ne_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::NE_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  bitand_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::BitAnd_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  xor_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::Xor_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  bitor_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::BitOr_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  and_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::LogicalAnd_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  or_(L&& lhs, R&& rhs) noexcept
	{
		return construct_lambda<lambda_operations::LogicalOr_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  add_( L&& lhs, R&& rhs ) noexcept
	{
		return construct_lambda<lambda_operations::Add_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}
	template <class L, class R >
	constexpr auto  sub_( L&& lhs, R&& rhs ) noexcept
	{
		return construct_lambda<lambda_operations::Sub_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  mul_( L&& lhs, R&& rhs ) noexcept
	{
		return construct_lambda<lambda_operations::Mul_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template <class L, class R >
	constexpr auto  div_( L&& lhs, R&& rhs ) noexcept
	{
		return construct_lambda<lambda_operations::Div_>(_check(std::forward<L>(lhs), std::forward<R>(rhs)));
	}

	template< class A >
	constexpr auto  sqrt_(A&& a) noexcept
	{
		return construct_lambda<lambda_operations::Sqrt_>(_check(std::forward<A>(a)));
	}

	template< class A >
	constexpr auto  cbrt_(A&& a) noexcept
	{
		return construct_lambda<lambda_operations::Cbrt_>(_check(std::forward<A>(a)));
	}

	template< class A >
	constexpr auto  sin_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Sin_>(_check(std::forward<A>(a)));
	}

	template< class A >
	constexpr auto  cos_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Cos_>(_check(std::forward<A>(a)));
	}

	template< class A >
	constexpr auto  tan_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Tan_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  asin_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Asin_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  acos_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Acos_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  atan_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Atan_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  sinh_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Sinh_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  cosh_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Cosh_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  tanh_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Tanh_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  asinh_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Asinh_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  acosh_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Acosh_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  atanh_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Atanh_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  exp_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Exp_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  exp2_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Exp2_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  expm1_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Expm1_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  log_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Log_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  log10_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Log10_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  log2_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Log2_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  log1p_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Log1p_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  abs_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Abs_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  erf_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Erf_>(_check(std::forward<A>( a )));
	}

	template < class A >
	constexpr auto  erfc_( A&& a ) noexcept
	{
		return construct_lambda<lambda_operations::Erfc_>(_check(std::forward<A>( a )));
	}

	template< class L, class R >
	constexpr auto  pow_( L&& a, R&& n ) noexcept
	{
		return construct_lambda<lambda_operations::Pow_>(_check(std::forward<L>( a ), std::forward<R>( n )));
	}

	template< class T1, class T2, class T3 >
	constexpr auto  fma_( T1&& v1, T2&& v2, T3&& v3 ) noexcept
	{
		return construct_lambda<lambda_operations::Fma_>(_check(std::forward<T1>( v1 ), std::forward<T2>( v2 ), std::forward<T3>( v3 )));
	}

	template < typename L, typename R >
	constexpr auto  max_( L&& v1, R&& v2 ) noexcept
	{
		return construct_lambda<lambda_operations::Max_>(_check(std::forward<L>( v1 ), std::forward<R>( v2 )));
	}

	template < typename L, typename R >
	constexpr auto  min_( L&& v1, R&& v2 ) noexcept
	{
		return construct_lambda<lambda_operations::Min_>(_check(std::forward<L>( v1 ), std::forward<R>( v2 )));
	}

	template < typename T, typename ... Args >
	constexpr auto max_( T&& head, Args&&... args ) noexcept
	{
		return max_( std::forward<T>( head ), max_( std::forward<Args>( args )... ) );
	}

	template < typename T, typename ... Args >
	constexpr auto min_( T&& head, Args&&... args ) noexcept
	{
		return min_( std::forward<T>( head ), min_( std::forward<Args>( args )... ) );
	}

	template < typename... Ts >
	constexpr LambdaExpression<lambda_operations::Tuple_, Ts...>
	tuple_(Ts&&... xs) noexcept
	{
		return { std::forward<Ts>(xs)... };
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator % (T&& t, U&& u) {
		return ::cranberries::mod_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator << (T&& t, U&& u) {
		return ::cranberries::shiftl_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr,
		concepts::concept_required_not<T, concepts::iterable> = nullptr >
	constexpr auto operator >> (T&& t, U&& u) {
		return ::cranberries::shiftr_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator < (T&& t, U&& u) {
		return ::cranberries::lt_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator <= (T&& t, U&& u) {
		return ::cranberries::le_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator > (T&& t, U&& u) {
		return ::cranberries::gt_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator >= (T&& t, U&& u) {
		return ::cranberries::ge_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator == (T&& t, U&& u) {
		return ::cranberries::eq_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator != (T&& t, U&& u) {
		return ::cranberries::ne_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator & (T&& t, U&& u) {
		return ::cranberries::bitand_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator ^ (T&& t, U&& u) {
		return ::cranberries::xor_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, enabler_t<
		negation_v<concepts::required<std::decay_t<U>, concepts::iterable>>
		> = nullptr,
		enabler_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>> >> = nullptr >
	constexpr auto operator | (T&& t, U&& u) {
		return ::cranberries::bitor_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator && (T&& t, U&& u) {
		return ::cranberries::and_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
		constexpr auto operator || (T&& t, U&& u) {
		return ::cranberries::or_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator + (T&& t, U&& u) {
		return ::cranberries::add_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator - (T&& t, U&& u) {
		return ::cranberries::sub_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator * (T&& t, U&& u) {
		return ::cranberries::mul_(std::forward<T>(t), std::forward<U>(u));
	}

	template < class T, class U, std::enable_if_t<disjunction_v<
		is_expr<std::decay_t<T>>, is_placeholder<std::decay_t<T>>,
		is_expr<std::decay_t<U>>, is_placeholder<std::decay_t<U>>>, std::nullptr_t> = nullptr >
	constexpr auto operator / (T&& t, U&& u) {
		return ::cranberries::div_(std::forward<T>(t), std::forward<U>(u));
	}



namespace magic_arts {

	template < class... Elem >
	struct TupleDecomposerProxy {
		std::tuple<Elem...> elements;
		template < class T >
		constexpr TupleDecomposerProxy<Elem..., T>
			operator, (T&& t) {
			return TupleDecomposerProxy<Elem..., T>{{ cranberries::back_magic::make_tuple_cat(std::move(elements), std::forward<T>(t)) }};
		}
	};

	template < class... Elem >
	struct TupleMatchrProxy {
		std::tuple<Elem...> elements;
	};

	template < class >
	class is_ret : std::false_type {};

	template < >
	class is_ret<placeholder_t<0>> : std::false_type {};

	template < size_t N >
	class is_ret<placeholder_t<N>> : std::true_type {};

	template < class, class >
	struct ret_seq_for;

	template < class Pack, size_t... I >
	struct ret_seq_for<Pack, std::index_sequence<I...>> {
		template < size_t N >
		using rip = std::conditional_t< std::is_same<pack_element_t<N, Pack>, placeholder_t<0>>::value, size_constant<0>, placeholder_t<N>>;
		using type
			= pack_t2v_transform_t<
					bind_1st<_index>::expr,
					pack_remove_t<
						size_constant<0>,
						type_pack< rip<I>... >
					>
				>;
	};


	template < size_t, class >
	struct TupleDecompose_;
	
	template < size_t N, size_t... Indices >
	struct TupleDecompose_<N, size_pack<Indices...>> {
		using is_expression = std::nullptr_t;
		template < class Tuple >
		constexpr auto operator()(Tuple&& tup) const {
			static_assert( std::tuple_size<remove_cvr_t<Tuple>>::value == N, "tuple size mismatch" );
			return std::make_tuple( std::get<Indices>(tup)... );
		}
	};

	template < class... Elem >
	auto IntoDecompose(TupleDecomposerProxy<Elem...>) {
		return TupleDecompose_< sizeof...(Elem), typename ret_seq_for< TupleDecomposerProxy<remove_cvr_t<Elem>...>, std::index_sequence_for<Elem...> >::type >{};
	}


	//template < class, size_t = 0, class = size_pack<>, class = size_pack<> >
	//struct decompose_split;

	//template < class Head, size_t I, class... Tail, size_t... Ret, size_t... Match >
	//struct decompose_split<std::tuple<Head, Tail...>, I, size_pack<Ret...>, size_pack<Match...>>
	//	: std::conditional_t<is_ret<Head>::value,
	//		decompose_split<std::tuple<Tail...>, I+1, size_pack<Ret..., I>, size_pack<Match...>>,
	//			std::conditional_t<std::is_same<remove_cvr_t<Head>, placeholder_t<0>>::value,
	//			decompose_split<std::tuple<Tail...>, I + 1, size_pack<Ret...>, size_pack<Match...>>,
	//				decompose_split<std::tuple<Tail...>, I + 1, size_pack<Ret...>, size_pack<Match..., I>>
	//	>>
	//{};

}

template < class T, class U,
	enabler_t<is_placeholder_v<remove_cvr_t<T>> && is_placeholder_v<remove_cvr_t<U>>> = nullptr >
constexpr auto operator, (T&& t, U&& u) {
	return magic_arts::TupleDecomposerProxy<remove_cvr_t<T>, remove_cvr_t<U>>{ {std::forward<T>(t), std::forward<U>(u)}};
}


} // ! namespace cranberries

namespace std {
	template < class F, class... Args >
	struct is_bind_expression<::cranberries::LambdaExpression<F, Args...>>
		: public std::true_type
	{};
}

#endif

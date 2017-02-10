#ifndef CRANBRIIES_INTERVAL_LIB_COMPARE_FUNC_OBJ_HPP
#define CRANBRIIES_INTERVAL_LIB_COMPARE_FUNC_OBJ_HPP

#include "../interval.hpp"
#include "order_symbol.hpp"

namespace cranberries {
namespace interval_lib {
namespace compare {
namespace func_obj
{
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

} // ! func_obj
} // ! compare
} // ! interval_lib
} // ! cranberries
#endif
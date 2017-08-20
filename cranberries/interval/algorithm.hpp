#ifndef CRANBERRIES_INTERVAL_LIB_ALGORITHM_HPP
#define CRANBERRIES_INTERVAL_LIB_ALGORITHM_HPP

#include <type_traits>
#include <iterator>
#include <initializer_list>
#include <vector>
#include <numeric>
#include <algorithm>
#include "interval.hpp"
#include "compare.hpp"

namespace cranberries {

  /*  Interval Max  */

  /*  Two interval Argument Max  */

  template < typename T >
  inline interval<T> max( interval<T> const& a, interval<T> const& b ) noexcept
  {
    return weak_less( a, b ) ? b : a;
  }

  /*  initializer_list<interval> Argument Max  */

  template < typename T >
  inline interval<T> max( std::initializer_list< interval<T> > list ) noexcept
  {
    std::vector< interval<T> > tmp( list );
    std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T> const& a, interval<T> const& b ) { return weak_greater( a, b ); } );
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
  template < typename T, typename Compare, std::enable_if_t<!is_interval_v<Compare>, std::nullptr_t> = nullptr >
  inline interval<T> max( interval<T> const& a, interval<T> const& b, Compare comp ) noexcept
  {
    return comp( a, b ) ? a : b;
  }

  template < typename First, typename Second >
  inline constexpr First max( First&& a, Second&& b )
  {
    return weak_less( a, b ) ? a : b;
  }

  /*  Variadic arguments Max  */

  template < typename First, typename Second, typename ...Tail >
  inline constexpr First max( First&& a, Second&& b, Tail&& ... args )
  {
    return max( max( std::forward<First>( a ), std::forward<Second>( b ) ), std::forward<Tail>( args )... );
  }


  /*  Interval Min  */

  /*  Two interval Argument Min  */

  template < typename T >
  inline interval<T> min( interval<T> const& a, interval<T> const& b ) noexcept
  {
    return weak_less( a, b ) ? a : b;
  }

  /*  iitializer_list<interval> Argument Min  */

  template < typename T >
  inline interval<T> min( std::initializer_list< interval<T> > list ) noexcept
  {
    std::vector< interval<T> > tmp( list );
    std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return weak_less( a, b ); } );
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

  /*  Two intervals and Predicate Argument Min   */

  template < typename T, typename Compare, std::enable_if_t<!is_interval_v<Compare>, std::nullptr_t> = nullptr >
  inline interval<T> min( interval<T> const& a, interval<T> const& b, Compare comp ) noexcept
  {
    return comp( a, b ) ? a : b;
  }

  template < typename First, typename Second >
  inline constexpr First min( First&& a, Second&& b )
  {
    return weak_less( a, b ) ? a : b;
  }

  /*  Variadic arguments Min  */

  template < typename First, typename Second, typename ...Tail >
  inline constexpr First min( First&& a, Second&& b, Tail&& ... args )
  {
    return min( min( std::forward<First>( a ), std::forward<Second>( b ) ), std::forward<Tail>( args )... );
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

} // ! cranberries  
#endif
/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTERVAL_LIB_IO_HPP
#define CRANBERRIES_INTERVAL_LIB_IO_HPP

#include "interval.hpp"
#include "compare.hpp"
#include "version.hpp"

namespace cranberries {
namespace compare {
  /*  for enum output with string  */

  //interval_ordering to string
  const std::string enum2str( compare::interval_ordering const& x )
  {
    switch ( x )
    {
    case compare::interval_ordering::less:
      return std::string{ "less" };
    case compare::interval_ordering::greater:
      return std::string{ "greater" };
    case compare::interval_ordering::unordered:
      return std::string{ "unordered" };
    default:
      return std::string{ "niether" };
    }
  }

  //partial_ordering to string
  const std::string enum2str( compare::partial_ordering const& x )
  {
    switch ( x )
    {
    case compare::partial_ordering::less:
      return std::string{ "less" };
    case compare::partial_ordering::greater:
      return std::string{ "greater" };
    case compare::partial_ordering::unordered:
      return std::string{ "unordered" };
    default:
      return std::string{ "niether" };
    }
  }

  //weak_ordering to string
  const std::string enum2str( compare::weak_ordering const& x )
  {
    switch ( x )
    {
    case compare::weak_ordering::less:
      return std::string{ "less" };
    case compare::weak_ordering::greater:
      return std::string{ "greater" };
    case compare::weak_ordering::equivalent:
      return std::string{ "equivalent" };
    default:
      return std::string{ "niether" };
    }
  }


  /*  ostream << enum overload  */

  //ostream << interval_ordering
  std::ostream& operator<<( std::ostream& s, compare::interval_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

  //ostream << partial_ordering
  std::ostream& operator<<( std::ostream& s, compare::partial_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

  //ostream << weak_ordering
  std::ostream& operator<<( std::ostream& s, compare::weak_ordering const& x )
  {
    return ( s << enum2str( x ) );
  }

} // ! namespace compare

/*  output for version  */

std::ostream& operator<<( std::ostream& s, interval_version const& v )
{
  return s << ( static_cast<unsigned>( v ) / 0x01000000 )
    << "." << ( ( static_cast<unsigned>( v ) % 0x01000000 ) / 0x00010000 )
    << "." << ( static_cast<unsigned>( v ) % 0x00010000 );
}


} // ! cranberries

#endif
#ifndef CRANBERRIES_INTERVAL_LIB_IO_HPP
#define CRANBERRIES_INTERVAL_LIB_IO_HPP

#include "interval.hpp"
#include "compare.hpp"
#include "version.hpp"

namespace cranberries {
namespace interval_lib {
  namespace compare
  {
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

    //total_ordering to string
    const std::string enum2str( compare::total_ordering const& x )
    {
      switch ( x )
      {
      case compare::total_ordering::less:
        return std::string{ "less" };
      case compare::total_ordering::greater:
        return std::string{ "greater" };
      case compare::total_ordering::equal:
        return std::string{ "equal" };
      default:
        return std::string{ "niether" };
      }
    }

    //interval_relation to string to string
    const std::string enum2str( compare::interval_relation const& x )
    {
      switch ( x )
      {
      case compare::interval_relation::interval_less:
        return std::string{ "interval_less" };
      case compare::interval_relation::interval_greater:
        return std::string{ "interval_greater" };
      case compare::interval_relation::partial_less:
        return std::string{ "partial_less" };
      case compare::interval_relation::partial_greater:
        return std::string{ "partial_greater" };
      case compare::interval_relation::weak_less:
        return std::string{ "weak_less" };
      case compare::interval_relation::weak_greater:
        return std::string{ "weak_greater" };
      case compare::interval_relation::total_less:
        return std::string{ "total_less" };
      case compare::interval_relation::total_greater:
        return std::string{ "total_greater" };
      case compare::interval_relation::contain:
        return std::string{ "contain" };
      case compare::interval_relation::part_of:
        return std::string{ "part_of" };
      case compare::interval_relation::equal:
        return std::string{ "equal" };
      case compare::interval_relation::niether:
        return std::string{ "niether" };
      }
      CRANBERRIES_LOGIC_ERROR_THROW_WITH_MSG( "unknown enumerator." );
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

    //ostream << total_ordering
    std::ostream& operator<<( std::ostream& s, compare::total_ordering const& x )
    {
      return ( s << enum2str( x ) );
    }

    //ostream << interval_relation
    std::ostream& operator<<( std::ostream& s, compare::interval_relation const& x )
    {
      return ( s << enum2str( x ) );
    }

  }

  /*  output for version  */

  std::ostream& operator<<( std::ostream& s, version_tag const& v )
  {
    return s << ( static_cast<unsigned>( v ) / 0x01000000 )
      << "." << ( ( static_cast<unsigned>( v ) % 0x01000000 ) / 0x00010000 )
      << "." << ( static_cast<unsigned>( v ) % 0x00010000 );
  }


} // ! interval_lib
} // ! cranberries

#endif
#ifndef CRANBERRIES_STREAMS_OPERATORS_GROUPING_BY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_GROUPING_BY_HPP
#include <utility>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <deque>
#include "..\utility.hpp"
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {


namespace {

  template < opt Option >
  struct MapTraits
  {
    template < typename Key, typename T >
    using type = std::map<Key, T>;
  };

  template < >
  struct MapTraits<opt::unordered>
  {
    template < typename Key, typename T >
    using type = std::unordered_map<Key, T>;
  };

} // ! anonymous-namespace



  template <
    opt Option,
    template <class T, class Allocator = std::allocator<T>> class Seeq,
    typename Keygen
  >
  class GrouingBy
    : private detail::TerminateStreamOperatorBase
  {
  public:
    GrouingBy( Keygen gen ) : keygen_{ std::forward<Keygen>( gen ) } {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator ()
    (
      Stream&& stream_
    ) {
      static_assert(
        is_callable_v<Keygen, E&>,
        "Invalid predicate designated."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      
      auto result = typename MapTraits<Option>:: template type<std::result_of_t<Keygen( E )>, Seeq<E>>{};

      for (auto&& e : stream_)
        result[ keygen_( e ) ].push_back( e );

      return result;
    }

  private:
    Keygen keygen_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
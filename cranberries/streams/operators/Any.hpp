/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_OPERATORS_ANY_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ANY_HPP
#include <utility>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename Pred
  >
  class AnyOf
    : private cranberries_magic::EagerOperationModuleBase
  {
  public:
    AnyOf( Pred p ) noexcept
      : pred_{ std::forward<Pred>( p ) }
    {}

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
        is_callable_v<Pred,E&>,
        "Invalid predicate designated."
      );
      static_assert(
        std::is_same<
          bool,std::result_of_t<Pred(E&)>
        >::value,
        "Predicate must be return bool."
      );
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      for (auto&& e : stream_)
        if (pred_(e)) return true;
      return false;
    }

  private:
    Pred pred_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
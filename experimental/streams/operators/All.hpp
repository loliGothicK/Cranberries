#ifndef CRANBERRIES_STREAMS_OPERATORS_ALL_HPP
#define CRANBERRIES_STREAMS_OPERATORS_ALL_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename Pred
  >
  class AllOf
    : private detail::TerminateStreamOperatorBase
  {
  public:
    AllOf( Pred p ) noexcept
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
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
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
      for (auto&& e : stream_)
        if (!pred_(e)) return false;
      return true;
    }

  private:
    Pred pred_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
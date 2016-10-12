#ifndef CRANBERRIES_STREAMS_OPERATORS_DROPWHILE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_DROPWHILE_HPP
#include <utility>
#include <type_traits>
#include "..\utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  template <
    typename Pred
  >
  class DropWhile
    : private detail::IntermidiateStreamOperatorBase
    , private detail::StreamFilterBase
  {
  public:
    DropWhile( Pred p ) noexcept
      : pred_{ std::forward<Pred>( p ) }
    {}

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type,
      std::enable_if_t<is_range_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    )
      noexcept(false)
    {
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
      auto&& src = stream_.get();
      auto&& iter = src.begin();
      // peel loop
      while ( pred_(*iter) ) iter = src.erase(iter);
      return std::forward<Stream>(stream_);
    }

    template <
      typename T,
      std::enable_if_t<!is_range_v<std::decay_t<T>>,std::nullptr_t> = nullptr
    >
    bool
    operator[]
    (
      T&& a
    )
      const noexcept
    {
      static_assert(
        is_callable_v<Pred,T&&>,
        "Invalid predicate designated."
      );
      static_assert(
        std::is_same<
        bool,std::result_of_t<Pred(T&&)>
        >::value,
        "Predicate must be return bool."
      );

      if (flag)  return flag = pred_(a);
      return false;
    }
  private:
    size_t flag = true;
    Pred pred_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
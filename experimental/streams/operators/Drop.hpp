#ifndef CRANBERRIES_STREAMS_OPERATORS_DROP_HPP
#define CRANBERRIES_STREAMS_OPERATORS_DROP_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  class Drop
    : private cranberries_magic::LazyOperationModuleBase
    , private cranberries_magic::StreamFilterBase
  {
  public:
    Drop( size_t n ) : count{}, n_{ n } {}

    template <
      typename STREAM,
      std::enable_if_t<is_range_v<std::decay_t<STREAM>>,std::nullptr_t> = nullptr
    >
    inline
    decltype(auto)
    operator()(
      STREAM&& stream_
    ) {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      auto&& src = stream_.get();
      // deletion
      for ( auto&& iter = src.begin(); count<n_; ++count ) {
        iter = src.erase( iter );
      }
      return std::forward<STREAM>(stream_);
    }

    template <
      typename T
    >
    bool
    operator[]
    (
      T&& // not use
    )
      noexcept
    {
      if ( count < n_ ) {
        ++count;
        return true;
      }
      return false;
    }

  private:
    size_t count{};
    std::size_t n_;
  };



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
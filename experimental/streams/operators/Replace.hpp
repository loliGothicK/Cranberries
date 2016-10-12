#ifndef CRANBERRIES_STREAMS_OPERATORS_REPLACE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_REPLACE_HPP
#include <utility>
#include <type_traits>
#include <algorithm>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  // Intermidiate Operation
  template <
    typename ElemType
  >
  class Replace
    : private detail::IntermidiateStreamOperatorBase
    , private detail::StreamOperatorBase
  {
  public:
    Replace( ElemType new_, ElemType old_ ) noexcept
      : new_value{ new_ }
      , old_value{ old_ }
    {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::replace( stream_.begin(), stream_.end(), old_value, new_value );
      return std::forward<Stream>(stream_);
    }

    bool
    operator[]
    (
      ElemType const& a
    )
      noexcept
    {
      return a == old_value ? new_value : a;
    }

  private:
    ElemType old_value;
    ElemType new_value;
  };

  template <
    typename UnaryOp,
    typename New
  >
  class ReplaceIf
    : private  detail::IntermidiateStreamOperatorBase
    , private detail::StreamOperatorBase
  {
  public:
    ReplaceIf( UnaryOp op, New new_ ) noexcept
      : op_{ std::forward<UnaryOp>( op ) }
      , new_value{ std::forward<New>(new_) }
    {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()(
      Stream&& stream_
    )
      noexcept(false)
    {
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      std::replace_if( stream_.begin(), stream_.end(), op_, new_value );
      return std::forward<Stream>(stream_);
    }

    bool
    operator[]
    (
      New const& a
    )
      noexcept(noexcept(op_(a)))
    {
      return op_(a) ? new_value : a;
    }

    // member
  private:
    UnaryOp op_;
    New new_value;
  };



} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
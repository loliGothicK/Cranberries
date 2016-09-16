#ifndef CRANBERRIES_STREAMS_OPERATORS_MAP_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MAP_HPP
#include <utility>
#include "../detail/tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {


  template < typename F >
  struct MapProxy {
    F f;
  };

  template <
    typename UnaryFunc
  >
  class Transform
    : private detail::IntermidiateStreamOperatorBase
    , private detail::StreamOperatorBase
  {
  public:
    Transform( UnaryFunc f ) : f_{ std::forward<UnaryFunc>( f ) } {}

    template < typename F, typename T >
    void apply( F&& f, T&& a ) {
      a =  f(std::forward<T>(a));
    }

    template <
      typename Stream,
      std::enable_if_t<is_range_v<std::decay_t<Stream>>,std::nullptr_t> = nullptr
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      auto&& v = stream_.get();
      size_t i{};
      for(;i<v.size() % 8;++i)
          apply(f_,v[i]);
      for(; i<v.size();i+=8){
          apply(f_,v[i]);
          apply(f_,v[i+1]);
          apply(f_,v[i+2]);
          apply(f_,v[i+3]);
          apply(f_,v[i+4]);
          apply(f_,v[i+5]);
          apply(f_,v[i+6]);
          apply(f_,v[i+7]);
      }
      return std::forward<Stream>(stream_);
    }

    template <
      typename T
    >
    auto operator[](T&& a) const noexcept {
      return f_(a);
    }



    // mapper

    UnaryFunc f_;
  };

  template <
    typename OldStream,
    typename UnaryFunc
  >
  class Map
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Map( OldStream old, UnaryFunc f ) : old{ std::forward<OldStream>( old ) }, f_{ std::forward<UnaryFunc >> ( f ) } {}

    template <
      typename Stream
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      old.eval();
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( old.empty() );
      stream_.reserve(old.size());
      auto&& v = old.get();
      size_t i{};
      for(;i<v.size() % 8;++i)
          stream_.emplace_back(v[i]);
      for(; i<v.size();i+=8){
          stream_.emplace_back(v[i]);
          stream_.emplace_back(v[i+1]);
          stream_.emplace_back(v[i+2]);
          stream_.emplace_back(v[i+3]);
          stream_.emplace_back(v[i+4]);
          stream_.emplace_back(v[i+5]);
          stream_.emplace_back(v[i+6]);
          stream_.emplace_back(v[i+7]);
      }
      return std::forward<Stream>(stream_);
    }

    // member
    OldStream old;
    UnaryFunc f_;
  };

} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
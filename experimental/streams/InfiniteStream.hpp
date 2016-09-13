#ifndef CRANBERRIES_STREAMS_INFINITE_STREAM_HPP
#define CRANBERRIES_STREAMS_INFINITE_STREAM_HPP
#include <utility>
#include <type_traits>
#include <tuple>
#include "detail/tag.hpp"
#include "utility.hpp"
#include "stream_error.hpp"
#include "operators\TakeWhile.hpp"

namespace cranberries {
namespace streams {

  template < typename T >
  struct Ranged{

    template < typename Range >
    decltype(auto) operator()(Range&& range){
      range.reserve( static_cast<size_t>((last-first)/step) );
      for ( ;first < last; first+=step )
      {
        range.emplace_back( first );
      }
      return std::forward<Range>(range);
    }

    T first;
    T last;
    T step;
  };

  template < typename T, typename Gen >
  class GenerateStream : private detail::InfiniteStreamBase
  {
  public:
    using type = T;

    GenerateStream( Gen gen ) : current_{ gen() }, gen_{ std::forward<Gen>( gen ) } {}

    type get() noexcept { return make_finally( [&] { advance(); } ), current_; }

    type advance() noexcept { return current_ = gen_(); }

    type current() noexcept { return current_; }

  private:
    T current_;
    Gen gen_;
  };

  template < typename InitType, typename Func >
  class IterateStream : private detail::InfiniteStreamBase
  {
  public:
    using type = InitType;

    IterateStream( InitType init, Func func ) noexcept : current_{ init }, next_{ func } {}

    type get() noexcept { return make_finally( [&] { advance(); } ), current_; }

    type advance() noexcept { return current_ = next_( current_ ); }

    type current() noexcept { return current_; }

  private:
    InitType current_;
    Func next_;
  };

  template <
    typename Stream,
    typename Operator
  >
  class StreamOperator : private detail::InfiniteStreamBase
  {
  public:
    using type = decltype( std::declval<Stream>().get() );

    StreamOperator( Stream x, Operator op ) noexcept
      : current_{ op( x.current() ) }
      , stream_{ std::forward<Stream>(x) }
      , op_{ std::forward<Operator>(op) }
    {}

    type get() noexcept { return make_finally( [&] { advance(); } ), current_; }

    type advance() noexcept { return current_  = op_(stream_.advance()); }

    type current() noexcept { return current_; }

  private:
    type current_;
    Stream stream_;
    Operator op_;
  };

  template <
    typename Stream,
    typename Filter
  >
  class StreamFilter : private detail::InfiniteStreamBase
  {
  public:
    using type = typename std::decay_t<Stream>::type;

    StreamFilter( Stream x, Filter pred ) noexcept : stream_{ std::forward<Stream>(x) }, pred_{ std::forward<Filter>(pred) } {}

    type get() noexcept {
      while ( pred_( stream_.current() ) ) advance();
      return stream_.get();
    }

    type advance() noexcept { return stream_.advance(); }

    type current() noexcept { return stream_.current(); }

  private:
    Stream stream_;
    Filter pred_;
  };

  template <
    typename Stream1,
    typename Stream2,
    bool IsFinite1 = detail::is_finite_stream_v<Stream1>,
    bool IsFinite2 = detail::is_finite_stream_v<Stream2>
  >
  struct StreamMerger
    : private detail::InfiniteStreamBase
  {
    static_assert(IsFinite1&&IsFinite2,"internal critical error!");
    using type = decltype( std::declval<Stream1>().get() );

    StreamMerger( Stream1 x, Stream2 y ) noexcept
      : current_{ x.current() < y.current() ? x.current() : y.current() }
      , stream_1{ std::forward<Stream1>(x) }
      , stream_2{ std::forward<Stream2>(y) }
      , flag{ x.current() <= y.current() }
    {}

    type get() noexcept {
      return make_finally( [&] { advance(); } ), current_;
    }

    type advance() noexcept {
      return current_ = ( flag ? stream_1.advance() <= stream_2.current() : stream_1.current() <= stream_2.advance() ) ? ( flag = true, stream_1.current() ) : ( flag = false, stream_2.current() ) ;
    }

    type current() noexcept { return current_; }

    type current_;
    Stream1 stream_1;
    Stream2 stream_2;
    bool flag;
  };

  template <
    typename Stream1,
    typename Stream2
  >
  struct StreamMerger<
    Stream1,Stream2,
    true,false
  >
    : private detail::InfiniteStreamBase
  {
    using type = typename std::decay_t<Stream1>::element_type;

    StreamMerger( Stream1 x, Stream2 y ) noexcept
      : current_{ x.current() <= y.current() ? x.current() : y.current() }
      , stream_1{ std::forward<Stream1>(x) }
      , stream_2{ std::forward<Stream2>(y) }
      , flag{ x.current() <= y.current() }
    {}

    type get() noexcept {
      return make_finally( [&] { advance(); } ), current_;
    }

    type advance() noexcept {
      if ( flag ) {
        return current_ = stream_1.is_next()
          ? stream_1.current() <= stream_2.current()
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() )
          : ( flag = false, stream_2.current() );
      } else {
        return current_ = stream_1.is_end()
          ? ( flag = false, stream_2.advance() )
          : stream_1.current() <= stream_2.get()
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() );
      }
    }


    type current() noexcept { return current_; }

    type current_;
    Stream1 stream_1;
    Stream2 stream_2;
    bool flag;
  };


  template <
    typename Stream1,
    typename Stream2
  >
  struct StreamMerger<
    Stream1,Stream2,
    false,true
  >
    : private detail::InfiniteStreamBase
  {
    using type = decltype(std::declval<Stream1>().get());

    StreamMerger( Stream1 x, Stream2 y ) noexcept
      : current_{ x.current() <= y.current() ? x.current() : y.current() }
      , stream_1{ std::forward<Stream1>(x) }
      , stream_2{ std::forward<Stream2>(y) }
      , flag{ x.current() <= y.current() }
    {}

    type get() noexcept {
      return make_finally( [&] { advance(); } ), current_;
    }

    type advance() noexcept {
      if ( flag ) {
        return current_ = stream_2.is_end()
          ? ( flag = true, stream_1.advance() )
          : stream_1.advance() <= stream_2.current()
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() );
      } else {
        return current_ = stream_2.is_next()
          ? stream_1.current() <= stream_2.current()
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() )
          : ( flag = true, stream_1.current() );
      }
    }


    type current() noexcept { return current_; }

    type current_;
    Stream1 stream_1;
    Stream2 stream_2;
    bool flag;
  };


  template < typename Stream >
  struct Limited
  {
    template < typename Range >
    decltype( auto ) operator()( Range&& range ) {
      range.reserve( lim_ );
      for ( size_t i{}; i < lim_; ++i ) {
        range.emplace_back( stream_.get() );
      }
      return std::forward<Range>(range);
    }
    size_t lim_;
    Stream stream_;
  };

  template <
    typename Stream,
    typename Pred
  >
  struct LimitedWhile
  {
    template < typename Range >
    decltype( auto ) operator()( Range&& range ) {

      while ( pred_( stream_.current() ) ) {
        range.emplace_back( stream_.get() );
      }
      return std::forward<Range>(range);
    }

    operators::TakeWhile<Pred>&& pred_;
    Stream stream_;
  };


} // ! namespace stream
} // ! namespace cranberries

#endif
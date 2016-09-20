#ifndef CRANBERRIES_STREAMS_INFINITE_STREAM_HPP
#define CRANBERRIES_STREAMS_INFINITE_STREAM_HPP
#include <utility>
#include <type_traits>
#include <tuple>
#include "detail/tag.hpp"
#include "utility.hpp"
#include "stream_error.hpp"
#include "detail\enable_men_fn.hpp"

namespace cranberries {
namespace streams {

  template < typename T >
  struct Ranged
  {
    template <
      typename Range
    >
    decltype(auto)
    operator()
    (
      Range&& range
    ){
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

  template <
    typename T,
    typename Gen
  >
  class GenerateStream
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<GenerateStream<T,Gen>>
  {
  public:
    using element_type = T;

    GenerateStream( Gen gen ) : current_{ gen() }, gen_{ std::forward<Gen>( gen ) } {}

    element_type get() noexcept { return current_; }

    element_type advance() noexcept { return current_ = gen_(); }

    element_type current() { return current_; }

  private:
    T current_;
    Gen gen_;
  };

  template < typename InitType, typename Func >
  class IterateStream
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<IterateStream<InitType,Func>>
  {
  public:
    using element_type = InitType;

    IterateStream( InitType init, Func func ) noexcept : current_{ init }, next_{ std::forward<Func>(func) } {}
    IterateStream( InitType init ) noexcept : current_{ init }, next_{} {}

    IterateStream( IterateStream&& ) = default;
    IterateStream& operator=( IterateStream&& ) = default;

    element_type get() noexcept { return current_; }

    element_type advance() noexcept { return current_ = next_( current_ ); }

    element_type current() { return current_; }

  private:
    std::decay_t<InitType> current_;
    Func next_;
  };

  template < typename T >
  class CyclicStream
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<CyclicStream<T>>
  {
  public:

    using element_type = T;

    template <
      typename Range,
      std::enable_if_t<is_range_v<std::decay_t<Range>>,std::nullptr_t> = nullptr
    >
    CyclicStream( Range&& range ) : cycle_{ range.begin(), range.end() }, current_{cycle_.begin()} {}

   template <
      typename Iterator,
      std::enable_if_t<
        is_iterator_v<Iterator>
     && std::is_same<T, typename Iterator::value_type>::value,
        std::nullptr_t
      > = nullptr
    >
    CyclicStream( Iterator first, Iterator last ) : cycle_{ first, last }, current_{cycle_.begin()} {}


    CyclicStream( std::initializer_list<T> il ) : cycle_{ il }, current_{cycle_.begin()} {}

    element_type get() { return *current_; }

    element_type advance() { return *( ++current_ == cycle_.end() ? (current_ = cycle_.begin()) : current_ ); }

    element_type current() { return *current_; }

  private:
    std::vector<T> cycle_;
    typename std::vector<T>::iterator current_;
  };

  template <
    typename Stream,
    typename Operator
  >
  class StreamOperator
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<StreamOperator<Stream,Operator>>
  {
  public:
    using element_type = typename std::decay_t<Stream>::element_type;

    StreamOperator( Stream x, Operator op ) noexcept
      : current_{ op[ x.get() ] }
      , stream_{ std::forward<Stream>(x) }
      , op_{ std::forward<Operator>(op) }
    {}

    element_type get() noexcept { return current_; }

    element_type advance() noexcept { return current_ = op_[stream_.advance(),stream_.get()]; }

    element_type current() { return current_; }

  private:
    element_type current_;
    Stream stream_;
    Operator op_;
  };

  template <
    typename Stream,
    typename Filter
  >
  class StreamFilter
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<StreamFilter<Stream,Filter>>
  {
  public:
    using element_type = typename std::decay_t<Stream>::element_type;

    StreamFilter( Stream x, Filter pred ) noexcept : stream_{ std::forward<Stream>(x) }, pred_{ std::forward<Filter>(pred) } {}

    element_type get() noexcept {
      while ( pred_[stream_.get()] ) advance();
      return stream_.current();
    }

    element_type advance() noexcept {
      return stream_.advance();
    }

    element_type current() { return stream_.current(); }

  private:
    Stream stream_;
    Filter pred_;
  };

  namespace {
  namespace workaround {
  
  template <
    bool IsStream
  >
    struct for_msvc
  {
    template <
      typename Stream,
      typename Range
    >
    static
    void
    invoke
    (
      Stream&& stream_,
      Range&& proj_
    ) {
      stream_.insert(stream_.end(), proj_.rbegin(), proj_.rend());
    }
  };

  template < >
  struct for_msvc<true>
  {
    template <
      typename Stream,
      typename Range
    >
    static
    void
    invoke
    (
      Stream&& stream_,
      Range&& proj_
    ) {
      proj_.eval();
      stream_.insert(stream_.end(), proj_.rbegin(), proj_.rend());
    }
  };

} // ! namespace workaround
} // ! anonymous-namespace


  template <
    typename Stream,
    typename Operator
  >
  class StreamFlatTransformer
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<StreamFlatTransformer<Stream,Operator>>
  {
  public:
    using element_type = typename std::decay_t<Stream>::element_type;

    StreamFlatTransformer(Stream x, Operator op)
      : stream_{ std::forward<Stream>(x) }
      , op_{ std::forward<Operator>(op) }
      , proj_{}
    {
      push(op_[stream_.get()]);
    }

    template <
      typename T
    >
    void
    push_tuple
    (
      T&& arg
    ) {
      proj_.emplace_back(std::move(arg));
    }

    template <
      typename Head,
      typename ...Tail
    >
    void
    push_tuple(
      Head&& head,
      Tail&& ...tail
    ) {
      return push_tuple(std::forward<Tail>(tail)...), proj_.emplace_back(std::move(head));
    }

    template <
      typename Range,
      std::enable_if_t<is_range_v<std::decay_t<Range>>,std::nullptr_t> = nullptr
    >
    void
    push
    (
      Range&& range
    )
      noexcept
    {
      workaround::for_msvc<
        detail::is_finite_stream_v<std::decay_t<Range>>
      >::invoke(proj_, std::forward<Range>(range));
    }

    template <
      typename Tuple,
      std::enable_if_t<is_tuple_v<std::decay_t<Tuple>>, std::nullptr_t> = nullptr
    >
    void
    push
    (
      Tuple&& tup
    )
      noexcept
    {
      apply( [&](auto&& ...args) { push_tuple(std::forward<decltype(args)>(args)...); }, std::forward<Tuple>(tup) );
    }



    element_type get() noexcept { return proj_.back(); }

    element_type advance() noexcept {
      proj_.pop_back();
      if (proj_.empty()) {
        stream_.advance();
        push(op_[stream_.get()]);
      }
      return proj_.back();
    }

    element_type current() noexcept { return proj_.back(); }

  private:
    Stream stream_;
    Operator op_;
    std::vector<element_type> proj_;
  };

  template <
    typename Stream1,
    typename Stream2,
    bool IsFinite1 = detail::is_finite_stream_v<Stream1>,
    bool IsFinite2 = detail::is_finite_stream_v<Stream2>
  >
  struct StreamMerger
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<StreamMerger<Stream1,Stream2,IsFinite1,IsFinite2>>
  {
    static_assert(!(IsFinite1&&IsFinite2),"internal critical error!");
    using element_type = typename std::decay_t<Stream1>::element_type;

    StreamMerger( Stream1 x, Stream2 y ) noexcept
      : current_{ x.get() <= y.get() ? x.get() : y.get() }
      , stream_1{ std::forward<Stream1>(x) }
      , stream_2{ std::forward<Stream2>(y) }
      , flag{ x.current() <= y.current() }
    {}

    element_type get() noexcept {
      return current_;
    }

    element_type advance() noexcept {
      return current_ =
        flag
        ? ( stream_1.advance(), stream_1.get() <= stream_2.get() )
        ? ( flag = true, stream_1.get() )
        : ( flag = false, stream_2.get() )
        : ( stream_2.advance(), stream_1.get() <= stream_2.get() )
        ? ( flag = true, stream_1.get() )
        : ( flag = false, stream_2.get() );
    }

    element_type current() { return current_; }

  private:
    element_type current_;
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
    , public detail::enable_men_fn_inf<StreamMerger<Stream1,Stream2,true,false>>
  {
    using element_type = typename std::decay_t<Stream1>::element_type;

    StreamMerger( Stream1 x, Stream2 y ) noexcept
      : current_{ x.current() <= y.get() ? x.current() : y.get() }
      , stream_1{ std::forward<Stream1>(x) }
      , stream_2{ std::forward<Stream2>(y) }
      , flag{ x.current() <= y.current() }
    {}

    element_type get() noexcept { return current_; }

    element_type advance() noexcept {
      if ( flag ) {
        return current_ = stream_1.is_next()
          ? stream_1.current() <= stream_2.get()
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() )
          : ( flag = false, stream_2.current() );
      } else {
        return current_ = stream_1.is_end()
          ? ( flag = false, stream_2.advance(), stream_2.get() )
          : (stream_2.advance(), stream_1.current() <= stream_2.current() )
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() );
      }
    }

    element_type current() { return current_; }

  private:
    element_type current_;
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
    , public detail::enable_men_fn_inf<StreamMerger<Stream1,Stream2,false,true>>
  {
    using element_type = decltype(std::declval<Stream1>().get());

    StreamMerger( Stream1 x, Stream2 y ) noexcept
      : current_{ x.get() <= y.current() ? x.get() : y.current() }
      , stream_1{ std::forward<Stream1>(x) }
      , stream_2{ std::forward<Stream2>(y) }
      , flag{ x.current() <= y.current() }
    {}

    element_type get() noexcept { return current_; }

    element_type advance() noexcept {
      if ( flag ) {
        return current_ = stream_2.is_end()
          ? ( flag = false, stream_1.advance(), stream_1.current() )
          : ( stream_1.advance(), stream_1.get() <= stream_2.current() )
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() );
      } else {
        return current_ = stream_2.is_next()
          ? stream_1.get() <= stream_2.current()
          ? ( flag = true, stream_1.current() )
          : ( flag = false, stream_2.current() )
          : ( flag = true, stream_1.current() );
      }
    }

    element_type current() { return current_; }

  private:
    element_type current_;
    Stream1 stream_1;
    Stream2 stream_2;
    bool flag;
  };

  template <
    typename Stream1,
    typename Stream2
  >
  class StreamConcatenator
    : private detail::InfiniteStreamBase
    , public detail::enable_men_fn_inf<StreamConcatenator<Stream1,Stream2>>
  {
    static_assert(
      detail::is_finite_stream_v<Stream1>
      && detail::is_infinite_stream_v<Stream2>,
      "" // TODO
    );
  public:
    using element_type = typename std::decay_t<Stream1>::element_type;

    StreamConcatenator
    (
      Stream1 x, Stream2 y
    ) noexcept
      : stream_1{ std::forward<Stream1>( x ) }
      , stream_2{ std::forward<Stream2>( y ) }
      , current_{ stream_1.current() }
    {}

    element_type get() { return current_; }

    element_type advance() {
      if ( flag )
      {
        return current_ =
          stream_1.is_next()
          ? stream_1.is_end()
          ? ( flag = false, stream_2.get() )
          : stream_1.current()
          : ( flag = false, stream_2.get() );
      } else {
        return current_ = stream_2.advance(), stream_2.get();
      }
    }

    element_type current() { return current_; }

  private:
    Stream1 stream_1;
    Stream2 stream_2;
    element_type current_;
    bool flag = true;
  };


  template < typename Stream >
  struct Limited
  {
    template < typename Range >
    decltype( auto ) operator()( Range&& range ) {
      range.reserve( lim_ );
      for ( size_t i{}; i < lim_; ++i,stream_.advance() ) {
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

      while ( pred_[ stream_.get() ] ) {
        range.emplace_back( stream_.current() );
        stream_.advance();
      }
      return std::forward<Range>(range);
    }

    Stream stream_;
    operators::TakeWhile<Pred>&& pred_;
  };


} // ! namespace stream
} // ! namespace cranberries

#endif
/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_STREAM_HPP
#define CRANBERRIES_STREAMS_STREAM_HPP
#include <vector>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include "../common/exception.hpp"
#include "operators/Identity.hpp"
#include "cranberries_magic/enable_men_fn.hpp"
#include "OperationTree.hpp"

namespace cranberries {
namespace streams {

  //-------------//
  // stream Body //
  //-------------//

  /*

  [ Note : stream process is one of iterate process.
  Abstract iterate process as stream.
  stream is abstract process that continuously operating to treat range object through pipeline.

  << stream Create Phase >>
  stream<T> is able to construct from Range<T>, iterator pair has element_type as T and initializer_list<T>.
  Template parameter T is source type. In stream, source manageed as vector<T>.
  Template parameter Operation is Operation Tree.
  Root Operation is default setting at creating stream<T> .

  << Pipeline Construction Phase >>
  You can chain Intermidiate Operation.
  Intermidiate Operation is lazy evaluate.
  The return type Intermidiate Operation is temporary proxy stream object.
  Operation is reccuresively construct like 'Operation<A,Operation<B,C>>'.
  To bind temporary proxy stream, you can use 'auto' keyword.

  << Termination Phase >>
  You can execute Terminate Operation or convert stream to container.
  Execute Terminate Operation or onvert to container, evaluating Intermidiate Operation Pipeline.

  Let 's' is a stream and 'A', 'B' are Intermidiate Queries, and 'C' is Terminate Operation.
  stream Intermidiate Queries are combine with operator >> and Terminate Operation using operator >> .

  [ Example :
  s >> A >> B >> C;
  - end example ]

  When convert stream to container, use convert kryword( constexpr variable ).
  Implicit convert to any container from iterator pair.

  [ Example :
  std::vector<int> v = s >> A >> B >> convert;
  - end example ]


  - end note ]

  */

  template <
    typename T,
    typename Operation
  >
  class stream
    : private cranberries_magic::FiniteStreamBase
    , public cranberries_magic::enable_men_fn<stream<T,Operation>>
  {
  public:
    typedef  T                                                element_type;
    typedef  std::vector<T>                                   range_type;
    typedef  typename std::vector<T>::value_type              value_type;
    typedef  typename std::vector<T>::reference               reference;
    typedef  typename std::vector<T>::const_reference         const_reference;
    typedef  typename std::vector<T>::iterator                iterator;
    typedef  typename std::vector<T>::const_iterator          const_iterator;
    typedef  typename std::vector<T>::size_type               size_type;
    typedef  typename std::vector<T>::difference_type         difference_type;
    typedef  typename std::vector<T>::reverse_iterator        reverse_iterator;
    typedef  typename std::vector<T>::const_reverse_iterator  const_reverse_iterator;

    stream
    (
      std::vector<T>&& src,
      Operation&& q,
      std::string path,
      std::ios::openmode openmode
    )
      : source_{std::move(src)}
      , operation_{std::forward<Operation>(q)}
      , path_{path}
      , openmode_{openmode}
    { }

    stream
    (
      std::string path,
      std::ios::openmode openmode = std::ios::in | std::ios::out
    )
      : source_{}
      , operation_{ operators::Identity{} }
      , path_{ path }
      , openmode_{ openmode }
    { }


    template <
      typename Iterator,
      enabler_t<is_iterator_v<Iterator>> = nullptr
    >
    stream
    (
      Iterator&& first,
      Iterator&& last,
      Operation&& q
    ) noexcept
      : source_{ std::forward<Iterator>(first), std::forward<Iterator>(last) }
      , operation_{ std::move(q) }
    {}

    stream
    (
      std::initializer_list<T> init_list,
      Operation&& q
    ) noexcept
      : source_{ init_list }
      , operation_{ std::move(q) }
    {}

    template <
      typename Range,
      enabler_t<is_range_v<Range>> = nullptr
    >
    stream
    (
      Range&& range,
      Operation&& q
    ) noexcept
      : source_{ range.begin(), range.end() }
      , operation_{ std::move(q) }
    {}

    template <
      typename Iterator,
      enabler_t<is_iterator_v<Iterator>> = nullptr
    >
    stream
    (
      Iterator&& first,
      Iterator&& last
    ) noexcept
      : source_{ std::forward<Iterator>(first), std::forward<Iterator>(last) }
      , operation_{}
    {}

    stream
    (
      std::initializer_list<T> init_list
    ) noexcept
      : source_{ init_list }
     , operation_{}
    {}

    template <
      typename Range,
      enabler_t<is_range_v<Range>> = nullptr
    >
    stream
    (
      Range&& range
    ) noexcept
      : source_{ range.begin(), range.end() }
      , operation_{}
    {}

    stream() noexcept
      : source_{}
      , operation_{}
    {}

    stream(stream const&) = default;
    stream(stream&&) = default;
    ~stream() = default;
    stream& operator=(stream const&) = default;
    stream& operator=(stream&&) = default;

    // stream to Range converter
    template <
      typename TargetRange // Concept : Range must be construct from iterator pair.
    >
    inline
    TargetRange
    convert
    ()
      const noexcept(false)
    {
      return TargetRange{ source_.begin(), source_.end() };
    }

    // Range wrappers

    inline decltype(auto) begin() { return source_.begin(); }

    inline decltype(auto) end() { return source_.end(); }

    inline decltype(auto) begin() const { return source_.cbegin(); }

    inline decltype(auto) end() const { return source_.cend(); }

    inline decltype(auto) cbegin() const { return source_.cbegin(); }

    inline decltype(auto) cend() const { return source_.cend(); }

    inline decltype(auto) rbegin() { return source_.rbegin(); }

    inline decltype(auto) rend() { return source_.rend(); }

    inline decltype(auto) crbegin() const { return source_.crbegin(); }

    inline decltype(auto) crend() const { return source_.crend(); }

    size_t size() { return source_.size(); }

    inline decltype(auto) max_size() { return source_.max_size(); }

    inline decltype(auto) resize(size_t n) { return source_.resize(n); }

    inline decltype(auto) capacity() { return source_.capacity(); }

    inline bool empty() { return source_.empty(); }

    inline decltype(auto) reserve(size_t n) { return source_.reserve(n); }

    inline decltype(auto) insert(const_iterator pos, T const& v) { return source_.insert(pos, v); }

    inline decltype(auto) insert(const_iterator pos, T&& v) { return source_.insert(pos, std::move(v)); }

    inline decltype( auto ) insert( const_iterator pos, size_type n, T const& v ) { return source_.insert( pos, n, v ); }

    template <class InputIterator>
    decltype(auto) insert(
      const_iterator pos,
      InputIterator first,
      InputIterator last
    ) {
      return source_.insert(pos, first, last);
    }

    inline decltype(auto) insert(
      const_iterator pos,
      std::initializer_list<T> il
    ) {
      return source_.insert(pos, il);
    }

    iterator erase( const_iterator pos ) { return source_.erase( pos ); }

    iterator erase( const_iterator first, const_iterator last ) { return source_.erase( first, last ); }

    void swap( std::vector<T> v ) { source_.swap( v ); }

    reference at( size_type n ) { return source_.at( n ); }

    template <class InputIterator>
    void assign( InputIterator first, InputIterator last ) { source_.assign( first, last ); }

    void assign( size_type n, const T& u ) { source_.assign( n, u ); }

    void assign( std::initializer_list<T> il ) { source_.assign( il ); }

    reference front() { return source_.front(); }

    reference back() { return source_.back(); }

    template < typename U >
    inline void push_back(U&& v) { source_.push_back(v); }

    template < typename ...U >
    decltype(auto) emplace_back(U&& ...v) { return source_.emplace_back(std::forward<U>(v)...); }

    inline void pop_back() { source_.pop_back(); }

    template < typename U >
    inline void emplace_back(U&& v) { source_.emplace_back(v); }

    inline void shrink_to_fit() { source_.shrink_to_fit(); }

    inline void clear() { source_.clear(); }

    value_type operator[]( size_t const& n ) { return source_[n]; }

    // resource getter
    inline auto& get() { return source_; }

    auto path() { return path_; }

    auto openmode() { return openmode_; }

    void fopen(std::string path) { path_ = path; }

    template < typename Operator >
    auto make_pipeline(Operator&& op) { return make_op_tree(std::move(operation_), std::move(op)); }


    // Operator Registration
    template <
      typename Operator
    >
    auto lazy( Operator&& op ) {
      return stream<T, OperationTree<Operation, Operator>>{
        std::move(source_), OperationTree<Operation, Operator>{ std::move(operation_), std::forward<Operator>(op) },
          path_, openmode_
      };
    }

    // Operator Registration
    //template < typename Operator >
    //auto lazy(Operator&& op) {
    //  return [&](auto&& op_) { return stream<T, decltype(op_)>{
    //    std::move(source_), std::move(op_), path_, openmode_
    //  }; }(make_pipeline(std::move(op)));
    //}

    template < typename Operator >
    decltype(auto) eager(Operator&& op) {
      return op(*this);
    }

    // execute lazy evaluation
    decltype(auto) eval() noexcept(false) {
      return operation_(*this); // evaluate pipeline.
    }

    void once() noexcept(false) {
      if ( once_flag ) {
        once_flag = false;
        eval();
        current_ = source_.begin();
      }
    }

    T current() noexcept(false) {
      once( );
      return *current_;
    }

    bool is_end() noexcept(false) {
      once( );
      return current_ == source_.end();
    }

    bool is_next() noexcept(false) {
      once( );
      return current_ == source_.end() ? false : ++current_ != source_.end();
    }

  private:
    // source
    std::vector<T> source_{};

    // Operation Expression Template Tree
    Operation operation_{};

    std::string path_{};

    std::ios::openmode openmode_{std::ios::in|std::ios::out};

    // For merge/concat to infinite stream
    typename std::vector<T>::iterator current_{};

    bool once_flag = true;
  };


} // ! namespace streams
} // ! namespace cranberries

#endif

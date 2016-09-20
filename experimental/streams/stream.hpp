#ifndef CRANBERRIES_STREAMS_STREAM_HPP
#define CRANBERRIES_STREAMS_STREAM_HPP
#include <vector>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include "operators\Identity.hpp"
#include "detail\enable_men_fn.hpp"

namespace cranberries {
namespace streams {


  template <
    typename Op1, // evaluate first
    typename Op2  // evaluate next
  >
  struct OperationTree
  {
    template <
      typename Stream
    >
      inline
      decltype(auto)
      operator()(
        Stream&& stream
        ) {
      return op2(op1(std::forward<Stream>(stream)));
    }

    // members
    Op1 op1;
    Op2 op2;
  };


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

  Let 's' is a stream and 'A', 'B' are Intermidiate Querys, and 'C' is Terminate Operation.
  stream Intermidiate Querys are combine with operator >> and Terminate Operation using operator || .

  [ Example :
  s >> A >> B || C;
  - end example ]

  When convert stream to container, use convert kryword( constexpr variable ).
  Implicit convert to any container from iterator pair.

  [ Example :
  std::vector<int> v = s >> A >> B || convert;
  - end example ]


  - end note ]

  */

  template <
    typename T,
    typename Operation
  >
  class stream
    : private detail::FiniteStreamBase
    , public detail::enable_men_fn<stream<T,Operation>>
  {
  public:
    typedef T element_type;
    typedef typename std::vector<T>::value_type value_type;;
    typedef std::vector<T> range_type;
    typedef typename std::vector<T>::reference reference;
    typedef typename std::vector<T>::const_reference const_reference;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::difference_type difference_type;
    typedef typename std::vector<T>::reverse_iterator reverse_iterator;
    typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

    template <
      typename Iterator
    >
    stream
    (
      Iterator first,
      Iterator last,
      Operation&& q
    )
      : source{ first,last }
      , operation{ std::move(q) }
    {}

    stream
    (
      std::initializer_list<T> init_list,
      Operation&& q
    )
      : source{ init_list }
      , operation{ std::move(q) }
    {}

    template <
      typename Range
    >
    stream
    (
      Range&& range,
      Operation&& q
    )
      : source{ range.begin(), range.end() }
      , operation{ std::move(q) }
    {}

    template <
      typename Iterator
    >
    stream
    (
      Iterator first,
      Iterator last
    )
      : source{ first,last }
      , operation{ }
    {}

    stream
    (
      std::initializer_list<T> init_list
    )
      : source{ init_list }
     , operation{}
    {}

    template <
      typename Range,
      std::enable_if_t<is_range_v<std::decay_t<Range>>,std::nullptr_t> = nullptr
    >
    stream
    (
      Range&& range
    )
      : source{ range.begin(), range.end() }
      , operation{}
    {}

    stream()
      : source{}
      , operation{}
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
      const
    {
      return TargetRange{ source.begin(), source.end() };
    }

    // Range wrappers

    inline decltype(auto) begin() { return source.begin(); }

    inline decltype(auto) end() { return source.end(); }

    inline decltype(auto) begin() const { return source.cbegin(); }

    inline decltype(auto) end() const { return source.cend(); }

    inline decltype(auto) cbegin() const { return source.cbegin(); }

    inline decltype(auto) cend() const { return source.cend(); }

    inline decltype(auto) rbegin() { return source.rbegin(); }

    inline decltype(auto) rend() { return source.rend(); }

    inline decltype(auto) crbegin() const { return source.crbegin(); }

    inline decltype(auto) crend() const { return source.crend(); }

    inline decltype(auto) size() { return source.size(); }

    inline decltype(auto) max_size() { return source.max_size(); }

    inline decltype(auto) resize(size_t n) { return source.resize(n); }

    inline decltype(auto) capacity() { return source.capacity(); }

    inline bool empty() { return source.empty(); }

    inline decltype(auto) reserve(size_t n) { return source.reserve(n); }

    inline decltype(auto) insert(const_iterator pos, T const& v) { return source.insert(pos, v); }

    inline decltype(auto) insert(const_iterator pos, T&& v) { return source.insert(pos, std::move(v)); }

    inline decltype( auto ) insert( const_iterator pos, size_type n, T const& v ) { return source.insert( pos, n, v ); }

    template <class InputIterator>
    decltype(auto) insert(
      const_iterator pos,
      InputIterator first,
      InputIterator last
    ) {
      return source.insert(pos, first, last);
    }

    inline decltype(auto) insert(
      const_iterator pos,
      std::initializer_list<T> il
    ) {
      return source.insert(pos, il);
    }

    iterator erase( const_iterator pos ) { return source.erase( pos ); }

    iterator erase( const_iterator first, const_iterator last ) { return source.erase( first, last ); }

    void swap( std::vector<T> v ) { source.swap( v ); }

    reference at( size_type n ) { return source.at( n ); }

    template <class InputIterator>
    void assign( InputIterator first, InputIterator last ) { source.assign( first, last ); }

    void assign( size_type n, const T& u ) { source.assign( n, u ); }

    void assign( std::initializer_list<T> il ) { source.assign( il ); }

    reference front() { return source.front(); }

    reference back() { return source.back(); }

    template < typename U >
    inline void push_back(U&& v) { source.push_back(v); }

    inline void pop_back() { source.pop_back(); }

    template < typename U >
    inline void emplace_back(U&& v) { source.emplace_back(v); }

    inline void shrink_to_fit() { source.shrink_to_fit(); }

    value_type operator[]( size_t const& n ) { return source[n]; }

    // resource getter
    inline auto& get() { return source; }


    // Operator Registration
    template <
      typename Operator
    >
    inline
    auto
    lazy(
      Operator&& op
    ) {
      return stream<T, OperationTree<Operation, Operator>>{
        std::move( source ), OperationTree<Operation, Operator>{ std::move(operation), std::forward<Operator>(op) }
      };
    }

    // execute lazy evaluation
    inline
    decltype(auto)
    eval
    ()
    {
      return operation(*this); // evaluate operation tree
    }

    void once(){
      if ( once_flag ) {
        eval();
        current_ = source.begin();
        once_flag = false;
      }
    }

    T current() {
      once( );
      return *current_;
    }

    bool is_end(){
      once( );
      return current_ == source.end();
    }

    bool is_next(){
      once( );
      return current_ == source.end() ? false : ++current_ != source.end();
    }

  private:
    // source source
    std::vector<T> source{};

    // Operation Tree
    Operation operation{};

    typename std::vector<T>::iterator current_{};

    bool once_flag = true;
  };


} // ! namespace stream
} // ! namespace cranberries

#endif

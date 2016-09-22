#ifndef CRANBERRIES_STREAMS_OPERATORS_CONCATENATE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_CONCATENATE_HPP
#include <utility>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {
  
  template <
    typename Branch,
    bool B = detail::is_finite_stream_v<Branch>
  >
  class Concatenate
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Concatenate( Branch stream_ ) : branch{ std::forward<Branch>(stream_) } {}
    
    Concatenate( Concatenate&& ) = default;

    Concatenate& operator=( Concatenate&& ) = default;



    // for evaluate Branch Stream only once
    void
    once()
    {
      if (flag) {
        branch.eval();
        flag = false;
      }
    }

    template <
      typename Stream,
      typename E = std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      static_assert(
        std::is_constructible<
          E,element_type_of_t<Branch>
        >::value,
        "[to stream] >> concat([from range]) requires element type of [to stream] is constructible from value tpe of [from range]."
      );

      once(); // perfectly call once
      auto&& lv = stream_.get();
      auto&& rv = branch.get();
      lv.reserve(lv.size() + rv.size());
      lv.insert(lv.end(), rv.begin(), rv.end());
      return std::forward<Stream>(stream_);
    }

    decltype( auto ) release() {return std::move(branch); }

  private:
    Branch branch; // Branch Stream
    bool flag = true;
  };


  template <
    typename Branch
  >
  class Concatenate<Branch,false>
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Concatenate( Branch stream_ ) : branch{ std::forward<Branch>( stream_ ) } {}

    Concatenate( Concatenate&& ) = default;

    Concatenate& operator=( Concatenate&& ) = default;

    template <
      typename Stream,
      typename E = std::decay_t<Stream>::element_type
    >
    inline
    decltype(auto)
    operator()
    (
      Stream&& stream_
    ) {
      static_assert(
        std::is_constructible<
          E, element_type_of_t<Branch>
        >::value,
        "[to stream] >> concat([from range]) requires element type of [to stream] is constructible from value tpe of [from range]."
      );
      using std::begin; using std::end; using cranberries::size;
      auto&& lv = stream_.get();
      lv.reserve( lv.size() + size( branch ) );
      lv.insert( lv.end(), begin( branch ), end( branch ) );
      return std::forward<Stream>( stream_ );
    }

    decltype(auto) release() { return std::move( branch ); }

  private:
    Branch branch; // Branch Stream
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
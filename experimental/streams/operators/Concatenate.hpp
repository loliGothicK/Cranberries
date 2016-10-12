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
  class Join
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Join( Branch stream_ ) noexcept
      : branch{ std::forward<Branch>(stream_) }
    {}
    
    Join( Join&& ) = default;

    Join& operator=( Join&& ) = default;



    // for evaluate Branch Stream only once
    void
    once() noexcept(false)
    {
      if (flag) {
        branch.eval();
        flag = false;
      }
    }

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
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
        std::is_constructible<
          E,element_type_of_t<Branch>
        >::value,
        "[to stream] >> joined([from range]) requires element type of [to stream] is constructible from value tpe of [from range]."
      );

      once(); // perfectly call once
      auto&& lv = stream_.get();
      auto&& rv = branch.get();
      lv.reserve(lv.size() + rv.size());
      lv.insert(lv.end(), rv.begin(), rv.end());
      return std::forward<Stream>(stream_);
    }

    decltype( auto ) release() noexcept {return std::move(branch); }

  private:
    Branch branch; // Branch Stream
    bool flag = true;
  };


  template <
    typename Branch
  >
  class Join<Branch,false>
    : private detail::IntermidiateStreamOperatorBase
  {
  public:
    Join( Branch stream_ ) noexcept
      : branch{ std::forward<Branch>( stream_ ) }
    {}

    Join( Join&& ) = default;

    Join& operator=( Join&& ) = default;

    template <
      typename Stream,
      typename E = typename std::decay_t<Stream>::element_type
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
        std::is_constructible<
          E, element_type_of_t<Branch>
        >::value,
        "[to stream] >> joined([from range]) requires element type of [to stream] is constructible from value tpe of [from range]."
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
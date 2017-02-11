#ifndef CRANBERRIES_STREAMS_OPERATORS_DISTINCT_HPP
#define CRANBERRIES_STREAMS_OPERATORS_DISTINCT_HPP
#include <utility>
#include <unordered_map>
#include <type_traits>
#include "../utility.hpp"

namespace cranberries {
namespace streams {
namespace operators {

  struct DistinctProxy{};

  // Intermidiate Operation
  template <
    typename EqualityComparable
  >
  class Distinct
    : private cranberries_magic::LazyOperationModuleBase
    , private cranberries_magic::StreamFilterBase
  {
    static_assert(
      is_equality_comparable_v<EqualityComparable>,
      "Element type of Stream must be able to equality comparable."
    );
  public:
    Distinct() = default;

    template <
      typename Stream,
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
      CRANBERRIES_STREAM_EMPTY_ERROR_THROW_IF( stream_.empty() );
      auto&& source_ = stream_.get();
      for ( auto iter = source_.begin(); iter != source_.end();)
      {
        if ( !is_exist[*iter] )
        {
          is_exist[*iter] = true;
          iter = source_.erase(iter);
        }
        else{
          ++iter;
        }
      }
      source_.shrink_to_fit();
      return std::forward<Stream>(stream_);
    }

    bool
    operator[]
    (
      EqualityComparable const& arg
    )
      noexcept
    {
      if ( !is_exist[arg] ) {
        is_exist[arg] = true;
        return false;
      }
      return true;
    }
  private:
   std::unordered_map<EqualityComparable, bool> is_exist;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries

#endif
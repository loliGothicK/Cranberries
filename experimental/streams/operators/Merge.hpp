#ifndef CRANBERRIES_STREAMS_OPERATORS_MERGE_HPP
#define CRANBERRIES_STREAMS_OPERATORS_MERGE_HPP
#include <string>
#include <iostream>
#include "..\detail\tag.hpp"

namespace cranberries {
namespace streams {
namespace operators {


  template < typename Stream >
  struct Merge
  {
    template < typename Range >
    decltype(auto)
    operator()
    (
      Range&& range
    ){
      typename std::decay_t<Range>::range_type result{};
      auto&& lv = range.get();
      auto&& rv = stream_.eval().get();
      result.reserve(lv.size()+rv.size());
      std::merge(lv.begin(), lv.end(),
             rv.begin(), rv.end(),
             std::back_inserter(result));
      result.swap( lv );
      return std::forward<Range>( range );
    }

    Stream stream_;
  };


} // ! namespace operators
} // ! namespace stream
} // ! namespace cranberries
#endif
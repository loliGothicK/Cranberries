#ifndef CRANBERRIES_STREAMS_STAT_HPP
#define CRANBERRIES_STREAMS_STAT_HPP
#include <type_traits>
#include <iterator>
#include <algorithm>
#include <numeric>

namespace cranberries{
namespace stat{

  template < typename ForwardIterator >
  inline
  auto
  median(
    ForwardIterator first,
    ForwardIterator last
  ) {
    auto len = std::distance( first, last );
    std::advance(first,len/2);
    return len%2==1?static_cast<double>(*first):(*first+*(++first))/2.0;
  }

} // ! namespace stat

namespace streams{

  template < typename T >
  class statistics
  {

  };

} // ! namespace streams
} // ! namespace cranberries

#endif

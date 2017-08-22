/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_STREAMS_STAT_HPP
#define CRANBERRIES_STREAMS_STAT_HPP
#include <type_traits>
#include <iterator>
#include <algorithm>
#include <numeric>

namespace cranberries {
namespace stat {

  template < typename ForwardIterator >
  inline
  auto
  median(
    ForwardIterator first,
    ForwardIterator last
  )
    noexcept(false)
  {
    auto len = std::distance( first, last );
    CRANBERRIES_RANGE_ERROR_THROW_IF( len < 1 );
    std::advance(first,len/2);
    return len%2==1?static_cast<double>(*first):(*first+*(++first))/2.0;
  }

} // ! namespace stat
} // ! namespace cranberries

#endif

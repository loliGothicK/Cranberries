/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_UTILITY_REVERSE_INDEX_SEQUENCE_HPP
#define CRANBERRIES_UTILITY_REVERSE_INDEX_SEQUENCE_HPP

#include <tuple>
#include <utility>
#include <type_traits>
#include "../pack_operations.hpp"

namespace cranberries {

template < size_t N >
using make_reversed_index_sequence = typename pack_reverse<std::make_index_sequence<N>>::type;

template <class F, class Tuple>
inline constexpr decltype(auto) reverse_apply(F&& f, Tuple&& t)
{
  return cranberries_magic::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
    make_reversed_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

}
#endif
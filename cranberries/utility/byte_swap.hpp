/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_UTILLITY_BYTE_SWAP_HPP
#define CRANBERRIES_UTILLITY_BYTE_SWAP_HPP
#include <algorithm>
#include <type_traits>
#include "../integers.hpp"

namespace cranberries {
template <typename T, std::enable_if_t<std::is_pod<std::decay_t<T>>::value,
                                       std::nullptr_t> = nullptr>
inline constexpr auto byte_swap(T &&v) {
  uint8_t *bytes = reinterpret_cast<uint8_t *>(&v);
  std::reverse(bytes, bytes + sizeof(T));
  return *reinterpret_cast<T *>(bytes);
}
}  // namespace cranberries
#endif
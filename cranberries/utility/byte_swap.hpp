#ifndef CRANBERRIES_UTILLITY_BYTE_SWAP_HPP
#define CRANBERRIES_UTILLITY_BYTE_SWAP_HPP
#include <algorithm>
#include "../integers.hpp"

namespace cranberries {
template < typename T >
inline constexpr auto byte_swap(T&& v) {
  uint8_t* bytes = reinterpret_cast<uint8_t*>(&v);
  std::reverse(bytes, bytes + sizeof(T));
  return *reinterpret_cast<cranberries::uint_t<sizeof(T) * 8>*>(bytes);
}
}
#endif
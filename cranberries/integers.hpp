/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_INTEGERS_HPP
#define CRANBERRIES_INTEGERS_HPP
#include <cstdint>

namespace cranberries {

template <std::size_t N>
struct int_t_traits;

template <>
struct int_t_traits<8> {
  using type = std::int8_t;
};

template <>
struct int_t_traits<16> {
  using type = std::int16_t;
};

template <>
struct int_t_traits<32> {
  using type = std::int32_t;
};

template <>
struct int_t_traits<64> {
  using type = std::int64_t;
};

template <std::size_t N>
using int_t = typename int_t_traits<N>::type;

template <std::size_t N>
struct uint_t_traits;

template <>
struct uint_t_traits<8> {
  using type = std::uint8_t;
};

template <>
struct uint_t_traits<16> {
  using type = std::uint16_t;
};

template <>
struct uint_t_traits<32> {
  using type = std::uint32_t;
};

template <>
struct uint_t_traits<64> {
  using type = std::uint64_t;
};

template <std::size_t N>
using uint_t = typename uint_t_traits<N>::type;

template <std::size_t N>
struct int_fast_t_traits;

template <>
struct int_fast_t_traits<8> {
  using type = std::int_fast8_t;
};

template <>
struct int_fast_t_traits<16> {
  using type = std::int_fast16_t;
};

template <>
struct int_fast_t_traits<32> {
  using type = std::int_fast32_t;
};

template <>
struct int_fast_t_traits<64> {
  using type = std::int_fast64_t;
};

template <std::size_t N>
using int_fast_t = typename int_fast_t_traits<N>::type;

template <std::size_t N>
struct uint_fast_t_traits;

template <>
struct uint_fast_t_traits<8> {
  using type = std::uint_fast8_t;
};

template <>
struct uint_fast_t_traits<16> {
  using type = std::uint_fast16_t;
};

template <>
struct uint_fast_t_traits<32> {
  using type = std::uint_fast32_t;
};

template <>
struct uint_fast_t_traits<64> {
  using type = std::uint_fast64_t;
};

template <std::size_t N>
using uint_fast_t = typename uint_fast_t_traits<N>::type;

}  // namespace cranberries

constexpr size_t operator"" _byte(long long unsigned i) { return i * 8; }

#endif
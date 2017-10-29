/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_MATH_HPP
#define CRANBERRIES_MATH_HPP
#include <cstdint>
#include <type_traits>

namespace cranberries {

  template < size_t R, size_t I, size_t Result, bool >
  struct meta_permutations_impl {
    static constexpr size_t value = meta_permutations_impl<R, I - 1, Result*I, !(I >= R + 1)>::value;
  };

  template < size_t R, size_t I, size_t Result >
  struct meta_permutations_impl<R, I, Result, true> {
    static constexpr size_t value = Result;
  };

  template < size_t N, size_t R >
  using meta_permutations = meta_permutations_impl<N - R, N, 1, false>;

  template < size_t N, size_t R >
  constexpr size_t meta_permutations_v = meta_permutations<N, R>::value;

  template < size_t N, size_t K, size_t D = 1, size_t Result = 1, bool = false >
  struct meta_combinations {
    static constexpr size_t value = meta_combinations<N - 1, K, D + 1, Result*N / D, (D + 1 > K)>::value;
  };

  template < size_t N, size_t K, size_t D, size_t Result >
  struct meta_combinations <N, K, D, Result, true> {
    static constexpr size_t value = Result;
  };

  template < size_t N, size_t K >
  constexpr size_t meta_combinations_v = meta_combinations<N, K>::value;


  inline constexpr std::uint_fast64_t permutations(std::uint_fast64_t n, std::uint_fast64_t r) {
    r = n - r;
    std::uint_fast64_t result = 1;
    for (std::uint_fast64_t i = n; i >= r + 1; i--)result *= i;
    return result;
  }

  inline constexpr std::uint_fast64_t combinations(std::uint_fast64_t n, std::uint_fast64_t k) {
    std::uint_fast64_t r = 1;
    for (std::uint_fast64_t d = 1; d <= k; ++d) {
      r *= n--;
      r /= d;
    }
    return r;
  }
}

#endif
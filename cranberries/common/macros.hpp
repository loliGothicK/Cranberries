/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_COMMON_MCROS_HPP
#define CRANBERRIES_COMMON_MCROS_HPP

#ifdef _MSC_VER
  #define CRANBERRIES_FORCE_INLINE __forceinline
#elif __GNUC__
  #define CRANBERRIES_FORCE_INLINE __attribute__( ( always_inline ) )
#elif __clang__
  #define CRANBERRIES_FORCE_INLINE __attribute__( ( always_inline ) )
#else
  inline
#endif


#endif // !CRANBERRIES_COMMON_MCROS_HPP
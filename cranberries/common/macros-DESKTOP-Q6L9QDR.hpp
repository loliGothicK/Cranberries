/**
 * API for Git.io
 *
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */

#ifndef CRANBERRIES_COMMON_MCROS_HPP
#define CRANBERRIES_COMMON_MCROS_HPP

#define CRANBERRIES_CONCEPT_REQUIRE(...) std::enable_if_t<bool(__VA_ARGS__), std::nullptr_t> = nullptr

#ifdef _MSC_VER
	#define CRANBERRIES_FORCE_INLINE __forceinline
#elif __GNUC__
	#define CRANBERRIES_FORCE_INLINE __attribute__( ( always_inline ) )
#elif __clang__
	#define CRANBERRIES_FORCE_INLINE __attribute__( ( always_inline ) )
#else
	inline
#endif

#define CRANERRIES_AUTO_NOEXCEPT_SPECIFIED_BODY(...) \
	noexcept(noexcept(__VA_ARGS__)) { return __VA_ARGS__; }

#define CRANERRIES_AUTO_NOEXCEPT_SPECIFIED_DECLTYPE_BODY(DECLTYPE_OPERAND, ...) \
	noexcept(noexcept(__VA_ARGS__)) -> decltype(DECLTYPE_OPERAND) { return __VA_ARGS__; }


#endif // !CRANBERRIES_COMMON_MCROS_HPP
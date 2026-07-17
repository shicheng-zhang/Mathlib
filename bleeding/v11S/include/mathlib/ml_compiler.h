#ifndef MATHLIB_COMPILER_H
#define MATHLIB_COMPILER_H

/* Compiler identity */
#if defined(_MSC_VER)
#  define MATHLIB_COMPILER_MSVC 1
#elif defined(__clang__)
#  define MATHLIB_COMPILER_CLANG 1
#elif defined(__GNUC__)
#  define MATHLIB_COMPILER_GNUC 1
#endif

/* Inline & Alignment abstraction */
#if defined(MATHLIB_COMPILER_MSVC)
#  define MATHLIB_INLINE static __forceinline
#  define MATHLIB_NOINLINE __declspec(noinline)
#  define MATHLIB_ALIGN(n) __declspec(align(n))
#  define MATHLIB_RESTRICT __restrict
#else
#  define MATHLIB_INLINE static inline __attribute__((always_inline))
#  define MATHLIB_NOINLINE __attribute__((noinline))
#  define MATHLIB_ALIGN(n) __attribute__((aligned(n)))
#  define MATHLIB_RESTRICT restrict
#endif

/* Branch prediction hints */
#if defined(MATHLIB_COMPILER_GNUC) || defined(MATHLIB_COMPILER_CLANG)
#  define MATHLIB_LIKELY(x) __builtin_expect(!!(x), 1)
#  define MATHLIB_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define MATHLIB_LIKELY(x) (x)
#  define MATHLIB_UNLIKELY(x) (x)
#endif

#endif /* MATHLIB_COMPILER_H */

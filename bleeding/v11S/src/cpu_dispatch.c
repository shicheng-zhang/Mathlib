#include "cpu_dispatch.h"
#include <stddef.h>

#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#endif

/* ============================================================================
 * SCALAR FALLBACK (Internal linkage - not exported)
 * ========================================================================== */
static void ml_matmul_scalar(const double* ML_RESTRICT A, const double* ML_RESTRICT B, double* ML_RESTRICT C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

/* ============================================================================
 * AVX2 HARDWARE KERNEL (Internal linkage - not exported)
 * ========================================================================== */
#if ML_COMPILE_TIME_AVX2
ML_TARGET_AVX2
static void ml_matmul_avx2(const double* ML_RESTRICT A, const double* ML_RESTRICT B, double* ML_RESTRICT C, int N) {
    for (int i = 0; i < N; i++) {
        int j = 0;
        /* Vectorized body for full 4-wide chunks */
        for (; j <= N - 4; j += 4) {
            __m256d c_vec = _mm256_setzero_pd();
            for (int k = 0; k < N; k++) {
                __m256d a_vec = _mm256_broadcast_sd(&A[i * N + k]);
                __m256d b_vec = _mm256_loadu_pd(&B[k * N + j]);
                c_vec = _mm256_fmadd_pd(a_vec, b_vec, c_vec);
            }
            _mm256_storeu_pd(&C[i * N + j], c_vec);
        }
        /* Scalar tail for remaining N % 4 elements (Prevents OOB write corruption) */
        for (; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}
#endif

/* ============================================================================
 * PUBLIC API: COMPILE-TIME DISPATCH
 * No function pointers. No lazy init. Zero data races.
 * ========================================================================== */
ML_API void ml_matmul(const double* ML_RESTRICT A, const double* ML_RESTRICT B, double* ML_RESTRICT C, int N) {
    if (ML_UNLIKELY(A == NULL || B == NULL || C == NULL || N <= 0)) return;

#if ML_COMPILE_TIME_AVX2
    ml_matmul_avx2(A, B, C, N);
#else
    ml_matmul_scalar(A, B, C, N);
#endif
}

/* ============================================================================
 * PUBLIC API: CAPABILITY QUERIES
 * Pure compile-time constants. Allows user code to check features safely.
 * ========================================================================== */
ML_API int ml_cpu_has_avx2(void) {
    return ML_COMPILE_TIME_AVX2;
}

ML_API int ml_cpu_has_fma(void) {
#if defined(__FMA__) && (defined(__x86_64__) || defined(__i386__))
    return 1;
#else
    return 0;
#endif
}

ML_API int ml_cpu_has_sse41(void) {
#if defined(__SSE4_1__) && (defined(__x86_64__) || defined(__i386__))
    return 1;
#else
    return 0;
#endif
}

ML_API int ml_cpu_has_neon(void) {
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
    return 1;
#else
    return 0;
#endif
}

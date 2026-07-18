#ifndef LIBMATHC_FAST_MATH_H
#define LIBMATHC_FAST_MATH_H

#include <stdint.h>
#include <string.h>
#include "ml_core.h"
#include "bitwise_fp.h"

/* ============================================================================
 * FAST MATH APPROXIMATIONS
 * Note: Uses memcpy instead of union type-punning to guarantee ISO C99
 * compliance and prevent strict-aliasing Undefined Behavior.
 * ========================================================================== */

// Quake III Fast Inverse Sqrt for 64-bit doubles
ML_INLINE double ml_fast_rsqrt(double number) {
    uint64_t bits;
    memcpy(&bits, &number, sizeof(uint64_t));
    bits = 0x5fe6ec85e7de30daULL - (bits >> 1);
    double y;
    memcpy(&y, &bits, sizeof(double));

    // Two iterations of Newton-Raphson for 64-bit double precision
    y = y * (1.5 - (number * 0.5 * y * y));
    return y * (1.5 - (number * 0.5 * y * y));
}

// Fast Log2 using the integer-float isomorphism
ML_INLINE double ml_fast_log2(double x) {
    if (x < 0.0) return 0.0 / 0.0;
    if (x == 0.0) return -1.0 / 0.0;
    if (ml_isinf(x) || ml_isnan(x)) return x;

    uint64_t bits;
    memcpy(&bits, &x, sizeof(uint64_t));

    double exp = (double)((bits >> 52) & 0x7FF) - 1023.0;
    double mant = (double)(bits & ML_FP_MANT_MASK) / 4503599627370496.0;

    // 3rd-degree Minimax polynomial for log2(1+m) on [0, 1]
    double p = mant * (1.442695 + mant * (-0.721347 + mant * 0.278652));
    return exp + p;
}

// Fast Exp2 using reverse isomorphism + Minimax fractional polynomial
ML_INLINE double ml_fast_exp2(double x) {
    long long xi = (long long)x;
    if (x < 0.0 && x != (double)xi) xi -= 1; // Bitwise floor for negative numbers

    double exp_int = (double)xi;
    double frac = x - exp_int;

    // 5th-Degree Minimax polynomial for 2^frac - 1 on [0, 1]
    double p = frac * (ML_LN2 + frac * (0.2402265069591007 + frac * (0.05550410866482158 + frac * (0.009618129107628477 + frac * 0.00133335581464249))));
    double mant_approx = 1.0 + p;

    uint64_t bits = ((uint64_t)((long long)exp_int + 1023) << 52);
    double res;
    memcpy(&res, &bits, sizeof(double));

    return res * mant_approx;
}

#endif /* LIBMATHC_FAST_MATH_H */

#ifndef LIBMATHC_IEEE754_H
#define LIBMATHC_IEEE754_H

#include <stdint.h>
#include <string.h>
#include "ml_core.h"


/* ============================================================================
 * PURE IEEE 754 BIT-MASKING TRANSCENDENTALS
 * Note: Uses memcpy instead of union type-punning to guarantee ISO C99
 * compliance and prevent strict-aliasing Undefined Behavior.
 * ========================================================================== */

// Pure IEEE 754 Bit-Masking Logarithm
ML_INLINE double logarithm_ieee754(double x) {
    if (x <= 0.0) return 0.0 / 0.0;
    if (x == 1.0) return 0.0;

    uint64_t bits;
    memcpy(&bits, &x, sizeof(uint64_t));

    // Extract exponent (bits 52-62) and subtract bias (1023)
    int e = (int)((bits >> 52) & 0x7FF) - 1023;

    // Extract mantissa and restore the hidden bit (bit 52)
    uint64_t mantissa = (bits & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;

    // Convert mantissa to double in range [1.0, 2.0)
    double m = (double)mantissa / 4503599627370496.0; // 2^52

    // Adjust to [sqrt(2)/2, sqrt(2)] for optimal series convergence
    if (m < 0.7071067811865475) {
        m *= 2.0;
        e--;
    }

    // Fast series: 2 * (z + z^3/3 + z^5/5...) where z = (m-1)/(m+1)
    double z = (m - 1.0) / (m + 1.0);
    double z2 = z * z;
    double res = z;
    double term = z;
    for (int i = 3; i <= 15; i += 2) {
        term *= z2;
        res += term / i;
    }

    return 2.0 * res + e * ML_LN2;
}

// Pure IEEE 754 Bit-Masking Exponential
ML_INLINE double exponential_ieee754(double x) {
    if (x == 0.0) return 1.0;

    // Calculate n = round(x / ln2)
    double n_d = x / ML_LN2;
    int n = (int)(n_d + (n_d > 0 ? 0.5 : -0.5));

    // Calculate remainder r = x - n * ln2
    double r = x - n * ML_LN2;

    // Taylor series for e^r (r is very small, converges instantly)
    double res = 1.0;
    double term = 1.0;
    for (int i = 1; i <= 15; i++) {
        term *= r / i;
        res += term;
    }

    // Multiply by 2^n using pure IEEE 754 exponent bit-shifting
    // FIX: Properly handle negative 'n' without uint64_t casting corruption
    uint64_t bits;
    memcpy(&bits, &res, sizeof(uint64_t));

    int64_t current_exp = (int64_t)((bits >> 52) & 0x7FF);
    int64_t new_exp = current_exp + n;

    if (new_exp <= 0) return 0.0; // Underflow
    if (new_exp >= 0x7FF) return (res < 0) ? -1.0 / 0.0 : 1.0 / 0.0; // Overflow

    bits = (bits & 0x800FFFFFFFFFFFFFULL) | ((uint64_t)new_exp << 52);

    double final_res;
    memcpy(&final_res, &bits, sizeof(double));
    return final_res;
}

#endif /* LIBMATHC_IEEE754_H */

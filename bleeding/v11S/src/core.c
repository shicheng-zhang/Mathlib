#include "ml_core.h"

ML_API double ml_fabs(double x) {
    ml_fp_cast c; c.d = x;
    c.u &= 0x7FFFFFFFFFFFFFFFULL;
    return c.d;
}

ML_API int ml_isnan(double x) {
    ml_fp_cast c; c.d = x;
    uint64_t exp = c.u & 0x7FF0000000000000ULL;
    uint64_t mant = c.u & 0x000FFFFFFFFFFFFFULL;
    return (exp == 0x7FF0000000000000ULL) && (mant != 0);
}

ML_API int ml_isinf(double x) {
    ml_fp_cast c; c.d = x;
    uint64_t exp = c.u & 0x7FF0000000000000ULL;
    uint64_t mant = c.u & 0x000FFFFFFFFFFFFFULL;
    return (exp == 0x7FF0000000000000ULL) && (mant == 0);
}

ML_API double ml_sqrt(double x) {
    if (x < 0.0) return 0.0/0.0;
    if (x == 0.0) return 0.0;
#if defined(__x86_64__) || defined(__i386__)
    double res;
    __asm__ ("sqrtsd %1, %0" : "=x" (res) : "x" (x));
    return res;
#else
    return __builtin_sqrt(x);
#endif
}

ML_API double ml_copysign(double x, double y) {
    ml_fp_cast cx, cy;
    cx.d = x; cy.d = y;
    cx.u = (cx.u & 0x7FFFFFFFFFFFFFFFULL) | (cy.u & 0x8000000000000000ULL);
    return cx.d;
}

ML_API double ml_fmod(double x, double y) {
    if (ml_isnan(x) || ml_isnan(y) || ml_isinf(x)) return 0.0/0.0;
    if (ml_isinf(y)) return x;
    if (y == 0.0) return 0.0/0.0;
    double abs_y = ml_fabs(y);
    double r = ml_fabs(x);
    if (r < abs_y) return ml_copysign(r, x);
    if (r > abs_y * 9.22e18) {
        while (r >= abs_y) r -= abs_y;
        return ml_copysign(r, x);
    }
    long long q = (long long)(r / abs_y);
    r -= (double)q * abs_y;
    if (r >= abs_y) r -= abs_y;
    return ml_copysign(r, x);
}

ML_API double ml_round(double x) {
    if (ml_isnan(x) || ml_isinf(x)) return x;
    if (x > 9.22e18) return x;
    if (x < -9.22e18) return x;
    return (x >= 0.0) ? (double)(long long)(x + 0.5) : (double)(long long)(x - 0.5);
}

ML_API double ml_ldexp_pure(double x, int exp) {
    ml_fp_cast cast; cast.d = x;
    uint64_t exp_bits = (cast.u >> 52) & 0x7FF;
    if (exp_bits == 0 && x != 0.0) {
        cast.d = x * 4503599627370496.0;
        exp_bits = (cast.u >> 52) & 0x7FF;
        exp -= 52;
    }
    if (exp_bits == 0) return x;
    int64_t new_exp = (int64_t)exp_bits - 1023 + exp;
    if (new_exp >= 1024) {
        cast.u = (cast.u & 0x8000000000000000ULL) | 0x7FF0000000000000ULL;
        return cast.d;
    }
    if (new_exp <= -1023) return 0.0;
    cast.u = (cast.u & 0x800FFFFFFFFFFFFFULL) | ((uint64_t)(new_exp + 1023) << 52);
    return cast.d;
}

ML_API double ml_frexp_pure(double x, int *exp) {
    ml_fp_cast cast; cast.d = x;
    uint64_t exp_bits = (cast.u >> 52) & 0x7FF;

    if (exp_bits == 0) {
        if (x == 0.0) { *exp = 0; return x; }
        // Normalize subnormal by multiplying by 2^52
        double norm = x * 4503599627370496.0;
        ml_fp_cast cast2; cast2.d = norm;
        exp_bits = (cast2.u >> 52) & 0x7FF;
        *exp = (int)exp_bits - 1022 - 52;
        cast2.u = (cast2.u & 0x800FFFFFFFFFFFFFULL) | 0x3FE0000000000000ULL;
        return cast2.d;
    }

    *exp = (int)exp_bits - 1022;
    cast.u = (cast.u & 0x800FFFFFFFFFFFFFULL) | 0x3FE0000000000000ULL;
    return cast.d;
}


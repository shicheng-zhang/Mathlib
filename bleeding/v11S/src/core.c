#include "ml_compiler.h"
#include "ml_core.h"
#include "internal/ieee_exact.h"

ML_API double ml_sqrt(double x) {
    if (x < 0.0) return ml_make_nan();
    if (x == 0.0) return x;

#if defined(__x86_64__) || defined(__i386__)
    double res;
    __asm__ ("sqrtsd %1, %0" : "=x" (res) : "x" (x));
    return res;
#else
    return __builtin_sqrt(x);
#endif
}

ML_API double ml_ldexp_pure(double x, int exp) {
    ml_fp_parts_t p = ml_fp_decompose(x);

    if (p.kind == ML_FP_ZERO || p.kind == ML_FP_INF || p.kind == ML_FP_NAN) {
        return x;
    }

    long long new_exp = (long long)p.exp + (long long)exp;

    if (new_exp > 971) {
        return ml_make_inf(p.sign);
    }

    if (new_exp < -1200) {
        uint64_t bits = p.sign ? 0x8000000000000000ULL : 0ULL;
        double d;
        memcpy(&d, &bits, sizeof(double));
        return d;
    }

    return ml_fp_compose(p.sig, (int)new_exp, p.sign);
}

ML_API double ml_frexp_pure(double x, int *exp) {
    if (!exp) return ml_make_nan();

    ml_fp_parts_t p = ml_fp_decompose(x);

    if (p.kind == ML_FP_ZERO) {
        *exp = 0;
        return x;
    }

    if (p.kind == ML_FP_INF || p.kind == ML_FP_NAN) {
        *exp = 0;
        return x;
    }

    uint64_t sig = p.sig;
    int e = p.exp;

    while (sig < (1ULL << 52)) {
        sig <<= 1;
        e--;
    }

    double m = (double)sig / 9007199254740992.0; /* 2^53 */
    *exp = e + 53;

    return p.sign ? -m : m;
}

ML_API double ml_fmod(double x, double y) {
    /* v11S AUDIT IP-1: exact fmod for all finite nonzero inputs */
    if (ml_isnan(x) || ml_isnan(y) || ml_isinf(x)) return ml_make_nan();
    if (ml_isinf(y)) return x;
    if (y == 0.0) return ml_make_nan();

    double ax = ml_fabs(x);
    double ay = ml_fabs(y);

    if (ax < ay) return x;
    if (ax == ay) return ml_copysign(0.0, x);

    ml_fp_parts_t px = ml_fp_decompose(ax);
    ml_fp_parts_t py = ml_fp_decompose(ay);

    if (px.sig == 0) return ml_copysign(0.0, x);
    if (py.sig == 0) return ml_make_nan();

    int d = px.exp - py.exp;

    if (d < 0) {
        int sd = -d;

        /*
         * For |x| >= |y| this should not happen except in exotic subnormal
         * alignments. If it does, align y to x's exponent exactly when safe.
         */
        if (sd >= 64 || py.sig > (UINT64_MAX >> sd)) {
            return ml_make_nan();
        }

        uint64_t ysig = py.sig << sd;
        uint64_t rem = px.sig % ysig;

        return ml_fp_compose(rem, px.exp, ml_signbit(x));
    }

    uint64_t rem = px.sig % py.sig;

    if (rem == 0) {
        return ml_copysign(0.0, x);
    }

    for (int i = 0; i < d; i++) {
        rem <<= 1;
        if (rem >= py.sig) rem -= py.sig;
        if (rem == 0) break;
    }

    return ml_fp_compose(rem, py.exp, ml_signbit(x));
}

ML_API double ml_round(double x) {
    if (ml_isnan(x) || ml_isinf(x)) return x;
    if (x == 0.0) return x;

    if (x > 4503599627370496.0) return x;
    if (x < -4503599627370496.0) return x;

    return (x >= 0.0)
        ? (double)(long long)(x + 0.5)
        : (double)(long long)(x - 0.5);
}

#ifndef ML_CORE_H
#define ML_CORE_H

#include <stdint.h>
#include <string.h>
#include "ml_compiler.h"

/* Safe IEEE-754 Specials (UBSan-proof, MSVC-compatible) */
static inline double ml_make_inf(int negative) {
    uint64_t bits = negative ? 0xFFF0000000000000ULL : 0x7FF0000000000000ULL;
    double val; memcpy(&val, &bits, sizeof(double)); return val;
}
static inline double ml_make_nan(void) {
    uint64_t bits = 0x7FF8000000000000ULL;
    double val; memcpy(&val, &bits, sizeof(double)); return val;
}

ML_INLINE double ml_fabs(double x) {
    uint64_t bits; memcpy(&bits, &x, sizeof(uint64_t));
    bits &= 0x7FFFFFFFFFFFFFFFULL; memcpy(&x, &bits, sizeof(double)); return x;
}
ML_INLINE int ml_isnan(double x) {
    uint64_t bits; memcpy(&bits, &x, sizeof(uint64_t));
    return ((bits & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL) && ((bits & 0x000FFFFFFFFFFFFFULL) != 0);
}
ML_INLINE int ml_isinf(double x) {
    uint64_t bits; memcpy(&bits, &x, sizeof(uint64_t));
    return ((bits & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL) && ((bits & 0x000FFFFFFFFFFFFFULL) == 0);
}
ML_INLINE double ml_copysign(double x, double y) {
    uint64_t bx, by; memcpy(&bx, &x, sizeof(uint64_t)); memcpy(&by, &y, sizeof(uint64_t));
    bx = (bx & 0x7FFFFFFFFFFFFFFFULL) | (by & 0x8000000000000000ULL);
    memcpy(&x, &bx, sizeof(double)); return x;
}
ML_INLINE double ml_ldexp_pure(double x, int exp) {
    uint64_t bits; memcpy(&bits, &x, sizeof(uint64_t));
    int64_t current_exp = (int64_t)((bits >> 52) & 0x7FF);
    int64_t new_exp = current_exp + exp;
    if (new_exp <= 0) return 0.0; /* Flush-To-Zero (FTZ) for performance; does not generate subnormals. */
    if (new_exp >= 0x7FF) return ml_make_inf(x < 0);
    bits = (bits & 0x800FFFFFFFFFFFFFULL) | ((uint64_t)new_exp << 52);
    memcpy(&x, &bits, sizeof(double)); return x;
}
ML_INLINE double ml_frexp_pure(double x, int *exp) {
    uint64_t bits; memcpy(&bits, &x, sizeof(uint64_t));
    uint64_t exp_bits = (bits >> 52) & 0x7FF;
    if (exp_bits == 0) {
        if (x == 0.0) { *exp = 0; return x; }
        double norm = x * 4503599627370496.0; memcpy(&bits, &norm, sizeof(uint64_t));
        exp_bits = (bits >> 52) & 0x7FF; *exp = (int)exp_bits - 1022 - 52;
        bits = (bits & 0x800FFFFFFFFFFFFFULL) | 0x3FE0000000000000ULL;
        memcpy(&x, &bits, sizeof(double)); return x;
    }
    *exp = (int)exp_bits - 1022;
    bits = (bits & 0x800FFFFFFFFFFFFFULL) | 0x3FE0000000000000ULL;
    memcpy(&x, &bits, sizeof(double)); return x;
}

ML_API double ml_sqrt(double x);
ML_API double ml_fmod(double x, double y);
ML_API double ml_round(double x);

#ifndef ML_PI
#define ML_PI 3.14159265358979323846
#endif
#ifndef ML_E
#define ML_E 2.71828182845904523536
#endif
#ifndef ML_LN2
#define ML_LN2 0.693147180559945309417
#endif

#endif /* ML_CORE_H */

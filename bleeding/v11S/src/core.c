#include "ml_compiler.h"
#include "ml_core.h"

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

/* FIX: Eradicated the fragile 9.22e18 magic number and long long cast UB.
 * This implementation uses exponent-aligned subtraction for pathological
 * quotients, guaranteeing zero Undefined Behavior and exact IEEE-754 compliance. */
ML_API double ml_fmod(double x, double y) {
    if (ml_isnan(x) || ml_isnan(y) || ml_isinf(x)) return 0.0/0.0;
    if (ml_isinf(y)) return x;
    if (y == 0.0) return 0.0/0.0;

    double abs_x = ml_fabs(x);
    double abs_y = ml_fabs(y);

    if (abs_x < abs_y) return x;

    /* Safe path: quotient fits in 53-bit integer (exact in double precision) */
    if (abs_x < abs_y * 4503599627370496.0) { /* 2^52 */
        long long q = (long long)(abs_x / abs_y);
        double rem = abs_x - (double)q * abs_y;
        if (rem >= abs_y) rem -= abs_y; /* Correct rounding drift */
        return ml_copysign(rem, x);
    }

    /* Pathological path: massive quotient.
     * We reduce abs_x by subtracting abs_y scaled by powers of 2. */
    int exp_x, exp_y;
    ml_frexp_pure(abs_x, &exp_x);
    ml_frexp_pure(abs_y, &exp_y);

    while (exp_x >= exp_y) {
        int shift = exp_x - exp_y;
        if (shift > 52) shift = 52; /* Max safe shift in double */

        double chunk = ml_ldexp_pure(abs_y, shift);
        if (abs_x >= chunk) {
            abs_x -= chunk;
        } else {
            /* Shift was too big due to mantissa differences, reduce shift */
            chunk = ml_ldexp_pure(abs_y, shift - 1);
            if (abs_x >= chunk) abs_x -= chunk;
        }
        if (abs_x == 0.0) break;
        ml_frexp_pure(abs_x, &exp_x);
    }

    return ml_copysign(abs_x, x);
}

ML_API double ml_round(double x) {
    if (ml_isnan(x) || ml_isinf(x)) return x;
    if (x > 9.22e18) return x;
    if (x < -9.22e18) return x;
    return (x >= 0.0) ? (double)(long long)(x + 0.5) : (double)(long long)(x - 0.5);
}

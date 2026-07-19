#include "ml_compiler.h"
#include "ml_integral.h"

ML_API double ml_factorial_float(double x) {
    if (x < 0.0) return ml_make_nan();
    if (x == 0.0) return 1.0; // Stirling's approximation fails near 0; hardcode exact
    return ml_sqrt(2.0 * MATHLIB_PI * x) * ml_pow((x / MATHLIB_E), x);
}

ML_API double ml_integral_traditional(double a, double b, double exponent, double additive, double d) {
    if (d == 0.0 || ml_isnan(d) || ml_isnan(a) || ml_isnan(b)) return ml_make_nan();
    if ((d > 0 && a >= b) || (d < 0 && a <= b)) return 0.0;

    double result = 0.0;
    double x = a;
    const int max_steps = 10000000; // O(1) temporal ceiling

    for (int step = 0; step < max_steps; step++) {
        if ((d > 0 && x >= b) || (d < 0 && x <= b)) return result;

        result += (ml_pow(x, exponent) + additive) * d;

        /* ULTRA-NITTY-GRITTY FIX: Floating-Point Stall Detection
         * If 'd' is so small relative to 'x' that x + d == x due to IEEE-754
         * ULP absorption limits, the loop will never terminate naturally.
         * We detect the stall and abort immediately to save CPU cycles. */
        double next_x = x + d;
        if (ML_UNLIKELY(next_x == x)) {
            return ml_make_nan(); // Signal failure due to precision stall
        }
        x = next_x;
    }

    /* Step limit exceeded: signal failure instead of returning a wrong answer */
    return ml_make_nan();
}

ML_API double ml_gamma_new(double x) {
    if (ml_isnan(x)) return ml_make_nan();
    if (x <= 0.0) return ml_make_nan(); // Undefined for 0 and negative integers
    if (x > 171.0) return ml_make_inf(0); // 172! overflows double (approx 1.24e309)

    /* Iterative reduction to prevent stack overflow on large inputs */
    double result = 1.0;
    while (x > 2.0) {
        x -= 1.0;
        result *= x;
    }

    if (x < 1.0) {
        /* Bounded recursion: x+1 will be in [1.0, 2.0), hitting the base case
         * immediately on the next call. Max stack depth is strictly 2. */
        return result * ml_gamma_new(x + 1.0) / x;
    }

    /* Base case: x is in [1.0, 2.0]
     * Minimax/Lanczos approximation polynomial for the [1,2] domain */
    double z = x - 1.0;
    double p = -0.193527818 + z * 0.035868343;
    p = 0.482199394 + z * p;
    p = -0.756704078 + z * p;
    p = 0.918206857 + z * p;
    p = -0.897056937 + z * p;
    p = 0.989028236 + z * p;
    p = -0.577191652 + z * p;
    return result * (1.0 + z * p);
}

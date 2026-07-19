#include "ml_compiler.h"
#include "ml_exp_log.h"

ML_API double ml_exp(double x) {
    if (x == 0.0) return 1.0;
    if (x > 709.78) return ml_make_inf(0);
    if (x < -745.13) return 0.0;

    /* Cody-Waite extended precision reduction (Already branchless) */
    double n = ml_round(x / ML_LN2);
    double r = x - n * 0.69314718036912381649 - n * 1.90821490974462528503e-10;

    static const double inv_fact[] = {
        1.0, 1.0, 0.5, 0.16666666666666666, 0.041666666666666664,
        0.008333333333333333, 0.001388888888888889, 0.0001984126984126984,
        2.48015873015873e-05, 2.7557319223985893e-06, 2.7557319223985888e-07,
        2.505210838544172e-08, 2.08767569878681e-09, 1.6059043836821613e-10,
        1.1470745597729725e-11, 7.647163731819816e-13, 4.779477332387385e-14,
        2.8114572543455206e-15, 1.5619206968586226e-16, 8.22063524662433e-18
    };

    /* Horner's Method (Branchless) */
    double result = inv_fact[19];
    for (int i = 18; i >= 1; i--) result = ML_FMA(result, r, inv_fact[i]);
    result = ML_FMA(result, r, 1.0);

    return ml_ldexp_pure(result, (int)n);
}

ML_API double ml_log(double x) {
    if (x == 0.0) return -ml_make_inf(0);
    if (x < 0.0) return ml_make_nan();
    if (x == 1.0) return 0.0;

    int e;
    double m = ml_frexp_pure(x, &e);

    /* BRANCHLESS SIMD PREP:
     * Replaces the `if (m < 0.707...)` branch with a branchless arithmetic mask.
     * This prevents SIMD execution divergence and allows the compiler to
     * auto-vectorize this function across arrays using blend instructions. */
    int adjust = (m < 0.7071067811865475);
    m *= (1.0 + adjust); /* If adjust=1, m*=2.0. If 0, m*=1.0 */
    e -= adjust;

    double z = (m - 1.0) / (m + 1.0);
    double z2 = z * z;

    /* Horner's Method for 2 * atanh(z) (Branchless) */
    double poly = 0.09523809523809523;
    poly = poly * z2 + 0.10526315789473684;
    poly = poly * z2 + 0.11764705882352941;
    poly = poly * z2 + 0.13333333333333333;
    poly = poly * z2 + 0.15384615384615385;
    poly = poly * z2 + 0.18181818181818182;
    poly = poly * z2 + 0.2222222222222222;
    poly = poly * z2 + 0.2857142857142857;
    poly = poly * z2 + 0.4;
    poly = poly * z2 + 0.6666666666666666;
    poly = poly * z2 + 2.0;

    return z * poly + e * ML_LN2;
}

ML_API double ml_pow(double x, double y) { return ml_exp(y * ml_log(x)); }
ML_API double ml_logb(double x, double b) { return ml_log(x) / ml_log(b); }
ML_API double ml_sinh(double x) { return (ml_exp(x) - ml_exp(-x)) / 2.0; }
ML_API double ml_cosh(double x) { return (ml_exp(x) + ml_exp(-x)) / 2.0; }
ML_API double ml_tanh(double x) { return ml_sinh(x) / ml_cosh(x); }
ML_API double ml_asinh(double x) { return ml_log(x + ml_sqrt(x * x + 1.0)); }
ML_API double ml_acosh(double x) { return (x < 1.0) ? ml_make_nan() : ml_log(x + ml_sqrt(x * x - 1.0)); }
ML_API double ml_atanh(double x) { return (x <= -1.0 || x >= 1.0) ? ml_make_nan() : 0.5 * ml_log((1.0 + x) / (1.0 - x)); }

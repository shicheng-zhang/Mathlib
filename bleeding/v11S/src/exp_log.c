#include "ml_compiler.h"
#include "ml_exp_log.h"
#include "internal/hypot.h"

/* ============================================================================
 * v11S CLOSURE: EXP / LOG / POW / HYPERBOLIC EDGE-CASE REPAIR
 *
 * This file has been rewritten to remove naive overflow traps and
 * incomplete special-case behavior.
 * ========================================================================== */

ML_API double ml_exp(double x) {
    if (ml_isnan(x)) return x;
    if (ml_isinf(x)) return x > 0.0 ? ml_make_inf(0) : 0.0;

    if (x == 0.0) return 1.0;
    if (x > 709.782712893384) return ml_make_inf(0);
    if (x < -745.133219101941) return 0.0;

    /* Cody-Waite extended precision reduction */
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

    double result = inv_fact[19];
    for (int i = 18; i >= 1; i--) {
        result = ML_FMA(result, r, inv_fact[i]);
    }
    result = ML_FMA(result, r, 1.0);

    return ml_ldexp_pure(result, (int)n);
}

ML_API double ml_log(double x) {
    if (ml_isnan(x)) return x;
    if (ml_isinf(x)) return x > 0.0 ? ml_make_inf(0) : ml_make_nan();

    if (x == 0.0) return -ml_make_inf(0);
    if (x < 0.0) return ml_make_nan();
    if (x == 1.0) return 0.0;

    int e;
    double m = ml_frexp_pure(x, &e);

    /* Branchless sqrt(2) balancing */
    int adjust = (m < 0.7071067811865475);
    m *= (1.0 + adjust);
    e -= adjust;

    double z = (m - 1.0) / (m + 1.0);
    double z2 = z * z;

    /* Horner evaluation for 2 * atanh(z) */
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

/*
 * Integer detection helper for pow().
 *
 * For |y| >= 2^52, every finite double is an integer, and all such
 * representable integers are even. Therefore odd = 0 in that range.
 */
static int ml_is_integer_double(double y, int *is_odd) {
    if (ml_isnan(y) || ml_isinf(y)) {
        *is_odd = 0;
        return 0;
    }

    if (ml_fabs(y) >= 4503599627370496.0) {
        *is_odd = 0;
        return 1;
    }

    double r = ml_round(y);
    if (r != y) {
        *is_odd = 0;
        return 0;
    }

    long long yi = (long long)r;
    *is_odd = (int)(yi & 1LL);
    return 1;
}

ML_API double ml_pow(double x, double y) {
    /* C99-style special cases first */
    if (y == 0.0) return 1.0;
    if (x == 1.0) return 1.0;

    if (ml_isnan(x) || ml_isnan(y)) {
        return ml_make_nan();
    }

    int odd = 0;
    int y_is_int = ml_is_integer_double(y, &odd);

    if (ml_isinf(y)) {
        double ax = ml_fabs(x);
        if (ax == 1.0) return 1.0;

        if (y > 0.0) {
            return (ax > 1.0) ? ml_make_inf(0) : 0.0;
        } else {
            return (ax > 1.0) ? 0.0 : ml_make_inf(0);
        }
    }

    if (ml_isinf(x)) {
        if (y > 0.0) {
            return (x < 0.0 && odd) ? -ml_make_inf(0) : ml_make_inf(0);
        } else {
            return (x < 0.0 && odd) ? -0.0 : 0.0;
        }
    }

    if (x == 0.0) {
        if (y > 0.0) {
            return (x < 0.0 && y_is_int && odd) ? -0.0 : 0.0;
        } else {
            return (x < 0.0 && y_is_int && odd) ? -ml_make_inf(0) : ml_make_inf(0);
        }
    }

    if (x < 0.0) {
        if (!y_is_int) return ml_make_nan();

        double mag = ml_exp(y * ml_log(-x));
        return odd ? -mag : mag;
    }

    return ml_exp(y * ml_log(x));
}

ML_API double ml_logb(double x, double b) {
    if (ml_isnan(x) || ml_isnan(b)) return ml_make_nan();
    if (b <= 0.0 || b == 1.0) return ml_make_nan();
    if (x < 0.0) return ml_make_nan();

    if (ml_isinf(b)) {
        return ml_isinf(x) ? ml_make_nan() : 0.0;
    }

    double lb = ml_log(b);

    if (x == 0.0) {
        return ml_make_inf(lb > 0.0 ? 1 : 0);
    }

    if (ml_isinf(x)) {
        return ml_make_inf(lb > 0.0 ? 0 : 1);
    }

    return ml_log(x) / lb;
}

ML_API double ml_sinh(double x) {
    if (ml_isnan(x)) return x;
    if (ml_isinf(x)) return x;

    double ax = ml_fabs(x);
    if (ax == 0.0) return x;

    if (ax > 709.782712893384) {
        return ml_make_inf(x < 0.0);
    }

    double ep = ml_exp(ax);
    double em = ml_exp(-ax);
    double r = 0.5 * (ep - em);

    return (x < 0.0) ? -r : r;
}

ML_API double ml_cosh(double x) {
    if (ml_isnan(x)) return x;
    if (ml_isinf(x)) return ml_make_inf(0);

    double ax = ml_fabs(x);

    if (ax > 709.782712893384) {
        return ml_make_inf(0);
    }

    double ep = ml_exp(ax);
    double em = ml_exp(-ax);

    return 0.5 * (ep + em);
}

ML_API double ml_tanh(double x) {
    if (ml_isnan(x)) return x;
    if (ml_isinf(x)) return ml_copysign(1.0, x);

    double ax = ml_fabs(x);

    if (ax == 0.0) return x;
    if (ax > 20.0) return ml_copysign(1.0, x);
    if (ax < 1e-4) return x;

    double e = ml_exp(-2.0 * ax);
    double t = (1.0 - e) / (1.0 + e);

    return ml_copysign(t, x);
}

ML_API double ml_asinh(double x) {
    if (ml_isnan(x) || ml_isinf(x)) return x;

    double ax = ml_fabs(x);
    if (ax == 0.0) return x;
    if (ax < 1e-4) return x;

    double r = ml_log(ax + ml_hypot_internal(ax, 1.0));
    return (x < 0.0) ? -r : r;
}

ML_API double ml_acosh(double x) {
    if (ml_isnan(x)) return x;
    if (x < 1.0) return ml_make_nan();
    if (x == 1.0) return 0.0;
    if (ml_isinf(x)) return x;

    if (x > 1e150) {
        return ml_log(2.0) + ml_log(x);
    }

    return ml_log(x + ml_sqrt((x - 1.0) * (x + 1.0)));
}

ML_API double ml_atanh(double x) {
    if (ml_isnan(x)) return x;
    if (x <= -1.0 || x >= 1.0) return ml_make_nan();

    if (ml_fabs(x) < 1e-4) return x;

    return 0.5 * ml_log((1.0 + x) / (1.0 - x));
}

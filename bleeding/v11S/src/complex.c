#include "ml_compiler.h"
#include "ml_complex.h"
#include "internal/hypot.h"

/* ============================================================================
 * v11S CLOSURE: COMPLEX NUMERICS REPAIR
 *
 * This file has been rewritten to remove overflow traps and improve
 * special-case behavior for NaN / Inf / zero inputs.
 * ========================================================================== */

ML_API double ml_cplx_abs(cplx a) {
    return ml_hypot_internal(a.real, a.imag);
}

ML_API double ml_cplx_arg(cplx a) {
    if (ml_isnan(a.real) || ml_isnan(a.imag)) {
        return ml_make_nan();
    }

    return ml_atan2(a.imag, a.real);
}

/*
 * Smith's method for complex division.
 *
 * This is retained because it is already much better than the naive
 * (a+bi)/(c+di) formula for overflow/underflow behavior.
 */
ML_API cplx ml_cplx_div(cplx a, cplx b) {
    if (ml_isnan(a.real) || ml_isnan(a.imag) ||
        ml_isnan(b.real) || ml_isnan(b.imag)) {
        return (cplx){ml_make_nan(), ml_make_nan()};
    }

    if (ml_fabs(b.real) >= ml_fabs(b.imag)) {
        if (b.real == 0.0) {
            return (cplx){ml_make_nan(), ml_make_nan()};
        }

        double ratio = b.imag / b.real;
        double denom = b.real + ratio * b.imag;

        return (cplx){
            (a.real + a.imag * ratio) / denom,
            (a.imag - a.real * ratio) / denom
        };
    } else {
        if (b.imag == 0.0) {
            return (cplx){ml_make_nan(), ml_make_nan()};
        }

        double ratio = b.real / b.imag;
        double denom = b.imag + ratio * b.real;

        return (cplx){
            (a.real * ratio + a.imag) / denom,
            (a.imag * ratio - a.real) / denom
        };
    }
}

ML_API cplx ml_cplx_exponential(cplx a) {
    if (ml_isnan(a.real) || ml_isnan(a.imag)) {
        return (cplx){ml_make_nan(), ml_make_nan()};
    }

    /*
     * If the imaginary part is infinite, cos/sin are undefined.
     * Return NaN instead of producing garbage.
     */
    if (ml_isinf(a.imag)) {
        return (cplx){ml_make_nan(), ml_make_nan()};
    }

    if (ml_isinf(a.real)) {
        double c = ml_cos(a.imag);
        double s = ml_sin(a.imag);

        if (a.real < 0.0) {
            return (cplx){0.0 * c, 0.0 * s};
        }

        double re = (c == 0.0) ? ml_copysign(0.0, c)
                               : ml_copysign(ml_make_inf(0), c);
        double im = (s == 0.0) ? ml_copysign(0.0, s)
                               : ml_copysign(ml_make_inf(0), s);

        return (cplx){re, im};
    }

    double mag = ml_exp(a.real);
    double c = ml_cos(a.imag);
    double s = ml_sin(a.imag);

    return (cplx){mag * c, mag * s};
}

ML_API cplx ml_cplx_logarithm(cplx a) {
    if (ml_isnan(a.real) || ml_isnan(a.imag)) {
        return (cplx){ml_make_nan(), ml_make_nan()};
    }

    double abs_a = ml_cplx_abs(a);
    double arg_a = ml_cplx_arg(a);

    return (cplx){ml_log(abs_a), arg_a};
}

ML_API cplx ml_cplx_power(cplx a, cplx b) {
    if (ml_isnan(a.real) || ml_isnan(a.imag) ||
        ml_isnan(b.real) || ml_isnan(b.imag)) {
        return (cplx){ml_make_nan(), ml_make_nan()};
    }

    cplx log_a = ml_cplx_logarithm(a);

    return ml_cplx_exponential((cplx){
        b.real * log_a.real - b.imag * log_a.imag,
        b.real * log_a.imag + b.imag * log_a.real
    });
}

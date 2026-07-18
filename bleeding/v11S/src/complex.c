#include "ml_compiler.h"
#include "ml_complex.h"

ML_API double ml_cplx_abs(cplx a) { return ml_sqrt(a.real * a.real + a.imag * a.imag); }

ML_API double ml_cplx_arg(cplx a) {
    if (a.real > 0) return ml_atan(a.imag / a.real);
    if (a.real < 0 && a.imag >= 0) return ml_atan(a.imag / a.real) + ML_PI;
    if (a.real < 0 && a.imag < 0) return ml_atan(a.imag / a.real) - ML_PI;
    if (a.real == 0 && a.imag > 0) return ML_PI / 2.0;
    if (a.real == 0 && a.imag < 0) return -ML_PI / 2.0;
    return 0.0 / 0.0;
}

/* FIX: Smith's Method to prevent intermediate overflow/underflow in complex division */
ML_API cplx ml_cplx_div(cplx a, cplx b) {
    double ratio, denom;
    if (ml_fabs(b.real) >= ml_fabs(b.imag)) {
        if (b.real == 0.0) return (cplx){0.0/0.0, 0.0/0.0};
        ratio = b.imag / b.real;
        denom = b.real + ratio * b.imag;
        return (cplx){(a.real + a.imag * ratio) / denom, (a.imag - a.real * ratio) / denom};
    } else {
        if (b.imag == 0.0) return (cplx){0.0/0.0, 0.0/0.0};
        ratio = b.real / b.imag;
        denom = b.imag + ratio * b.real;
        return (cplx){(a.real * ratio + a.imag) / denom, (a.imag * ratio - a.real) / denom};
    }
}

ML_API cplx ml_cplx_exponential(cplx a) {
    double mag = ml_exp(a.real);
    return (cplx){mag * ml_cos(a.imag), mag * ml_sin(a.imag)};
}

ML_API cplx ml_cplx_logarithm(cplx a) {
    return (cplx){ml_log(ml_cplx_abs(a)), ml_cplx_arg(a)};
}

ML_API cplx ml_cplx_power(cplx a, cplx b) {
    cplx log_a = ml_cplx_logarithm(a);
    return ml_cplx_exponential((cplx){b.real * log_a.real - b.imag * log_a.imag, b.real * log_a.imag + b.imag * log_a.real});
}

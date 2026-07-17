#include "ml_complex.h"

ML_API cplx cplx_add (cplx a, cplx b) {return (cplx) {a.real + b.real, a.imag + b.imag};}

ML_API cplx cplx_sub (cplx a, cplx b) {return (cplx) {a.real - b.real, a.imag - b.imag};}

ML_API cplx cplx_mul (cplx a, cplx b) {
    return (cplx) {a.real * b.real - a.imag * b.imag, a.real * b.imag + a.imag * b.real};
}

ML_API cplx cplx_div (cplx a, cplx b) {
    double denom = b.real * b.real + b.imag * b.imag;
    if (denom == 0.0) return (cplx){0.0/0.0, 0.0/0.0};
    return (cplx) {(a.real * b.real + a.imag * b.imag) / denom, (a.imag * b.real - a.real * b.imag) / denom};
}

ML_API double cplx_abs (cplx a) {return ml_sqrt(a.real * a.real + a.imag * a.imag);}

ML_API double cplx_arg (cplx a) {
    if (a.real > 0) {return ml_atan (a.imag / a.real);}
    if (a.real < 0 && a.imag >= 0) {return ml_atan (a.imag / a.real) + math_pi;}
    if (a.real < 0 && a.imag < 0) {return ml_atan (a.imag / a.real) - math_pi;}
    if (a.real == 0 && a.imag > 0) {return math_pi / 2;}
    if (a.real == 0 && a.imag < 0) {return -math_pi / 2;}
    return 0.0 / 0.0;
}

ML_API cplx cplx_conjugate (cplx a) {return (cplx) {a.real, -a.imag};}

ML_API cplx cplx_exponential (cplx a) {
    double mag = ml_exp (a.real);
    return (cplx) {mag * ml_cos (a.imag), mag * ml_sin (a.imag)};
}

ML_API cplx cplx_logarithm (cplx a) {
    return (cplx) {ml_log (cplx_abs (a)), cplx_arg (a)};
}

ML_API cplx cplx_power (cplx a, cplx b) {
    cplx log_a = cplx_logarithm (a);
    return cplx_exponential ((cplx) {b.real * log_a.real - b.imag * log_a.imag, b.real * log_a.imag + b.imag * log_a.real});
}


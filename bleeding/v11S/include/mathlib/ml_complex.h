#ifndef LIBMATHC_ML_COMPLEX_H
#define LIBMATHC_ML_COMPLEX_H

//Library header file for complex numbers
#include "ml_core.h"
#include "ml_exp_log.h"
#include "ml_trig.h"
typedef struct { double real; double imag; } cplx;
cplx cplx_add (cplx a, cplx b);

cplx cplx_sub (cplx a, cplx b);

cplx cplx_mul (cplx a, cplx b);
cplx cplx_div (cplx a, cplx b);
double cplx_abs (cplx a);

double cplx_arg (cplx a);
cplx cplx_conjugate (cplx a);

cplx cplx_exponential (cplx a);
cplx cplx_logarithm (cplx a);
cplx cplx_power (cplx a, cplx b);




#endif

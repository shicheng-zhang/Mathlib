#ifndef LIBMATHC_FFT_H
#define LIBMATHC_FFT_H

#include "ml_compiler.h"
#include "ml_complex.h"
#include "ml_core.h"

ML_API void ml_fft_execute(cplx *x, int n);
ML_API void ml_ifft_execute(cplx *x, int n);

#endif /* LIBMATHC_FFT_H */

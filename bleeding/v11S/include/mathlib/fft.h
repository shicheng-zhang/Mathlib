#ifndef LIBMATHC_FFT_H
#define LIBMATHC_FFT_H
#include "ml_complex.h"
#include "ml_core.h"
int is_power_of_two(int n);

void fft_execute(cplx *x, int n);
 void ifft_execute(cplx *x, int n);

#endif

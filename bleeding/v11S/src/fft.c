#include "ml_compiler.h"
#include "fft.h"

static inline int is_power_of_two(int n) { return (n > 0) && ((n & (n - 1)) == 0); }

ML_API void ml_fft_execute(cplx *x, int n) {
    if (!is_power_of_two(n)) return;

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) { j ^= bit; }
        j ^= bit;
        if (i < j) { cplx temp = x[i]; x[i] = x[j]; x[j] = temp; }
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = -2.0 * ML_PI / len;
        cplx wlen = {ml_cos(ang), ml_sin(ang)};
        for (int i = 0; i < n; i += len) {
            cplx w = {1.0, 0.0};
            for (int j = 0; j < len / 2; j++) {
                if ((j & 63) == 0) {
                    double theta = ang * j;
                    w = (cplx){ml_cos(theta), ml_sin(theta)};
                }
                cplx u = x[i + j];
                cplx v = ml_cplx_mul(x[i + j + len / 2], w);
                x[i + j] = ml_cplx_add(u, v);
                x[i + j + len / 2] = ml_cplx_sub(u, v);
                w = ml_cplx_mul(w, wlen);
            }
        }
    }
}

ML_API void ml_ifft_execute(cplx *x, int n) {
    if (!is_power_of_two(n)) return;
    for (int i = 0; i < n; i++) x[i].imag = -x[i].imag;
    ml_fft_execute(x, n);
    for (int i = 0; i < n; i++) {
        x[i].imag = -x[i].imag;
        x[i].real /= n;
        x[i].imag /= n;
    }
}

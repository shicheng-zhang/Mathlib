#include "payne_hanek.h"

ML_API int ml_rem_pio2(double x, double *y) {
    if (ml_isnan(x) || ml_isinf(x)) {
        *y = 0.0/0.0;
        return 0;
    }

    // Estimate n = round(x / (pi/2))
    double fn = ml_round(x * 0.63661977236758134308); // 2/pi
    int n = (int)fn;

    // Cody-Waite extended precision subtraction
    double r1 = x - fn * PIO2_HI;
    double r2 = -fn * PIO2_LO;

    *y = r1 + r2;
    return n & 3;
}


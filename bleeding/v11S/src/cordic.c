#include "cordic.h"

ML_API void ml_cordic_sincos(double theta, double *sin_out, double *cos_out) {
    // 1. Range reduction to [-pi, pi]
    theta = ml_fmod(theta, 2.0 * M_PI);
    if (theta > M_PI) theta -= 2.0 * M_PI;
    if (theta < -M_PI) theta += 2.0 * M_PI;

    // 2. Quadrant Mapping: CORDIC only converges in [-pi/2, pi/2]
    int negate_cos = 0;
    if (theta > M_PI / 2.0) {
        theta = M_PI - theta;
        negate_cos = 1;
    } else if (theta < -M_PI / 2.0) {
        theta = -M_PI - theta;
        negate_cos = 1;
    }

    double x = CORDIC_GAIN;
    double y = 0.0;
    double z = theta;

    for (int i = 0; i < 24; i++) {
        double x_new, y_new;
        if (z >= 0) {
            x_new = x - (y / (double)(1LL << i));
            y_new = y + (x / (double)(1LL << i));
            z -= cordic_atan[i];
        } else {
            x_new = x + (y / (double)(1LL << i));
            y_new = y - (x / (double)(1LL << i));
            z += cordic_atan[i];
        }
        x = x_new;
        y = y_new;
    }

    // 3. Apply Quadrant Sign Correction
    if (negate_cos) x = -x;

    *cos_out = x;
    *sin_out = y;
}


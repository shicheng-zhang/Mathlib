#include "ml_compiler.h"
#include "ml_fixed_point.h"

static const ml_q16_16_t fixed_cordic_atan[] = {
    51471, 30385, 16061, 8152, 4093, 2048, 1024, 512,
    256, 128, 64, 32, 16, 8, 4, 2, 1
};

ML_API ml_q16_16_t ml_fixed_mul(ml_q16_16_t a, ml_q16_16_t b) {
    int64_t res = ((int64_t)a * b) >> 16;
    if (res > 2147483647) return 2147483647;
    if (res < -2147483648) return -2147483648;
    return (ml_q16_16_t)res;
}

ML_API void ml_cordic_sincos_fixed(ml_q16_16_t theta, ml_q16_16_t *sin_out, ml_q16_16_t *cos_out) {
    theta = theta % ML_FIXED_TWO_PI;
    if (theta > ML_FIXED_PI) theta -= ML_FIXED_TWO_PI;
    if (theta < -ML_FIXED_PI) theta += ML_FIXED_TWO_PI;

    int negate_cos = 0;
    if (theta > ML_FIXED_HALF_PI) {
        theta = ML_FIXED_PI - theta;
        negate_cos = 1;
    } else if (theta < -ML_FIXED_HALF_PI) {
        theta = -ML_FIXED_PI - theta;
        negate_cos = 1;
    }

    ml_q16_16_t x = ML_FIXED_CORDIC_GAIN;
    ml_q16_16_t y = 0;
    ml_q16_16_t z = theta;

    for (int i = 0; i < 16; i++) {
        ml_q16_16_t x_new, y_new;
        if (z >= 0) {
            x_new = x - (y >> i);
            y_new = y + (x >> i);
            z -= fixed_cordic_atan[i];
        } else {
            x_new = x + (y >> i);
            y_new = y - (x >> i);
            z += fixed_cordic_atan[i];
        }
        x = x_new;
        y = y_new;
    }
    if (negate_cos) x = -x;
    *cos_out = x;
    *sin_out = y;
}

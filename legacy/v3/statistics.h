#ifndef LIBMATHC_STATISTICS_H
#define LIBMATHC_STATISTICS_H

//Library header file for statistics and probability
#include <math.h>
#include "combinatorics.h"
#include "exponential.h"
inline float mean (float *data, int n);
inline float mean (float *data, int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {sum += data[i];}
    return sum / n;
} inline float variance (float *data, int n);
inline float variance (float *data, int n) {
    float m = mean (data, n);
    float sum = 0;
    for (int i = 0; i < n; i++) {float diff = data[i] - m;
    sum += diff * diff;}
    return sum / n;
} inline float stddev (float *data, int n);
inline float stddev (float *data, int n) {return sqrt (variance (data, n));}
inline float binomial_pmf (int n, int k, float p);
inline float binomial_pmf (int n, int k, float p) {
    if (k < 0 || k > n || p < 0 || p > 1) {return 0.0 / 0.0;}
    return ncr (n, k) * pow (p, k) * pow (1 - p, n - k);
} inline float normal_pdf (float x, float mu, float sigma);
inline float normal_pdf (float x, float mu, float sigma) {
    if (sigma <= 0) {return 0.0 / 0.0;}
    float z = (x - mu) / sigma;
    return (1 / sqrt (2 * math_pi * sigma * sigma)) * exponential (-z * z / 2);
} inline void linear_regression (float *x, float *y, int n, float *out_m, float *out_b);
inline void linear_regression (float *x, float *y, int n, float *out_m, float *out_b) {
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for (int i = 0; i < n; i++) {sum_x += x[i];
    sum_y += y[i];
    sum_xy += x[i] * y[i];
    sum_x2 += x[i] * x[i];}
    float denom = n * sum_x2 - sum_x * sum_x;
    if (denom == 0) {*out_m = 0.0 / 0.0; *out_b = 0.0 / 0.0; return;}
    *out_m = (n * sum_xy - sum_x * sum_y) / denom;
    *out_b = (sum_y * sum_x2 - sum_x * sum_xy) / denom;
}



#endif

#include "ml_compiler.h"
#include "ml_statistics.h"

ML_API double ml_mean(const double *data, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

ML_API double ml_variance(const double *data, int n) {
    double m = ml_mean(data, n);
    double sum = 0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - m;
        sum += diff * diff;
    }
    return sum / n;
}

ML_API double ml_stddev(const double *data, int n) {
    return ml_sqrt(ml_variance(data, n));
}

ML_API double ml_binomial_pmf(int n, int k, double p) {
    if (k < 0 || k > n || p < 0 || p > 1) return 0.0 / 0.0;
    return (double)ml_ncr(n, k) * ml_pow(p, k) * ml_pow(1 - p, n - k);
}

ML_API double ml_normal_pdf(double x, double mu, double sigma) {
    if (sigma <= 0) return 0.0 / 0.0;
    double z = (x - mu) / sigma;
    return (1.0 / ml_sqrt(2.0 * ML_PI * sigma * sigma)) * ml_exp(-z * z / 2.0);
}

ML_API void ml_linear_regression(const double *x, const double *y, int n, double *out_m, double *out_b) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }
    double denom = n * sum_x2 - sum_x * sum_x;
    if (denom == 0) { *out_m = 0.0 / 0.0; *out_b = 0.0 / 0.0; return; }
    *out_m = (n * sum_xy - sum_x * sum_y) / denom;
    *out_b = (sum_y * sum_x2 - sum_x * sum_xy) / denom;
}

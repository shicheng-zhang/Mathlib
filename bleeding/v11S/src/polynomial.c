#include "ml_compiler.h"
#include "ml_polynomial.h"

ML_API double ml_polynomial_eval(const double *coeffs, int degree, double x) {
    double result = coeffs[degree];
    for (int i = degree - 1; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

ML_API void ml_polynomial_derivative(const double *coeffs, int degree, double *out) {
    for (int i = 0; i < degree; i++) {
        out[i] = coeffs[i + 1] * (i + 1);
    }
}

ML_API double ml_polynomial_newton(const double *coeffs, int degree, double x0, double epsilon, int max_iter) {
    double x = x0;
    for (int iter = 0; iter < max_iter; iter++) {
        double fx = coeffs[degree];
        double dfx = 0;
        for (int i = degree - 1; i >= 0; i--) {
            dfx = dfx * x + fx;
            fx = fx * x + coeffs[i];
        }
        if (ml_fabs(dfx) < epsilon) return 0.0 / 0.0;
        double x_next = x - fx / dfx;
        if (ml_fabs(x_next - x) < epsilon) return x_next;
        x = x_next;
    }
    return x;
}

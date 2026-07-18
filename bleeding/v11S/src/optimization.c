#include "ml_compiler.h"
#include "ml_optimization.h"

ML_API double ml_optimize_golden(ml_opt_func_t f, double a, double b, double tol, int max_iter) {
    double phi = (1.0 + ml_sqrt(5.0)) / 2.0;
    double resphi = 2.0 - phi;
    double x1 = a + resphi * (b - a);
    double x2 = b - resphi * (b - a);
    double f1 = f(x1);
    double f2 = f(x2);
    for (int i = 0; i < max_iter; i++) {
        if (b - a < tol) break;
        if (f1 < f2) {
            b = x2; x2 = x1; f2 = f1;
            x1 = a + resphi * (b - a); f1 = f(x1);
        } else {
            a = x1; x1 = x2; f1 = f2;
            x2 = b - resphi * (b - a); f2 = f(x2);
        }
    }
    return (a + b) / 2.0;
}

ML_API double ml_optimize_gradient_descent(ml_opt_func_t f, double start, double lr, double tol, int max_iter) {
    double x = start;
    for (int i = 0; i < max_iter; i++) {
        double grad = ml_derivative(f, x, 1e-5);
        double x_new = x - lr * grad;
        if (ml_fabs(x_new - x) < tol) break;
        x = x_new;
    }
    return x;
}

#include "ml_compiler.h"
#include "ml_quadratics.h"

ML_API double ml_equation(double a, double b, double c, double x) {
    return (a * x * x) + b * x + c;
}

ML_API double ml_formula_pos(double a, double b, double c) {
    double disc = b * b - 4 * a * c;
    if (disc < 0.0) return 0.0 / 0.0;
    if (b >= 0.0) {
        double q = -0.5 * (b + ml_sqrt(disc));
        return c / q;
    } else {
        double q = -0.5 * (b - ml_sqrt(disc));
        return q / a;
    }
}

ML_API double ml_formula_neg(double a, double b, double c) {
    double disc = b * b - 4 * a * c;
    if (disc < 0.0) return 0.0 / 0.0;
    if (b >= 0.0) {
        double q = -0.5 * (b + ml_sqrt(disc));
        return q / a;
    } else {
        double q = -0.5 * (b - ml_sqrt(disc));
        return c / q;
    }
}

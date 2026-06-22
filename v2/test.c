#include "test.h"
#include "combinatorics.h"
#include "quadratics.h"
#include "integral.h"
#include "trigonometry.h"
#include "exponential.h"
#include "numerical.h"
#include "polynomial.h"
#include "complex.h"
#include "linear_algebra.h"

int tests_passed = 0;
int tests_failed = 0;

float test_f_quad (float x) {return x * x - 4;}
float test_df_quad (float x) {return 2 * x;}
float test_f_cubic (float x) {return x * x * x - 27;}
float test_df_cubic (float x) {return 3 * x * x;}
float test_f_parabola (float x) {return x * x;}
float test_f_line (float x) {return 2 * x - 6;}

int main() {
    printf("=== Combinatorics ===\n");
    int fact_vals[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};
    for (int i = 0; i <= 10; i++) {CHECK_INT(factorial(i), fact_vals[i]);}
    for (int n = 0; n <= 10; n++) {
        for (int r = 0; r <= n; r++) {CHECK_INT(npr(n, r), factorial(n) / factorial(n - r));}
    }
    for (int n = 0; n <= 10; n++) {
        for (int r = 0; r <= n; r++) {CHECK_INT(ncr(n, r), factorial(n) / (factorial(n - r) * factorial(r)));}
    }
    CHECK_INT(npr(5, -1), 0);
    CHECK_INT(npr(5, 6), 0);
    CHECK_INT(ncr(5, -1), 0);
    CHECK_INT(ncr(5, 6), 0);

    printf("\n=== Quadratics ===\n");
    float quad_a[] = {1, 1, 2, 1};
    float quad_b[] = {2, 3, -4, -5};
    float quad_c[] = {1, 2, -6, 6};
    float quad_x[] = {3, 0, 1, 2};
    float quad_exp[] = {16, 2, -8, 0};
    for (int i = 0; i < 4; i++) {CHECK_NEAR(equation(quad_a[i], quad_b[i], quad_c[i], quad_x[i]), quad_exp[i]);}
    CHECK_NEAR(formula_pos(1, 3, 2), -1);
    CHECK_NEAR(formula_neg(1, 3, 2), -2);
    CHECK_NEAR(formula_pos(1, -5, 6), 3);
    CHECK_NEAR(formula_neg(1, -5, 6), 2);

    printf("\n=== Integral ===\n");
    CHECK_NEAR_LOOSE(factorial_float(5), 120, 5);
    CHECK_NEAR_LOOSE(factorial_float(10), 3628800, 50000);
    CHECK_NEAR(integral_traditional(0, 1, 2, 0, 0.001), 0.333333);
    CHECK_NEAR(integral_traditional(0, 2, 1, 0, 0.001), 2);
    CHECK_NEAR(gamma_new(1), 1);
    CHECK_NEAR(gamma_new(2), 1);
    CHECK_NEAR(gamma_new(3), 2);
    CHECK_NEAR_LOOSE(gamma_new(4), 6, 0.01);
    CHECK_NEAR(gamma_new(0.5), 1.772454);
    CHECK_NEAR(gamma_new(1.5), 0.886227);

    printf("\n=== Trigonometry ===\n");
    float trig_vals[] = {0, math_pi / 6, math_pi / 4, math_pi / 3, math_pi / 2, math_pi};
    float sin_exp[] = {0, 0.5, 0.707107, 0.866025, 1, 0};
    float cos_exp[] = {1, 0.866025, 0.707107, 0.5, 0, -1};
    for (int i = 0; i < 6; i++) {
        CHECK_NEAR(sine(trig_vals[i]), sin_exp[i]);
        CHECK_NEAR(cosine(trig_vals[i]), cos_exp[i]);
    }
    CHECK_NEAR(tangent(0), 0);
    CHECK_NEAR(tangent(math_pi / 4), 1);
    CHECK_NEAR(arcsine(0), 0);
    CHECK_NEAR(arcsine(1), math_pi / 2);
    CHECK_NEAR(arccosine(0), math_pi / 2);
    CHECK_NEAR(arccosine(1), 0);
    CHECK_NEAR(arctangent(0), 0);
    CHECK_NEAR(arctangent(1), math_pi / 4);
    CHECK_NEAR(arctangent(100), 1.560797);
    CHECK_NEAR(arccotangent(1), math_pi / 4);
    CHECK_NEAR(arccotangent(-1), 3 * math_pi / 4);
    CHECK_NEAR(arccotangent(0), math_pi / 2);

    printf("\n=== Exponential ===\n");
    CHECK_NEAR(exponential(0), 1);
    CHECK_NEAR(exponential(1), 2.718282);
    CHECK_NEAR(exponential(2), 7.389056);
    CHECK_NEAR(logarithm(1), 0);
    CHECK_NEAR(logarithm(math_e), 1);
    CHECK_NEAR(logarithm(math_e * math_e), 2);
    CHECK_NEAR(power(2, 3), 8);
    CHECK_NEAR(power(9, 0.5), 3);
    CHECK_NEAR(power(2, 10), 1024);
    CHECK_NEAR(logarithm_base(8, 2), 3);
    CHECK_NEAR(logarithm_base(27, 3), 3);
    CHECK_NEAR(hyperbolic_sine(0), 0);
    CHECK_NEAR(hyperbolic_cosine(0), 1);
    CHECK_NEAR(hyperbolic_tangent(0), 0);
    CHECK_NEAR(inverse_hyperbolic_sine(0), 0);
    CHECK_NEAR(inverse_hyperbolic_cosine(1), 0);
    CHECK_NEAR(inverse_hyperbolic_tangent(0), 0);

    printf("\n=== Numerical ===\n");
    CHECK_NEAR(newton_raphson(test_f_quad, test_df_quad, 3, 0.0001, 100), 2);
    CHECK_NEAR(newton_raphson(test_f_quad, test_df_quad, -3, 0.0001, 100), -2);
    CHECK_NEAR(newton_raphson(test_f_cubic, test_df_cubic, 5, 0.0001, 100), 3);
    CHECK_NEAR(bisection(test_f_quad, 1, 3, 0.0001, 100), 2);
    CHECK_NEAR(bisection(test_f_quad, -3, 0, 0.0001, 100), -2);
    CHECK_NEAR(bisection(test_f_line, 0, 5, 0.0001, 100), 3);
    for (float x = -5; x <= 5; x += 1) {CHECK_NEAR(derivative(test_f_parabola, x, 0.001), 2 * x);}
    CHECK_NEAR(second_derivative(test_f_parabola, 3, 0.1), 2);
    CHECK_NEAR(second_derivative(test_f_parabola, -2, 0.1), 2);
    CHECK_NEAR(integral_simpson(test_f_parabola, 0, 1, 100), 0.333333);
    CHECK_NEAR(integral_simpson(test_f_parabola, 0, 2, 100), 2.666667);

    printf("\n=== Polynomial ===\n");
    float coeffs_quad[] = {-4, 0, 1};
    float coeffs_cubic[] = {-27, 0, 0, 1};
    float deriv_quad[2];
    float deriv_cubic[3];
    CHECK_NEAR(polynomial_eval(coeffs_quad, 2, 2), 0);
    CHECK_NEAR(polynomial_eval(coeffs_quad, 2, 3), 5);
    CHECK_NEAR(polynomial_eval(coeffs_quad, 2, -2), 0);
    CHECK_NEAR(polynomial_eval(coeffs_cubic, 3, 3), 0);
    CHECK_NEAR(polynomial_eval(coeffs_cubic, 3, 0), -27);
    polynomial_derivative(coeffs_quad, 2, deriv_quad);
    CHECK_NEAR(deriv_quad[0], 0);
    CHECK_NEAR(deriv_quad[1], 2);
    polynomial_derivative(coeffs_cubic, 3, deriv_cubic);
    CHECK_NEAR(deriv_cubic[0], 0);
    CHECK_NEAR(deriv_cubic[1], 0);
    CHECK_NEAR(deriv_cubic[2], 3);
    CHECK_NEAR(polynomial_newton(coeffs_quad, 2, 3, 0.0001, 100), 2);
    CHECK_NEAR(polynomial_newton(coeffs_quad, 2, -1, 0.0001, 100), -2);
    CHECK_NEAR(polynomial_newton(coeffs_cubic, 3, 5, 0.0001, 100), 3);

    printf("\n=== Complex ===\n");
    cplx a = {1, 2};
    cplx b = {3, 4};
    cplx r = cplx_add(a, b);
    CHECK_NEAR(r.real, 4);
    CHECK_NEAR(r.imag, 6);
    r = cplx_sub(a, b);
    CHECK_NEAR(r.real, -2);
    CHECK_NEAR(r.imag, -2);
    r = cplx_mul(a, b);
    CHECK_NEAR(r.real, -5);
    CHECK_NEAR(r.imag, 10);
    r = cplx_div((cplx) {5, 10}, (cplx) {1, 2});
    CHECK_NEAR(r.real, 5);
    CHECK_NEAR(r.imag, 0);
    CHECK_NEAR(cplx_abs((cplx) {3, 4}), 5);
    CHECK_NEAR(cplx_abs((cplx) {5, 12}), 13);
    CHECK_NEAR(cplx_arg((cplx) {1, 1}), math_pi / 4);
    CHECK_NEAR(cplx_arg((cplx) {-1, 0}), math_pi);
    CHECK_NEAR(cplx_arg((cplx) {0, 1}), math_pi / 2);
    r = cplx_conjugate((cplx) {1, 2});
    CHECK_NEAR(r.real, 1);
    CHECK_NEAR(r.imag, -2);
    r = cplx_exponential((cplx) {0, math_pi});
    CHECK_NEAR(r.real, -1);
    CHECK_NEAR(r.imag, 0);
    r = cplx_exponential((cplx) {0, 0});
    CHECK_NEAR(r.real, 1);
    CHECK_NEAR(r.imag, 0);
    r = cplx_logarithm((cplx) {math_e, 0});
    CHECK_NEAR(r.real, 1);
    CHECK_NEAR(r.imag, 0);
    r = cplx_power((cplx) {math_e, 0}, (cplx) {2, 0});
    CHECK_NEAR(r.real, math_e * math_e);
    CHECK_NEAR(r.imag, 0);

    printf("\n=== Linear Algebra ===\n");
    CHECK_NEAR(vec2_dot(1, 2, 3, 4), 11);
    CHECK_NEAR(vec2_dot(0, 1, 1, 0), 0);
    CHECK_NEAR(vec2_cross(1, 0, 0, 1), 1);
    CHECK_NEAR(vec2_cross(1, 2, 3, 4), -2);
    CHECK_NEAR(vec2_mag(3, 4), 5);
    CHECK_NEAR(vec2_mag(5, 12), 13);
    CHECK_NEAR(vec3_dot(1, 2, 3, 4, 5, 6), 32);
    CHECK_NEAR(vec3_dot(1, 0, 0, 0, 1, 0), 0);
    float cx, cy, cz;
    vec3_cross(1, 0, 0, 0, 1, 0, &cx, &cy, &cz);
    CHECK_NEAR(cx, 0);
    CHECK_NEAR(cy, 0);
    CHECK_NEAR(cz, 1);
    vec3_cross(1, 2, 3, 4, 5, 6, &cx, &cy, &cz);
    CHECK_NEAR(cx, -3);
    CHECK_NEAR(cy, 6);
    CHECK_NEAR(cz, -3);
    CHECK_NEAR(vec3_mag(1, 2, 2), 3);
    CHECK_NEAR(vec3_mag(3, 4, 12), 13);
    float id[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    float m1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    float m2[9] = {1, 0, 0, 0, 2, 0, 0, 0, 3};
    float m_out[9];
    mat3x3_mul(id, m1, m_out);
    for (int i = 0; i < 9; i++) {CHECK_NEAR(m_out[i], m1[i]);}
    mat3x3_mul(m2, m2, m_out);
    CHECK_NEAR(m_out[0], 1);
    CHECK_NEAR(m_out[4], 4);
    CHECK_NEAR(m_out[8], 9);
    CHECK_NEAR(mat3x3_det(id), 1);
    float diag[9] = {2, 0, 0, 0, 3, 0, 0, 0, 4};
    CHECK_NEAR(mat3x3_det(diag), 24);
    mat3x3_transpose(m1, m_out);
    CHECK_NEAR(m_out[1], 4);
    CHECK_NEAR(m_out[3], 2);
    mat3x3_inverse(diag, m_out);
    CHECK_NEAR(m_out[0], 0.5);
    CHECK_NEAR(m_out[4], 1.0 / 3.0);
    CHECK_NEAR(m_out[8], 0.25);
    float rot[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1};
    float v[3] = {1, 0, 0};
    float v_out[3];
    linear_solve_3x3(rot, v, v_out);
    CHECK_NEAR(v_out[0], 0);
    CHECK_NEAR(v_out[1], -1);
    CHECK_NEAR(v_out[2], 0);

    TEST_SUMMARY();
}

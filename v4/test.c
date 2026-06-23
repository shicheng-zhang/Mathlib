#include <time.h>
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
#include "statistics.h"
#include "ode.h"
#include "optimization.h"
#include "fft.h"

int tests_passed = 0;
int tests_failed = 0;

double test_f_quad (double x) {return x * x - 4;}
double test_df_quad (double x) {return 2 * x;}
double test_f_cubic (double x) {return x * x * x - 27;}
double test_df_cubic (double x) {return 3 * x * x;}
double test_f_parabola (double x) {return x * x;}
double test_f_line (double x) {return 2 * x - 6;}

double test_ode_exp(double t, double y) { (void)t; return y; }
double test_opt_parabola(double x) { return (x - 3.0) * (x - 3.0) + 1.0; }

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
    double quad_a[] = { -4.37807108021383, 7.44171375606222, -2.38092766494391, -6.09277531124698 };
    double quad_b[] = { -8.76874034333991, -8.69858053047926, 6.39386092106106, 9.50961923566157 };
    double quad_c[] = { 5.20694018874546, 1.78381828982582, 9.69692207303278, -0.0346913519322847 };
    double quad_x[] = { -2.05063467614749, 5.20697783904846, -0.954406386486236, 5.57711875528241 };
    double quad_exp[] = { 4.77818522991039, 158.254846179856, 1.42581348316178, -136.509644030088 };
    for (int i = 0; i < 4; i++) {CHECK_NEAR(equation(quad_a[i], quad_b[i], quad_c[i], quad_x[i]), quad_exp[i]);}
    CHECK_NEAR(formula_pos(1, 3, 2), -1);
    CHECK_NEAR(formula_neg(1, 3, 2), -2);
    CHECK_NEAR(formula_pos(1, -5, 6), 3);
    CHECK_NEAR(formula_neg(1, -5, 6), 2);

    printf("\n=== Integral ===\n");
    CHECK_NEAR_LOOSE(factorial_float(5), 120, 5);
    CHECK_NEAR_LOOSE(factorial_float(10), 3628800, 50000);
    CHECK_NEAR_LOOSE(integral_traditional(0, 1, 2, 0, 0.001), 1.0 / 3.0, 1e-3);
    CHECK_NEAR_LOOSE(integral_traditional(0, 2, 1, 0, 0.001), 2.0, 5e-3);
    double sqrt_pi = sqrt(math_pi);
    CHECK_NEAR_LOOSE(gamma_new(1), 1.0, 1e-3);
    CHECK_NEAR_LOOSE(gamma_new(2), 1.0, 1e-3);
    CHECK_NEAR_LOOSE(gamma_new(3), 2.0, 5e-3);
    CHECK_NEAR_LOOSE(gamma_new(4), 6, 0.01);
    CHECK_NEAR_LOOSE(gamma_new(0.5), sqrt_pi, 1e-3);
    CHECK_NEAR_LOOSE(gamma_new(1.5), 0.5 * sqrt_pi, 1e-3);

    printf("\n=== Trigonometry ===\n");
    double trig_vals[] = { -847217.128272354, 718467.474610919, 5.76807247781451, 1e-12, 314159.651774876, -157082.771130551 };
    double sin_exp[] = { 0.962307284875001, -0.809146525566379, -0.492633040476092, 1e-12, 0.376871010400933, -0.0031415874894714 };
    double cos_exp[] = { 0.271964500397576, -0.587606926579203, 0.870237144364271, 1, 0.926265751023636, -0.999995065201847 };
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
    CHECK_NEAR(arctangent(100), 1.5607966601082315);
    CHECK_NEAR(arccotangent(1), math_pi / 4);
    CHECK_NEAR(arccotangent(-1), 3 * math_pi / 4);
    CHECK_NEAR(arccotangent(0), math_pi / 2);

    printf("\n=== Exponential ===\n");
    CHECK_NEAR_LOOSE(exponential(-4.09154896605328), 0.0167133251248628, 1e-7);
    CHECK_NEAR_LOOSE(exponential(-4.09154896605328), 0.0167133251248628, 1e-7);
    CHECK_NEAR_LOOSE(exponential(-4.09154896605328), 0.0167133251248628, 1e-7);
    CHECK_NEAR_LOOSE(logarithm(87.5092024270944), 4.47174395842889, 1e-7);
    CHECK_NEAR_LOOSE(logarithm(87.5092024270944), 4.47174395842889, 1e-7);
    CHECK_NEAR_LOOSE(logarithm(87.5092024270944), 4.47174395842889, 1e-7);
    CHECK_NEAR_LOOSE(power(3.84607869743585, 1.6646858157089), 9.41610827019948, 1e-7);
    CHECK_NEAR_LOOSE(power(3.84607869743585, 1.6646858157089), 9.41610827019948, 1e-7);
    CHECK_NEAR_LOOSE(power(3.84607869743585, 1.6646858157089), 9.41610827019948, 1e-7);
    CHECK_NEAR(logarithm_base(8, 2), 3);
    CHECK_NEAR(logarithm_base(27, 3), 3);
    CHECK_NEAR(hyperbolic_sine(0), 0);
    CHECK_NEAR(hyperbolic_cosine(0), 1);
    CHECK_NEAR(hyperbolic_tangent(0), 0);
    CHECK_NEAR(inverse_hyperbolic_sine(0), 0);
    CHECK_NEAR(inverse_hyperbolic_cosine(1), 0);
    CHECK_NEAR(inverse_hyperbolic_tangent(0), 0);

    printf("\n=== Numerical ===\n");
    CHECK_NEAR(newton_raphson(test_f_quad, test_df_quad, 3, 1e-12, 100), 2);
    CHECK_NEAR(newton_raphson(test_f_quad, test_df_quad, -3, 1e-12, 100), -2);
    CHECK_NEAR(newton_raphson(test_f_cubic, test_df_cubic, 5, 1e-12, 100), 3);
    CHECK_NEAR(bisection(test_f_quad, 1, 3, 1e-12, 100), 2);
    CHECK_NEAR(bisection(test_f_quad, -3, 0, 1e-12, 100), -2);
    CHECK_NEAR(bisection(test_f_line, 0, 5, 1e-12, 100), 3);
    for (double x = -5; x <= 5; x += 1) {CHECK_NEAR(derivative(test_f_parabola, x, 0.001), 2 * x);}
    CHECK_NEAR(second_derivative(test_f_parabola, 3, 0.1), 2);
    CHECK_NEAR(second_derivative(test_f_parabola, -2, 0.1), 2);
    CHECK_NEAR(integral_simpson(test_f_parabola, 0, 1, 100), 1.0 / 3.0);
    CHECK_NEAR(integral_simpson(test_f_parabola, 0, 2, 100), 8.0 / 3.0);

    printf("\n=== Polynomial ===\n");
    double coeffs_quad[] = {-4, 0, 1};
    double coeffs_cubic[] = {-27, 0, 0, 1};
    double deriv_quad[2];
    double deriv_cubic[3];
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
    CHECK_NEAR(polynomial_newton(coeffs_quad, 2, 3, 1e-12, 100), 2);
    CHECK_NEAR(polynomial_newton(coeffs_quad, 2, -1, 1e-12, 100), -2);
    CHECK_NEAR(polynomial_newton(coeffs_cubic, 3, 5, 1e-12, 100), 3);

    printf("\n=== Complex ===\n");
    cplx a = {1, 2};
    cplx b_cplx = {3, 4};
    cplx r = cplx_add(a, b_cplx);
    CHECK_NEAR(r.real, 4);
    CHECK_NEAR(r.imag, 6);
    r = cplx_sub(a, b_cplx);
    CHECK_NEAR(r.real, -2);
    CHECK_NEAR(r.imag, -2);
    r = cplx_mul(a, b_cplx);
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

    // --- Vec2 Exhaustive Tests ---
    vec2 v2a = {1, 2}, v2b = {3, 4};
    CHECK_NEAR(vec2_dot(v2a, v2b), 11);
    CHECK_NEAR(vec2_dot((vec2){0, 1}, (vec2){1, 0}), 0);
    CHECK_NEAR(vec2_cross(v2a, v2b), -2);
    CHECK_NEAR(vec2_cross((vec2){1, 0}, (vec2){0, 1}), 1);
    CHECK_NEAR(vec2_mag((vec2){3, 4}), 5);
    CHECK_NEAR(vec2_mag((vec2){5, 12}), 13);

    vec2 v2_add = vec2_add(v2a, v2b);
    CHECK_NEAR(v2_add.x, 4); CHECK_NEAR(v2_add.y, 6);
    vec2 v2_sub = vec2_sub(v2a, v2b);
    CHECK_NEAR(v2_sub.x, -2); CHECK_NEAR(v2_sub.y, -2);
    vec2 v2_scale = vec2_scale(v2a, 2.0);
    CHECK_NEAR(v2_scale.x, 2); CHECK_NEAR(v2_scale.y, 4);
    vec2 v2_norm = vec2_normalize((vec2){0, 5});
    CHECK_NEAR(v2_norm.x, 0); CHECK_NEAR(v2_norm.y, 1);

    // --- Vec3 Exhaustive Tests ---
    vec3 v3a = {1, 2, 3}, v3b = {4, 5, 6};
    CHECK_NEAR(vec3_dot(v3a, v3b), 32);
    CHECK_NEAR(vec3_dot((vec3){1, 0, 0}, (vec3){0, 1, 0}), 0);

    vec3 cx = vec3_cross((vec3){1, 0, 0}, (vec3){0, 1, 0});
    CHECK_NEAR(cx.x, 0); CHECK_NEAR(cx.y, 0); CHECK_NEAR(cx.z, 1);

    vec3 cy = vec3_cross(v3a, v3b);
    CHECK_NEAR(cy.x, -3); CHECK_NEAR(cy.y, 6); CHECK_NEAR(cy.z, -3);

    CHECK_NEAR(vec3_mag((vec3){1, 2, 2}), 3);
    CHECK_NEAR(vec3_mag((vec3){3, 4, 12}), 13);

    vec3 v3_add = vec3_add(v3a, v3b);
    CHECK_NEAR(v3_add.x, 5); CHECK_NEAR(v3_add.y, 7); CHECK_NEAR(v3_add.z, 9);
    vec3 v3_sub = vec3_sub(v3a, v3b);
    CHECK_NEAR(v3_sub.x, -3); CHECK_NEAR(v3_sub.y, -3); CHECK_NEAR(v3_sub.z, -3);
    vec3 v3_scale = vec3_scale(v3a, 2.0);
    CHECK_NEAR(v3_scale.x, 2); CHECK_NEAR(v3_scale.y, 4); CHECK_NEAR(v3_scale.z, 6);
    vec3 v3_norm = vec3_normalize((vec3){0, 0, 5});
    CHECK_NEAR(v3_norm.x, 0); CHECK_NEAR(v3_norm.y, 0); CHECK_NEAR(v3_norm.z, 1);

    // --- Mat3x3 Exhaustive Tests ---
    mat3x3 id = mat3x3_identity();
    mat3x3 m1 = {{1,2,3, 4,5,6, 7,8,9}};
    mat3x3 m2 = {{1,0,0, 0,2,0, 0,0,3}};

    mat3x3 m_out = mat3x3_mul(id, m1);
    for (int i = 0; i < 9; i++) { CHECK_NEAR(m_out.m[i], m1.m[i]); }

    m_out = mat3x3_mul(m2, m2);
    CHECK_NEAR(m_out.m[0], 1);
    CHECK_NEAR(m_out.m[4], 4);
    CHECK_NEAR(m_out.m[8], 9);

    CHECK_NEAR(mat3x3_det(id), 1);
    mat3x3 diag = {{2,0,0, 0,3,0, 0,0,4}};
    CHECK_NEAR(mat3x3_det(diag), 24);

    // Test singular matrix returns NaN
    mat3x3 singular = {{1,2,3, 4,5,6, 7,8,9}};
    mat3x3 inv_sing = mat3x3_inverse(singular);
    CHECK_NAN(inv_sing.m[0]);

    m_out = mat3x3_transpose(m1);
    CHECK_NEAR(m_out.m[1], 4);
    CHECK_NEAR(m_out.m[3], 2);

    m_out = mat3x3_inverse(diag);
    CHECK_NEAR(m_out.m[0], 0.5);
    CHECK_NEAR(m_out.m[4], 1.0 / 3.0);
    CHECK_NEAR(m_out.m[8], 0.25);

    mat3x3 rot = {{0,-1,0, 1,0,0, 0,0,1}};
    vec3 v = {1, 0, 0};
    vec3 v_out = mat3x3_mul_vec3(rot, v);
    CHECK_NEAR(v_out.x, 0);
    CHECK_NEAR(v_out.y, 1);
    CHECK_NEAR(v_out.z, 0);

    vec3 solve_out = linear_solve_3x3(rot, v);
    CHECK_NEAR(solve_out.x, 0);
    CHECK_NEAR(solve_out.y, -1);
    CHECK_NEAR(solve_out.z, 0);
    printf("\n=== Statistics ===\n");
    double data1[] = { -42.5538248942568, 1.6671119325518, -46.3865246208777, 95.6775017318609, 41.6195905503264 };
    CHECK_NEAR(mean(data1, 5), 10.0047709399209);
    CHECK_NEAR(variance(data1, 5), 2870.24287783113);
    CHECK_NEAR(stddev(data1, 5), 53.5746477154179);
    double data2[] = {2, 4, 4, 4, 5, 5, 7, 9};
    CHECK_NEAR(mean(data2, 8), 5);
    CHECK_NEAR(variance(data2, 8), 4);
    CHECK_NEAR(stddev(data2, 8), 2);
    double data3[] = {10, 10, 10};
    CHECK_NEAR(mean(data3, 3), 10);
    CHECK_NEAR(variance(data3, 3), 0);
    CHECK_NEAR(stddev(data3, 3), 0);
    for (int n = 1; n <= 10; n++) {
        double sum = 0;
        for (int k = 0; k <= n; k++) {sum += binomial_pmf(n, k, 0.5);}
        CHECK_NEAR(sum, 1);
    }
    CHECK_NEAR(binomial_pmf(5, 2, 0.5), 0.3125);
    CHECK_NEAR(binomial_pmf(5, 0, 0.5), 0.03125);
    CHECK_NEAR(binomial_pmf(5, 5, 0.5), 0.03125);
    CHECK_NEAR(binomial_pmf(10, 5, 0.5), 0.246093750000000);
    CHECK_NAN(binomial_pmf(5, -1, 0.5));
    CHECK_NAN(binomial_pmf(5, 6, 0.5));
    double z0 = normal_pdf(0, 0, 1);
    CHECK_NEAR(z0, 0.398942280401433);
    CHECK_NEAR(normal_pdf(1, 0, 1), 0.241970724519143);
    CHECK_NEAR(normal_pdf(0, 0, 2), 0.199471140200716);
    CHECK_NEAR(normal_pdf(2, 2, 1), 0.398942280401433);
    double lr_x[] = {1, 2, 3, 4, 5};
    double lr_y[] = {2, 4, 6, 8, 10};
    double m_slope, b_intercept;
    linear_regression(lr_x, lr_y, 5, &m_slope, &b_intercept);
    CHECK_NEAR(m_slope, 2);
    CHECK_NEAR(b_intercept, 0);
    double lr_x2[] = {0, 1, 2, 3};
    double lr_y2[] = {1, 3, 5, 7};
    linear_regression(lr_x2, lr_y2, 4, &m_slope, &b_intercept);
    CHECK_NEAR(m_slope, 2);
    CHECK_NEAR(b_intercept, 1);
    double lr_x3[] = {1, 2, 3};
    double lr_y3[] = {5, 5, 5};
    linear_regression(lr_x3, lr_y3, 3, &m_slope, &b_intercept);
    CHECK_NEAR(m_slope, 0);
    CHECK_NEAR(b_intercept, 5);

    printf("\n=== Phase 3 Benchmark ===\n");
    clock_t start = clock();
    for(int i=0; i<100000; i++) { sine(10000.0 + i); }
    clock_t end = clock();
    printf("Time for 100k sines (large angles): %f ms\n", (double)(end-start)/CLOCKS_PER_SEC * 1000);

    printf("\n=== ODE ===\n");
    CHECK_NEAR_LOOSE(ode_euler(test_ode_exp, 0.0, 1.0, 0.001, 1000), exponential(1.0), 0.01);
    CHECK_NEAR_LOOSE(ode_rk4(test_ode_exp, 0.0, 1.0, 0.001, 1000), exponential(1.0), 1e-7);

    printf("\n=== Optimization ===\n");
    CHECK_NEAR_LOOSE(optimize_golden(test_opt_parabola, -10.0, 10.0, 1e-5, 100), 3.0, 1e-4);
    CHECK_NEAR_LOOSE(optimize_gradient_descent(test_opt_parabola, 0.0, 0.1, 1e-5, 1000), 3.0, 1e-3);

    printf("\n=== FFT ===\n");
    cplx signal[8];
    for(int i=0; i<8; i++) {
        signal[i].real = sine(2.0 * math_pi * i / 8.0);
        signal[i].imag = 0.0;
    }
    fft_execute(signal, 8);
    CHECK_NEAR_LOOSE(cplx_abs(signal[1]), 4.0, 1e-7);
    CHECK_NEAR_LOOSE(cplx_abs(signal[7]), 4.0, 1e-7);
    CHECK_NEAR_LOOSE(cplx_abs(signal[0]), 0.0, 1e-7);
    CHECK_NEAR_LOOSE(cplx_abs(signal[2]), 0.0, 1e-7);
    ifft_execute(signal, 8);
    for(int i=0; i<8; i++) {
        CHECK_NEAR_LOOSE(signal[i].real, sine(2.0 * math_pi * i / 8.0), 1e-7);
        CHECK_NEAR_LOOSE(signal[i].imag, 0.0, 1e-7);
    }

    printf("\n=== IEEE 754 Edge Cases ===\n");
    cplx zero_c = {0.0, 0.0};
    cplx div_zero = cplx_div((cplx){1.0, 1.0}, zero_c);
    CHECK_NAN(div_zero.real);
    CHECK_NAN(div_zero.imag);
    CHECK_INF(exponential(1.0/0.0));
    CHECK_NEAR(exponential(-1.0/0.0), 0.0);
    CHECK_NEG_INF(logarithm(0.0));
    CHECK_NAN(logarithm(-1.0));
    CHECK_NAN(arcsine(2.0));
    CHECK_NAN(arccosine(2.0));

    TEST_SUMMARY();
}

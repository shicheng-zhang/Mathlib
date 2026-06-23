#ifndef LIBMATHC_SIMD_H
#define LIBMATHC_SIMD_H

// GCC vector extension for 4-wide double precision (AVX)
typedef double ml_vec4 __attribute__((vector_size(32)));

inline ml_vec4 ml_vec4_add(ml_vec4 a, ml_vec4 b) { return a + b; }
inline ml_vec4 ml_vec4_sub(ml_vec4 a, ml_vec4 b) { return a - b; }
inline ml_vec4 ml_vec4_mul(ml_vec4 a, ml_vec4 b) { return a * b; }
inline ml_vec4 ml_vec4_scale(ml_vec4 a, double s) { return a * (ml_vec4){s, s, s, s}; }

inline double ml_vec4_dot(ml_vec4 a, ml_vec4 b) {
    ml_vec4 prod = a * b;
    return prod[0] + prod[1] + prod[2] + prod[3];
}

inline double ml_vec4_mag(ml_vec4 a) {
    double dot = ml_vec4_dot(a, a);
    double res;
    __asm__ ("sqrtsd %1, %0" : "=x" (res) : "x" (dot));
    return res;
}

#endif

#ifndef LIBMATHC_LINEAR_ALGEBRA_H
#define LIBMATHC_LINEAR_ALGEBRA_H

//Library header file for linear algebra
#include <math.h>
inline float vec2_dot (float ax, float ay, float bx, float by);
inline float vec2_dot (float ax, float ay, float bx, float by) {return ax * bx + ay * by;}
inline float vec2_cross (float ax, float ay, float bx, float by);
inline float vec2_cross (float ax, float ay, float bx, float by) {return ax * by - ay * bx;}
inline float vec2_mag (float ax, float ay);
inline float vec2_mag (float ax, float ay) {return sqrt (ax * ax + ay * ay);}
inline float vec3_dot (float ax, float ay, float az, float bx, float by, float bz);
inline float vec3_dot (float ax, float ay, float az, float bx, float by, float bz) {return ax * bx + ay * by + az * bz;}
inline void vec3_cross (float ax, float ay, float az, float bx, float by, float bz, float *ox, float *oy, float *oz);
inline void vec3_cross (float ax, float ay, float az, float bx, float by, float bz, float *ox, float *oy, float *oz) {
    *ox = ay * bz - az * by;
    *oy = az * bx - ax * bz;
    *oz = ax * by - ay * bx;
} inline float vec3_mag (float ax, float ay, float az);
inline float vec3_mag (float ax, float ay, float az) {return sqrt (ax * ax + ay * ay + az * az);}
inline void mat3x3_mul (float *a, float *b, float *out);
inline void mat3x3_mul (float *a, float *b, float *out) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            out[i * 3 + j] = 0;
            for (int k = 0; k < 3; k++) {out[i * 3 + j] += a[i * 3 + k] * b[k * 3 + j];}
        }
    }
} inline float mat3x3_det (float *m);
inline float mat3x3_det (float *m) {
    return m[0] * (m[4] * m[8] - m[5] * m[7])
         - m[1] * (m[3] * m[8] - m[5] * m[6])
         + m[2] * (m[3] * m[7] - m[4] * m[6]);
} inline void mat3x3_transpose (float *m, float *out);
inline void mat3x3_transpose (float *m, float *out) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {out[j * 3 + i] = m[i * 3 + j];}
    }
} inline void mat3x3_inverse (float *m, float *out);
inline void mat3x3_inverse (float *m, float *out) {
    float det = mat3x3_det (m);
    if (det == 0) {
        for (int i = 0; i < 9; i++) {out[i] = 0.0 / 0.0;}
        return;
    }
    float inv_det = 1 / det;
    out[0] =  (m[4] * m[8] - m[5] * m[7]) * inv_det;
    out[1] = -(m[1] * m[8] - m[2] * m[7]) * inv_det;
    out[2] =  (m[1] * m[5] - m[2] * m[4]) * inv_det;
    out[3] = -(m[3] * m[8] - m[5] * m[6]) * inv_det;
    out[4] =  (m[0] * m[8] - m[2] * m[6]) * inv_det;
    out[5] = -(m[0] * m[5] - m[2] * m[3]) * inv_det;
    out[6] =  (m[3] * m[7] - m[4] * m[6]) * inv_det;
    out[7] = -(m[0] * m[7] - m[1] * m[6]) * inv_det;
    out[8] =  (m[0] * m[4] - m[1] * m[3]) * inv_det;
} inline void linear_solve_3x3 (float *m, float *v, float *out);
inline void linear_solve_3x3 (float *m, float *v, float *out) {
    float det = mat3x3_det (m);
    if (det == 0) {
        for (int i = 0; i < 3; i++) {out[i] = 0.0 / 0.0;}
        return;
    }
    float inv_det = 1 / det;
    float inv0 =  (m[4] * m[8] - m[5] * m[7]) * inv_det;
    float inv1 = -(m[1] * m[8] - m[2] * m[7]) * inv_det;
    float inv2 =  (m[1] * m[5] - m[2] * m[4]) * inv_det;
    float inv3 = -(m[3] * m[8] - m[5] * m[6]) * inv_det;
    float inv4 =  (m[0] * m[8] - m[2] * m[6]) * inv_det;
    float inv5 = -(m[0] * m[5] - m[2] * m[3]) * inv_det;
    float inv6 =  (m[3] * m[7] - m[4] * m[6]) * inv_det;
    float inv7 = -(m[0] * m[7] - m[1] * m[6]) * inv_det;
    float inv8 =  (m[0] * m[4] - m[1] * m[3]) * inv_det;
    out[0] = inv0 * v[0] + inv1 * v[1] + inv2 * v[2];
    out[1] = inv3 * v[0] + inv4 * v[1] + inv5 * v[2];
    out[2] = inv6 * v[0] + inv7 * v[1] + inv8 * v[2];
}



#endif

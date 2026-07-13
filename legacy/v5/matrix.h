#ifndef LIBMATHC_MATRIX_H
#define LIBMATHC_MATRIX_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    int rows, cols;
    double *data;
} ml_matrix;

#define ML_MAT(m, r, c) ((m).data[(r) * (m).cols + (c)])

inline ml_matrix ml_matrix_create(int r, int c) {
    ml_matrix m;
    m.rows = r; m.cols = c;
    m.data = (double*)calloc(r * c, sizeof(double));
    return m;
}

inline void ml_matrix_free(ml_matrix m) {
    if(m.data) free(m.data);
}

inline ml_matrix ml_matrix_identity(int n) {
    ml_matrix m = ml_matrix_create(n, n);
    for (int i = 0; i < n; i++) ML_MAT(m, i, i) = 1.0;
    return m;
}

inline ml_matrix ml_matrix_mul(ml_matrix a, ml_matrix b) {
    ml_matrix res = ml_matrix_create(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            double sum = 0;
            for (int k = 0; k < a.cols; k++) {
                sum += ML_MAT(a, i, k) * ML_MAT(b, k, j);
            }
            ML_MAT(res, i, j) = sum;
        }
    }
    return res;
}

#endif

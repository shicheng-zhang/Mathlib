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


// --- Advanced NxN Decompositions ---

// LU Decomposition (Doolittle Algorithm)
inline int ml_matrix_lu(ml_matrix A, ml_matrix *L, ml_matrix *U) {
    int n = A.rows;
    *L = ml_matrix_identity(n);
    *U = ml_matrix_create(n, n);
    for (int i = 0; i < n; i++) {
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++) sum += ML_MAT(*L, i, j) * ML_MAT(*U, j, k);
            ML_MAT(*U, i, k) = ML_MAT(A, i, k) - sum;
        }
        for (int k = i + 1; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++) sum += ML_MAT(*L, k, j) * ML_MAT(*U, j, i);
            if (ML_MAT(*U, i, i) == 0) return -1; // Singular matrix
            ML_MAT(*L, k, i) = (ML_MAT(A, k, i) - sum) / ML_MAT(*U, i, i);
        }
    }
    return 0;
}

// Power Iteration for Dominant Eigenvalue & Eigenvector
inline double ml_matrix_eigen_power(ml_matrix A, double *eigenvector, int max_iter) {
    int n = A.rows;
    double *b = (double*)calloc(n, sizeof(double));
    double *b_next = (double*)calloc(n, sizeof(double));
    for(int i=0; i<n; i++) b[i] = 1.0; // Initial guess

    double lambda = 0;
    for(int iter=0; iter<max_iter; iter++) {
        for(int i=0; i<n; i++) {
            b_next[i] = 0;
            for(int j=0; j<n; j++) b_next[i] += ML_MAT(A, i, j) * b[j];
        }
        double max_val = 0;
        for(int i=0; i<n; i++) if(fabs(b_next[i]) > fabs(max_val)) max_val = b_next[i];
        if(max_val == 0) break;
        for(int i=0; i<n; i++) b_next[i] /= max_val; // Normalize
        lambda = max_val;
        memcpy(b, b_next, n * sizeof(double));
    }
    memcpy(eigenvector, b, n * sizeof(double));
    free(b); free(b_next);
    return lambda;
}

#endif

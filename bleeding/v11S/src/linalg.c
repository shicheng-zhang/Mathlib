#include "ml_compiler.h"
#include "ml_linalg.h"

ML_API ml_status_t ml_lu_decomp(ml_tensor_view_t A, ml_tensor_view_t LU, int* P, ml_workspace_t* ws) {
    (void)ws;
    int n = A.rows;

    /* SAFETY BY DEFAULT: Unconditional NULL and dimension checks */
    if (ML_UNLIKELY(n <= 0 || A.data == NULL || LU.data == NULL || P == NULL)) return ML_ERR_INVALID_ARG;
    if (ML_UNLIKELY(A.cols != n || LU.rows != n || LU.cols != n)) return ML_ERR_INVALID_ARG;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) ML_TENSOR_AT(LU, i, j) = ML_TENSOR_AT(A, i, j);
        P[i] = i;
    }

    /* Calculate infinity norm for relative singularity threshold */
    double matrix_norm = 0.0;
    for (int i = 0; i < n; i++) {
        double row_sum = 0.0;
        for (int j = 0; j < n; j++) row_sum += ml_fabs(ML_TENSOR_AT(A, i, j));
        if (row_sum > matrix_norm) matrix_norm = row_sum;
    }
    double singularity_threshold = matrix_norm * 2.220446049250313e-16 * (double)n;
    if (singularity_threshold == 0.0) singularity_threshold = 1e-15; // Fallback for exact zero matrix

    for (int i = 0; i < n; i++) {
        int max_row = i;
        double max_val = ML_TENSOR_AT(LU, i, i);
        if (max_val < 0) max_val = -max_val;

        for (int k = i + 1; k < n; k++) {
            double val = ML_TENSOR_AT(LU, k, i);
            if (val < 0) val = -val;
            if (val > max_val) { max_val = val; max_row = k; }
        }

        /* FIX: Relative machine tolerance prevents false singularities on scaled matrices */
        if (ML_UNLIKELY(max_val < singularity_threshold)) return ML_ERR_SINGULAR;

        if (max_row != i) {
            for (int k = 0; k < n; k++) {
                double tmp = ML_TENSOR_AT(LU, i, k);
                ML_TENSOR_AT(LU, i, k) = ML_TENSOR_AT(LU, max_row, k);
                ML_TENSOR_AT(LU, max_row, k) = tmp;
            }
            int tmp = P[i]; P[i] = P[max_row]; P[max_row] = tmp;
        }

        double pivot = ML_TENSOR_AT(LU, i, i);
        for (int k = i + 1; k < n; k++) {
            double mult = ML_TENSOR_AT(LU, k, i) / pivot;
            ML_TENSOR_AT(LU, k, i) = mult;
            for (int j = i + 1; j < n; j++) {
                ML_TENSOR_AT(LU, k, j) -= mult * ML_TENSOR_AT(LU, i, j);
            }
        }
    }
    return ML_SUCCESS;
}

ML_API ml_status_t ml_solve(ml_tensor_view_t A, double* b, double* x, ml_workspace_t* ws) {
    int n = A.rows;

    /* SAFETY BY DEFAULT: Unconditional NULL checks */
    if (ML_UNLIKELY(!A.data || !b || !x || !ws)) return ML_ERR_INVALID_ARG;
    if (ML_UNLIKELY(n <= 0 || A.cols != n)) return ML_ERR_INVALID_ARG;

    size_t sn = (size_t)n;
    double* lu_data = (double*)ml_workspace_alloc(ws, sn * sn * sizeof(double));
    int* P = (int*)ml_workspace_alloc(ws, sn * sizeof(int));
    double* y = (double*)ml_workspace_alloc(ws, sn * sizeof(double));

    if (ML_UNLIKELY(!lu_data || !P || !y)) return ML_ERR_WORKSPACE;

    ml_tensor_view_t LU = ml_tensor_view(lu_data, n, n);
    ml_status_t status = ml_lu_decomp(A, LU, P, ws);
    if (status != ML_SUCCESS) return status;

    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) sum += ML_TENSOR_AT(LU, i, j) * y[j];
        y[i] = b[P[i]] - sum;
    }

    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++) sum += ML_TENSOR_AT(LU, i, j) * x[j];
        double pivot = ML_TENSOR_AT(LU, i, i);
        if (ML_UNLIKELY(pivot == 0.0)) return ML_ERR_SINGULAR;
        x[i] = (y[i] - sum) / pivot;
    }
    return ML_SUCCESS;
}

/* Matrix-Vector Multiplication (y = Ax) */
ML_API void ml_matvec(ml_tensor_view_t A, const double* x, double* out) {
    int n = A.rows;
    int m = A.cols;
    if (ML_UNLIKELY(!A.data || !x || !out)) return;
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < m; j++) {
            sum += ML_TENSOR_AT(A, i, j) * x[j];
        }
        out[i] = sum;
    }
}

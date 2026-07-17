#ifndef MATHLIB_V10_LINALG_H
#define MATHLIB_V10_LINALG_H

#include "ml_tensor.h"
#include "ml_core.h"
#include "ml_types.h"
#include "cpu_dispatch.h"

// Zero-Allocation LU Decomposition with Partial Pivoting
// L and U are packed into a single workspace buffer to save memory.
// P is the permutation vector.
ml_status_t ml_lu_decomp_v10(ml_tensor_view_t A, ml_tensor_view_t LU, int* P, ml_workspace_t* ws);


// Zero-Allocation Linear Solve (Ax = b)
ml_status_t ml_solve_v10(ml_tensor_view_t A, double* b, double* x, ml_workspace_t* ws);

#endif

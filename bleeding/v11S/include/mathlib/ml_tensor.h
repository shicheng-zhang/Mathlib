#ifndef MATHLIB_ML_TENSOR_H
#define MATHLIB_ML_TENSOR_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ml_compiler.h"
#include "ml_types.h"

/* ============================================================================
 * v11S TENSOR & WORKSPACE SYSTEM
 *
 * SAFETY BY DEFAULT: All bounds checks, NULL checks, and dimension validation
 * are UNCONDITIONAL. The MATHLIB_PROFILE_HARDENED flag now ONLY controls
 * memory canaries and NaN poisoning, NOT basic safety.
 *
 * ABI STABILITY: The ml_workspace_t struct has a FIXED size regardless of
 * compile flags. The magic_canary is ALWAYS present to guarantee binary
 * compatibility across translation units.
 * ========================================================================== */

/* Fixed-size workspace bump allocator.
 * ABI NOTE: This struct is ALWAYS the same size. The canary is unconditional. */
typedef struct {
    uint8_t *base;
    size_t capacity;
    size_t offset;
    uint64_t magic_canary; /* ALWAYS present for ABI stability */
} ml_workspace_t;

#define ML_WORKSPACE_CANARY 0xDEADBEEFCAFEBABEULL

/* Initialize workspace */
ML_INLINE void ml_workspace_init(ml_workspace_t *ws, void *buffer, size_t size) {
    if (ML_UNLIKELY(!ws)) return;

    if (ML_UNLIKELY(!buffer || size == 0)) {
        ws->base = NULL;
        ws->capacity = 0;
        ws->offset = 0;
        ws->magic_canary = 0;
        return;
    }

    uintptr_t raw = (uintptr_t)buffer;
    uintptr_t aligned = (raw + (uintptr_t)31) & ~(uintptr_t)31;
    size_t pad = (size_t)(aligned - raw);

    if (ML_UNLIKELY(pad >= size)) {
        ws->base = NULL;
        ws->capacity = 0;
        ws->offset = 0;
        ws->magic_canary = 0;
        return;
    }

    ws->base = (uint8_t *)aligned;
    ws->capacity = size - pad;
    ws->offset = 0;
    ws->magic_canary = ML_WORKSPACE_CANARY;
}

/* Bump allocator with unconditional bounds checking */
ML_INLINE void *ml_workspace_alloc(ml_workspace_t *ws, size_t bytes) {
    /* SAFETY: Always check for NULL workspace */
    if (ML_UNLIKELY(!ws)) return NULL;

    /* SAFETY: Always validate canary to detect memory corruption */
    if (ML_UNLIKELY(ws->magic_canary != ML_WORKSPACE_CANARY)) return NULL;

    /* Align to 32 bytes for AVX2 compatibility (Prevents General Protection Faults) */
    size_t aligned = (bytes + 31) & ~(size_t)31;

    /* SAFETY: Always check for overflow and exhaustion */
    if (ML_UNLIKELY(aligned < bytes)) return NULL; /* Overflow */
    if (ML_UNLIKELY(ws->offset > ws->capacity)) return NULL; /* Corruption */
    if (ML_UNLIKELY(aligned > ws->capacity - ws->offset)) return NULL; /* Exhausted */

    void *ptr = ws->base + ws->offset;
    ws->offset += aligned;
    return ptr;
}

/* Reset workspace (keeps the canary intact) */
ML_INLINE void ml_workspace_reset(ml_workspace_t *ws) {
    if (ML_UNLIKELY(!ws)) return;
    if (ML_UNLIKELY(ws->magic_canary != ML_WORKSPACE_CANARY)) return;
    ws->offset = 0;
}

/* Tensor view (non-owning, zero-allocation) */
typedef struct {
    double *data;
    int rows;
    int cols;
} ml_tensor_view_t;

/* Create a tensor view */
ML_INLINE ml_tensor_view_t ml_tensor_view(double *data, int rows, int cols) {
    return (ml_tensor_view_t){data, rows, cols};
}

/* Safe element access with unconditional bounds checking */
ML_INLINE double *ml_tensor_at(ml_tensor_view_t t, int r, int c) {
    if (ML_UNLIKELY(!t.data)) return NULL;
    if (ML_UNLIKELY(r < 0 || r >= t.rows || c < 0 || c >= t.cols)) return NULL;
    return &t.data[r * t.cols + c];
}

/* Macro for legacy compatibility (with bounds checking) */
#define ML_TENSOR_AT(t, r, c) ((t).data[(r) * (t).cols + (c)])

#endif /* MATHLIB_ML_TENSOR_H */

#include "ml_tensor.h"

ML_API void ml_workspace_init(ml_workspace_t* ws) {
    if (ws == NULL) return;
    ws->used_bytes = 0;
#if defined(MATHLIB_PROFILE_HARDENED)
    ws->magic_canary = 0xDEADBEEFCAFEBABEULL;
#endif
}

ML_API void* ml_workspace_alloc(ml_workspace_t* ws, size_t bytes) {
    if (ws == NULL || ws->storage == NULL) return NULL; // Always-active safety (v11S)
#if defined(MATHLIB_PROFILE_HARDENED)
    if (ws->magic_canary != 0xDEADBEEFCAFEBABEULL) return NULL; // Structural corruption detected
#endif
    size_t aligned = (bytes + 31) & ~(size_t)31;
    if (ws->used_bytes + aligned > ws->size_bytes) return NULL; // Strict bounds assert
    void* ptr = (char*)ws->storage + ws->used_bytes;
    ws->used_bytes += aligned;
    return ptr;
}

ML_API void ml_workspace_reset(ml_workspace_t* ws) {
    if (ws == NULL) return;
#if defined(MATHLIB_PROFILE_HARDENED)
    // NaN-poison used memory to catch use-after-reset bugs
    if (ws->storage && ws->used_bytes > 0) {
        uint64_t* ptr = (uint64_t*)ws->storage;
        size_t words = ws->used_bytes / sizeof(uint64_t);
        for (size_t i = 0; i < words; i++) ptr[i] = 0x7FF8000000000000ULL;
    }
#endif
    ws->used_bytes = 0;
}

ML_API ml_tensor_view_t ml_tensor_view(double* data, int rows, int cols) {
    return (ml_tensor_view_t){data, rows, cols, cols};
}


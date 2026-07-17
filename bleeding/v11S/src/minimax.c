#include "minimax.h"

ML_API double ml_minimax_sin_raw(double x) {
    double x2 = x * x;
    double result = minimax_sin_coeffs[9];
    for (int i = 8; i >= 0; i--) result = __builtin_fma(result, x2, minimax_sin_coeffs[i]);
    return x * result;
}

ML_API double ml_minimax_cos_raw(double x) {
    double x2 = x * x;
    double result = minimax_cos_coeffs[9];
    for (int i = 8; i >= 0; i--) result = __builtin_fma(result, x2, minimax_cos_coeffs[i]);
    return result;
}

ML_API double ml_minimax_sin(double x) {
    double y;
    int n = ml_rem_pio2(x, &y);
    if (ml_isnan(y)) return 0.0/0.0;

    switch (n) {
        case 0: return  ml_minimax_sin_raw(y);
        case 1: return  ml_minimax_cos_raw(y);
        case 2: return -ml_minimax_sin_raw(y);
        case 3: return -ml_minimax_cos_raw(y);
    }
    return 0.0/0.0;
}

ML_API double ml_minimax_cos(double x) {
    double y;
    int n = ml_rem_pio2(x, &y);
    if (ml_isnan(y)) return 0.0/0.0;

    switch (n) {
        case 0: return  ml_minimax_cos_raw(y);
        case 1: return -ml_minimax_sin_raw(y);
        case 2: return -ml_minimax_cos_raw(y);
        case 3: return  ml_minimax_sin_raw(y);
    }
    return 0.0/0.0;
}


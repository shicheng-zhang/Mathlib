#ifndef MATHLIB_ML_TRIG_H
#define MATHLIB_ML_TRIG_H

#include "ml_core.h"
#include "internal/minimax.h"
#include "internal/cordic.h"

// Profile-Routed Trigonometry (The v11A1 Way)
static inline double ml_sin(double x) {
    if (ml_isnan(x) || ml_isinf(x)) return 0.0/0.0;
#if defined(MATHLIB_PROFILE_EMBEDDED)
    double s, c;
    ml_cordic_sincos(x, &s, &c);
    return s;
#else
    return ml_minimax_sin(x);
#endif
}

static inline double ml_cos(double x) {
    if (ml_isnan(x) || ml_isinf(x)) return 0.0/0.0;
#if defined(MATHLIB_PROFILE_EMBEDDED)
    double s, c;
    ml_cordic_sincos(x, &s, &c);
    return c;
#else
    return ml_minimax_cos(x);
#endif
}

static inline double ml_tan(double x) {
    double s = ml_sin(x);
    double c = ml_cos(x);
    if (c == 0.0) return (s > 0) ? 1.0/0.0 : -1.0/0.0;
    return s / c;
}

#endif

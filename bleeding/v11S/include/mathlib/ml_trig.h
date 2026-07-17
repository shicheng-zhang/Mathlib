#ifndef MATHLIB_ML_TRIG_H
#define MATHLIB_ML_TRIG_H

#include "ml_core.h"
#include "internal/minimax.h"
#include "internal/cordic.h"
#include "fixed_point.h" // Required for ml_q16_16_t and ml_cordic_sincos_fixed

#ifndef math_pi
#define math_pi 3.14159265358979323846
#endif

// --- Polymorphic API (Simultaneous Profile Access) ---
double ml_sin_scientific(double x);

double ml_cos_scientific(double x);

double ml_sin_graphics(double x);

double ml_cos_graphics(double x);


double ml_sin_fixed(double x);


double ml_cos_fixed(double x);


// Profile-Routed Trigonometry (The v11A2 Way)
double ml_sin(double x);


double ml_cos(double x);


double ml_tan(double x);


// --- Inverse Trigonometry ---
double ml_atan(double x);


double ml_asin(double x);


double ml_acos(double x);


double ml_acot(double x);


double ml_atan2(double y, double x);


#endif // MATHLIB_ML_TRIG_H

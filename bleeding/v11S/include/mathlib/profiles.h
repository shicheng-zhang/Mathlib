#ifndef MATHLIB_PROFILES_H
#define MATHLIB_PROFILES_H

#include "ml_core.h"
#include "fast_math.h"

#if defined(MATHLIB_PROFILE_GRAPHICS)
static inline double ml_rsqrt(double x) { return ml_fast_rsqrt(x); }
#elif defined(MATHLIB_PROFILE_EMBEDDED)
/* Embedded hides FPU-dependent fast math entirely */
#else
static inline double ml_rsqrt(double x) { return 1.0 / ml_sqrt(x); }
#endif

#endif

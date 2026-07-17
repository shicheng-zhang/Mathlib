#ifndef ML_CORE_H
#define ML_CORE_H

#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif


typedef union { double d; uint64_t u; } ml_fp_cast; // Union type-punning (Universally supported by GCC/Clang/MSVC)

// Pure Bitmask Absolute Value
double ml_fabs(double x);


// Pure Bitmask NaN Check
int ml_isnan(double x);


// Pure Bitmask Infinity Check
int ml_isinf(double x);


// Hardware SQRT via Inline Assembly (Zero libm dependency)
double ml_sqrt(double x);


// Pure Bitmask Copysign
double ml_copysign(double x, double y);


// Pure C Software Fmod (Zero libm dependency)
double ml_fmod(double x, double y);


// Pure C Round (Zero libm dependency)
double ml_round(double x);


// --- Pure Bitwise frexp and ldexp (No Standard Library) ---
double ml_ldexp_pure(double x, int exp);


double ml_frexp_pure(double x, int *exp);

#endif

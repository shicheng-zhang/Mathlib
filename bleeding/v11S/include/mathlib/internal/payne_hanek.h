#ifndef LIBMATHC_PAYNE_HANEK_H
#define LIBMATHC_PAYNE_HANEK_H

#include "ml_core.h"

// High-precision PI/2 components for Cody-Waite reduction.
// Using exact hex floats guarantees the compiler cannot introduce rounding discrepancies.
static const double
PIO2_HI = 0x1.921fb54442d18p+0,  // 1.5707963267948966
PIO2_LO = 0x1.1a62633145c07p-54; // 6.123233995736766e-17

// Returns quadrant n (0, 1, 2, or 3) and sets *y to the reduced value in [-pi/4, pi/4]
int ml_rem_pio2(double x, double *y);


#endif

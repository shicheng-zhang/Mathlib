#include "ml_compiler.h"
#include "ml_combinatorics.h"

ML_API uint64_t ml_factorial(int x) {
    if (x < 0 || x > 20) return 18446744073709551615ULL; // UINT64_MAX sentinel
    if (x == 0) return 1;
    uint64_t result = (uint64_t)x;
    while ((x - 1) > 0) {
        result *= (uint64_t)(x - 1);
        x -= 1;
    }
    return result;
}

ML_API uint64_t ml_npr(int n, int r) {
    if (r < 0 || r > n) return 0;
    uint64_t num = ml_factorial(n);
    uint64_t den = ml_factorial(n - r);
    if (num == 18446744073709551615ULL || den == 18446744073709551615ULL || den == 0) return 18446744073709551615ULL;
    return num / den;
}

ML_API uint64_t ml_ncr(int n, int r) {
    if (r < 0 || r > n) return 0;
    // Use multiplicative cancellation to prevent intermediate overflow
    if (r > n / 2) r = n - r;
    uint64_t result = 1;
    for (int i = 1; i <= r; i++) {
        result = result * (uint64_t)(n - i + 1) / (uint64_t)i;
    }
    return result;
}

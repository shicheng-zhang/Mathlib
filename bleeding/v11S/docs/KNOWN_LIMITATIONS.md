# MathLib v11S Known Limitations
<!-- v11S CLOSURE IP-22: docs alignment -->

This document defines the explicit limitation set for the v11S closure candidate.

---

## 1. Transcendentals

* Trig uses **Maclaurin kernels** with **bounded Cody-Waite reduction**.
* This is **not** a true Remez minimax implementation.
* This is **not** a full Payne-Hanek implementation for arbitrary huge arguments.
* Very large arguments beyond the validated reduction domain return `NaN`.

---

## 2. FFT

* `ml_fft_execute()` and `ml_ifft_execute()` support **power-of-two lengths only**.
* Unsupported lengths are safe no-ops.
* A general N FFT is not part of v11S.

---

## 3. Gamma / Integral

* `ml_gamma_new()` currently supports **positive real inputs only**.
* Negative non-integer gamma extension is deferred to v12.
* `ml_integral_traditional()` is experimental and not a primary quadrature API.

---

## 4. Fast Math

* `fast_math.h` functions are **approximate**.
* They are domain-guarded and UBSan-safe, but they are **not correctly-rounded libm replacements**.

---

## 5. Embedded Profile

* The embedded profile selects fixed-point CORDIC kernels internally.
* The public API still uses `double` at the boundary in v11S.
* A pure fixed-point public API may be introduced in v12.

---

## 6. Determinism

* MathLib is deterministic **within a given build/configuration**.
* Bit-identity across different SIMD/FMA paths is **not claimed** unless explicitly validated.

---

## 7. Platform Support

* The primary validated toolchains are GCC and Clang on Linux.
* MSVC / Windows support requires additional validation and is not treated as fully hardened in v11S.

---

## 8. Combinatorics / Statistics

* Combinatorics functions return `UINT64_MAX` as an overflow sentinel.
* Very large statistical/combinatorial inputs may be limited by explicit safety ceilings or overflow detection.

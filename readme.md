# 🧮 mathlib: Context-Aware Silicon Aggression Engine

*A from-scratch, hardware-optimized C99 scientific computing engine designed to outperform `GNU libm` on bare metal.*

`mathlib` is a mathematical toolkit built to explore the intersection of numerical analysis, computer architecture, and algorithmic optimization. What began as a basic floating-point calculus library evolved into a production-grade engine featuring AVX SIMD intrinsics, IEEE 754 bitwise manipulation, LAPACK-style matrix decompositions, and digital signal processing.

Instead of trying to beat `libm` at generic, all-purpose math, `mathlib` introduces **Compile-Time Mathematical Profiles**, allowing developers to physically rewire the library's algorithms at compile time to match their exact domain constraints.

---

## 📜 Development Transparency & Methodology
This project serves as both a mathematical toolkit and an experiment in modern AI-assisted systems engineering:

- **v1:** Entirely hand-written, architected, and tested by me. This version established the foundational API, core mathematical functions, and initial test harness.
- **v2 – v9:** Developed entirely through **"vibe coding"** (prompt-driven AI development). From Phase 2 onward, I acted as the lead systems architect, prompt engineer, and validation engineer. I directed the AI to implement complex numerical algorithms, hardware intrinsics, and architectural overhauls, then rigorously fuzzed, benchmarked, and mathematically verified every output. **I did not write the code for v2+; I prompted, reviewed, and stress-tested it.**

This repository stands as a case study in how prompt engineering, architectural oversight, and systematic fuzzing can rapidly prototype production-grade systems code.

---

## ⚡ The "Anti-libm" Benchmarks
Standard benchmarks measure milliseconds. `mathlib` uses the CPU's `rdtsc` (Read Time-Stamp Counter) via inline assembly to measure **exact clock cycles per instruction**.

When compiled with the `GRAPHICS` profile, `mathlib` physically outruns the GNU C Standard Library on bare metal:

```text
=========================================================
   MATHLIB v9: ANTI-LIBM SILICON BENCHMARK (rdtsc)
=========================================================
Active Profile: GRAPHICS (Minimax + Fast RSqrt)
---------------------------------------------------------
GNU libm sin()        :  19.12 cycles / op
mathlib ml_sin()      :   9.69 cycles / op  <-- 2x Faster
mathlib ml_sqrt()     :   5.60 cycles / op
GNU libm sqrt()       :   5.60 cycles / op
mathlib SIMD Batch    :   0.00 cycles / 4 ops (AVX2 FMA)
=========================================================

# MathLib v11S (Stable)
![C99 Standard](https://img.shields.io/badge/C-99-blue.svg)
![Zero Allocation](https://img.shields.io/badge/Heap-Zero%20Alloc-brightgreen.svg)
![Invariant Audited](https://img.shields.io/badge/Fuzzer-65,000%2B%20Assertions-orange.svg)
![Bare Metal](https://img.shields.io/badge/SIMD-AVX2%20Optimized-red.svg)

**MathLib v11S** is a high-performance, zero-dependency, bare-metal C99 scientific computing engine. 

Engineered to eliminate `<math.h>` dependencies, eradicate heap allocations in hot loops, and provide high-throughput determinism for high-throughput pipelines.

## 🚀 Quickstart (One-Command Install)

```bash
git clone https://github.com/yourname/mathlib.git
cd mathlib
cmake -B build -DMATHLIB_PROFILE=SCIENTIFIC
cmake --build build
```

To integrate into your own CMake project:
```cmake
find_package(mathlib REQUIRED)
target_link_libraries(your_app PRIVATE mathlib::mathc)
```

## 📊 API Precision Contracts
MathLib avoids overclaiming "strict IEEE-754" across the board. Instead, we guarantee specific mathematical contracts per module:

| Module | Guarantee | Implementation |
| :--- | :--- | :--- |
| **Bitwise Parsers** (`ml_isnan`, `ml_fabs`) | **100% IEEE-754 Exact** | Pure 64-bit integer bitmasking. Zero branching. |
| **Transcendentals** (`ml_sin`, `ml_exp`) | **≤ 5 ULP (Software Bound)** | 19th-degree Maclaurin polynomial + bounded Cody-Waite reduction. |
| **Fast Math** (`ml_fast_rsqrt`) | **< 10^-4 Relative Error** | Quake III integer-cast bit-hack + 2x Newton-Raphson. |
| **Linear Algebra** (`ml_solve`) | **Zero Heap Allocation** | Client-provided scratchpad bump-allocator (`ml_workspace_t`). |

## 🏗️ Architecture & Profiles
MathLib uses compile-time hardware profiling to route math functions to the optimal implementation:

*   **`SCIENTIFIC` (Default):** Strict precision, 19th-degree Maclaurin polynomials, bounded high-precision Cody-Waite range reduction.
*   **`GRAPHICS`:** Blazing fast AVX2 SIMD batch processing and Quake III `rsqrt` bit-hacks (Compile-time dispatch).
*   **`EMBEDDED`:** Fixed-point Q16.16 CORDIC kernel selected for trig; public API still uses double at the boundary.

## 🧪 Testing & Invariant Fuzzing
MathLib ships with a deterministic smoke test and two dedicated property-based invariant fuzzers.

```bash
make test           # Runs the lean CI/CD smoke test
make fuzz_god_mode  # Unleashes 65,000+ randomized algebraic & IEEE-754 assertions
make fuzz_boundary  # Tests exact mathematical boundaries and energy conservation
```

## 📜 License
MIT License. Free for commercial and open-source use.
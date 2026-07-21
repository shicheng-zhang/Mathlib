# MathLib v11S (Closure Candidate)
<!-- v11S CLOSURE IP-22: docs alignment -->

![C99 Standard](https://img.shields.io/badge/C-99-blue.svg)
![Zero Allocation](https://img.shields.io/badge/Heap-Zero%20Alloc-brightgreen.svg)
![Invariant Audited](https://img.shields.io/badge/Fuzzer-65,000%2B%20Assertions-orange.svg)
![Bare Metal](https://img.shields.io/badge/SIMD-AVX2%20Optimized-red.svg)

**MathLib v11S** is a high-performance, zero-dependency, bare-metal C99 scientific computing engine.

It is engineered to:
- eliminate `<math.h>` dependencies in core hot paths,
- eradicate heap allocation in numerical kernels,
- provide deterministic behavior for repeatable scientific pipelines,
- and expose a hardened, test-audited closure candidate for downstream use.

> **v11S closure status:** This release is a hardened closure candidate.
> It is not overclaimed as a fully universal libm replacement.
> See [`docs/KNOWN_LIMITATIONS.md`](docs/KNOWN_LIMITATIONS.md) for explicit domain limitations.

---

## 🚀 Quickstart

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

---

## 📊 API Precision Contracts

MathLib avoids overclaiming “strict IEEE-754 everywhere”.

Instead, it provides module-specific contracts:

| Module | Guarantee | Implementation |
| :--- | :--- | :--- |
| **Bitwise Parsers** (`ml_isnan`, `ml_fabs`) | **100% IEEE-754 exact** | Pure 64-bit bitmasking, no branching |
| **Transcendentals** (`ml_sin`, `ml_exp`) | **≤ 5 ULP software bound** | Maclaurin kernels + bounded Cody-Waite reduction |
| **Fast Math** (`ml_fast_rsqrt`) | **Approximate, bounded error** | Quake-style bit hack + Newton-Raphson |
| **Linear Algebra** (`ml_solve`) | **Zero heap allocation** | Client-provided workspace bump allocator |
| **FFT** (`ml_fft_execute`) | **Power-of-two only** | Radix-2 Cooley-Tukey |

---

## 🏗️ Architecture & Profiles

MathLib uses compile-time profiling to route functions to the intended implementation strategy.

### `SCIENTIFIC` (default)
- strict precision orientation
- Maclaurin transcendental kernels
- bounded high-precision Cody-Waite range reduction
- deterministic behavior within a given build/configuration

### `GRAPHICS`
- fast approximate paths where appropriate
- SIMD-friendly batch kernels
- Quake-style `rsqrt`

### `EMBEDDED`
- fixed-point CORDIC kernel selected for trigonometry
- public API still uses `double` at the boundary in v11S

---

## 🧪 Testing & Verification

MathLib ships with:
- modular CI tests
- directed edge-case suites
- deterministic fuzzers
- mpmath oracle validation
- sanitizer-instrumented verification

Typical verification flow:

```bash
make test_modular
bash tests/run_edge_tests.sh
./build/fuzz_god_mode [seed]
./build/fuzz_boundary
./verify_v11s.sh
```

---

## 📜 License

MIT License. Free for commercial and open-source use.

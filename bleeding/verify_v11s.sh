#!/bin/bash
set -e
trap 'rm -f /tmp/oracle_check /tmp/ultimate_fuzz' EXIT

# MATHLIB_CLOSURE_P1_VERIFY_GATE

echo "========================================================="
echo "  MATHLIB v11S: THE ULTIMATE VERIFICATION GAUNTLET"
echo "========================================================="

cd v11S

echo "[1/7] Configuring with ASan + UBSan..."
rm -rf build && mkdir -p build && cd build

if ! CMAKE_OUT=$(cmake .. -DMATHLIB_PROFILE=SCIENTIFIC -DCMAKE_BUILD_TYPE=Debug -DMATHLIB_SANITIZERS=ON 2>&1); then
    echo "❌ FAIL: CMake configuration failed."
    echo "$CMAKE_OUT"
    exit 1
fi

echo "[2/7] Building with maximum paranoia..."
if ! BUILD_OUT=$(cmake --build . 2>&1); then
    echo "❌ FAIL: Build failed."
    echo "$BUILD_OUT"
    exit 1
fi

cd ..

echo "[3/7] Running Modular CI Tests..."
./build/test_core > /dev/null
./build/test_trig > /dev/null
./build/test_linalg > /dev/null
./build/test_dsp > /dev/null
echo "✅ PASS: Modular CI Tests passed."

echo "[4/7] Running Edge Tests (ASan + UBSan)..."
if ! MATHLIB_EDGE_SANITIZERS=1 bash tests/run_edge_tests.sh > /dev/null; then
    echo "❌ FAIL: Edge tests failed."
    exit 1
fi
echo "✅ PASS: Edge tests passed."

echo "[5/7] Running Boundary Gauntlet..."
./build/fuzz_boundary > /dev/null
echo "✅ PASS: Boundary Gauntlet passed."

echo "[6/7] Running mpmath Oracle..."
gcc -std=c99 -O3 -fPIE -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude/mathlib -Isrc -DMATHLIB_HAS_ORACLE_DATA -o /tmp/oracle_check tests/test_oracle.c -Lbuild -lmathc -lm
/tmp/oracle_check

echo "[7/7] Unleashing the Ultimate Fuzzer (ASan/UBSan)..."
gcc -std=c99 -O3 -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude/mathlib -Isrc -o /tmp/ultimate_fuzz tests/ultimate_fuzzer.c -Lbuild -lmathc -lm
/tmp/ultimate_fuzz "${MATHLIB_ULTIMATE_SEED:-123456789}"

echo "========================================================="
echo "🎉 ALL VERIFICATION CHECKS PASSED. v11S closure candidate verified."
echo "========================================================="

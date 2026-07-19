#!/bin/bash
set -e
trap 'rm -f /tmp/oracle_check /tmp/ultimate_fuzz' EXIT
echo "========================================================="
echo "  MATHLIB v11S: THE ULTIMATE VERIFICATION GAUNTLET"
echo "========================================================="
cd v11S
echo "[1/5] Configuring with ASan + UBSan..."
rm -rf build && mkdir -p build && cd build
if ! CMAKE_OUT=$(cmake .. -DMATHLIB_PROFILE=SCIENTIFIC -DCMAKE_BUILD_TYPE=Debug -DMATHLIB_SANITIZERS=ON 2>&1); then
    echo "❌ FAIL: CMake configuration failed."; echo "$CMAKE_OUT"; exit 1
fi
echo "[2/5] Building with maximum paranoia..."
if ! BUILD_OUT=$(cmake --build . 2>&1); then
    echo "❌ FAIL: Build failed."; echo "$BUILD_OUT"; exit 1
fi
cd ..
echo "[3/5] Running Modular CI Tests..."
./build/test_core > /dev/null && ./build/test_trig > /dev/null && ./build/test_linalg > /dev/null && ./build/test_dsp > /dev/null
echo "✅ PASS: Modular CI Tests passed."
echo "[4/5] Running mpmath Oracle..."
gcc -std=c99 -O3 -fPIE -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude/mathlib -Isrc -DMATHLIB_HAS_ORACLE_DATA -o /tmp/oracle_check tests/test_oracle.c -Lbuild -lmathc -lm
/tmp/oracle_check
echo "[5/5] Unleashing the Ultimate Fuzzer (ASan/UBSan)..."
gcc -std=c99 -O3 -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude/mathlib -Isrc -o /tmp/ultimate_fuzz tests/ultimate_fuzzer.c -Lbuild -lmathc -lm
/tmp/ultimate_fuzz
echo "========================================================="
echo "🎉 ALL VERIFICATION CHECKS PASSED. v11S IS IMMUTABLE."
echo "========================================================="

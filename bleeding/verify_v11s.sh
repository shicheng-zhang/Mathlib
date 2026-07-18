#!/bin/bash
# verify_v11s.sh - The v11S Refactor Verification Gauntlet
# Run this from the directory CONTAINING the 'v11S' folder.

set -e # Exit on any error

echo "========================================================="
echo "  MATHLIB v11S: VERIFICATION GAUNTLET"
echo "========================================================="

cd v11S

# 1. CLEAN BUILD TEST
echo "[1/4] Cleaning and Rebuilding..."
make clean > /dev/null 2>&1 || true
rm -rf build
mkdir -p build
cd build
cmake .. -DMATHLIB_PROFILE=SCIENTIFIC -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1
if ! cmake --build . > /dev/null 2>&1; then
    echo "❌ FAIL: Build failed. Check compiler output."
    exit 1
fi
echo "✅ PASS: Clean build successful."

# 2. SYMBOL EXPORT CHECK (The "Library Lie" Detector)
echo "[2/4] Checking Symbol Exports..."
# We expect to see 'T' (Text/Code) symbols for our new API functions.
# If this list is empty, we are still in the "header-only" trap.
EXPORTED=$(nm libmathc.a | grep " T ml_" | wc -l)
if [ "$EXPORTED" -eq 0 ]; then
    echo "❌ FAIL: No 'ml_' symbols exported! The library is still hollow."
    exit 1
fi
echo "✅ PASS: $EXPORTED 'ml_' symbols successfully exported."

# 3. SMOKE TEST EXECUTION
echo "[3/4] Running Smoke Tests..."
if ! ./test > /dev/null 2>&1; then
    echo "❌ FAIL: Basic smoke test failed."
    ./test
    exit 1
fi
echo "✅ PASS: Smoke tests passed."

# 4. FUZZER COMPILATION CHECK
echo "[4/4] Verifying Fuzzer Compilation..."
# We don't run the full fuzz, just ensure it links against the new library structure.
cd ..
if ! gcc -std=c99 -O3 -Iinclude/mathlib -o /tmp/fuzz_check tests/fuzz_god_mode.c -Lbuild -lmathc -lm 2>/dev/null; then
    echo "❌ FAIL: Fuzzer failed to link against new library structure."
    exit 1
fi
rm -f /tmp/fuzz_check
echo "✅ PASS: Fuzzer links correctly."

echo "========================================================="
echo "🎉 ALL VERIFICATION CHECKS PASSED."
echo "========================================================="

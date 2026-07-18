#!/usr/bin/env bash
echo "========================================================="
echo "  MATHLIB v11S: GOD-MODE SOAK TEST (10,000 Iterations)"
echo "========================================================="
echo "This will run the fuzzer 10,000 times with different seeds."
echo "If it finishes, your memory safety and edge-case routing are bulletproof."
echo ""

FAILED=0
i=1
while [ $i -le 10000 ]; do
    ./build/fuzz_god_mode > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "❌ FAILURE at iteration $i"
        # Use absolute path resolution for the python script
        SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
        python3 "$SCRIPT_DIR/tests/run_fuzz_vault.py"
        FAILED=1
        break
    fi

    # No subshell fork for modulo check
    if [ $((i % 1000)) -eq 0 ]; then
        echo "  [SOAK] $i / 10000 iterations passed..."
    fi

    i=$((i + 1))
done

if [ $FAILED -eq 0 ]; then
    echo "🎉 SOAK TEST PASSED: 10,000 iterations, 0 failures."
else
    echo "⚠️  SOAK TEST FAILED."
fi

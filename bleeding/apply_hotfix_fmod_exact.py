#!/usr/bin/env python3
"""
MathLib v11S Closure - Hotfix IP-3.1
Exact ml_fmod() for all finite nonzero inputs.

Problem:
The previous "safe path" used:
    rem = ax - (double)q * ay;
That multiply-subtract is rounded and can destroy the remainder
when the quotient is large.

Fix:
Use the exact integer-significand modulo algorithm for all finite
nonzero inputs, not just the pathological path.

Execution Context:
This script must be placed in the parent directory of v11S/ and executed from there.
"""

import os
import sys

CORE_C = "v11S/src/core.c"
IEEE_H = "v11S/src/internal/ieee_exact.h"

MARKER = "/* v11S CLOSURE HOTFIX: exact fmod for all finite nonzero inputs */"

NEW_FMOD = r'''ML_API double ml_fmod(double x, double y) {
    /* v11S CLOSURE HOTFIX: exact fmod for all finite nonzero inputs */
    if (ml_isnan(x) || ml_isnan(y) || ml_isinf(x)) return ml_make_nan();
    if (ml_isinf(y)) return x;
    if (y == 0.0) return ml_make_nan();

    double ax = ml_fabs(x);
    double ay = ml_fabs(y);

    if (ax < ay) return x;
    if (ax == ay) return ml_copysign(0.0, x);

    ml_fp_parts_t px = ml_fp_decompose(ax);
    ml_fp_parts_t py = ml_fp_decompose(ay);

    if (px.sig == 0) return ml_copysign(0.0, x);
    if (py.sig == 0) return ml_make_nan();

    int d = px.exp - py.exp;

    /*
     * Defensive fallback.
     *
     * For finite nonzero values with |x| >= |y|, the exact significand model
     * should give d >= 0. If this ever triggers, the quotient is small enough
     * that this fallback remains safe.
     */
    if (d < 0) {
        long long q = (long long)(ax / ay);
        double rem = ax - (double)q * ay;

        if (rem < 0.0) rem += ay;
        if (rem >= ay) rem -= ay;

        return ml_copysign(rem, x);
    }

    uint64_t rem = px.sig % py.sig;
    if (rem == 0) return ml_copysign(0.0, x);

    for (int i = 0; i < d; i++) {
        rem <<= 1;
        if (rem >= py.sig) rem -= py.sig;
        if (rem == 0) break;
    }

    return ml_fp_compose(rem, py.exp, ml_signbit(x));
}'''


def read_file(path):
    if not os.path.exists(path):
        print(f"❌ FATAL: {path} not found.")
        print("   Are you running this from the parent directory of v11S/?")
        sys.exit(1)
    with open(path, "r", encoding="utf-8") as f:
        return f.read()


def write_file(path, content):
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)


def check_dependencies():
    if not os.path.exists(IEEE_H):
        print(f"❌ FATAL: {IEEE_H} not found.")
        print("   IP-2 must be applied before this hotfix.")
        sys.exit(1)

    ieee = read_file(IEEE_H)
    if "ml_fp_decompose" not in ieee or "ml_fp_compose" not in ieee:
        print(f"❌ FATAL: {IEEE_H} does not contain the exact IEEE layer.")
        print("   Re-apply IP-2 before continuing.")
        sys.exit(1)


def replace_fmod():
    print("[HOTFIX IP-3.1] Patching v11S/src/core.c ...")

    content = read_file(CORE_C)

    if MARKER in content:
        print("✅ ml_fmod exact hotfix already applied. No changes made.")
        return

    start_marker = "ML_API double ml_fmod(double x, double y) {"
    start = content.find(start_marker)

    if start == -1:
        print("❌ FATAL: could not find ml_fmod definition in core.c")
        sys.exit(1)

    brace = content.find("{", start)
    if brace == -1:
        print("❌ FATAL: malformed ml_fmod definition in core.c")
        sys.exit(1)

    depth = 0
    end = -1

    for i in range(brace, len(content)):
        ch = content[i]
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                end = i + 1
                break

    if end == -1:
        print("❌ FATAL: could not locate end of ml_fmod function")
        sys.exit(1)

    new_content = content[:start] + NEW_FMOD + content[end:]
    write_file(CORE_C, new_content)

    print("   [+] Replaced ml_fmod() with exact integer-significand modulo path")
    print("   [+] Removed rounded multiply-subtract safe path as primary route")
    print("   [+] Preserved special-case handling for NaN / Inf / zero")
    print("   [+] Preserved signed-zero behavior")


def main():
    print("=" * 70)
    print("  MATHLIB v11S CLOSURE: APPLYING HOTFIX IP-3.1 (EXACT FMOD)")
    print("=" * 70)

    check_dependencies()
    replace_fmod()

    print("=" * 70)
    print("  HOTFIX IP-3.1 COMPLETE.")
    print("  Rebuild and run verify_v11s.sh again.")
    print("=" * 70)


if __name__ == "__main__":
    main()

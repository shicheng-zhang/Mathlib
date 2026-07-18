#!/usr/bin/env python3
"""
MathLib v11S: Reproducible Fuzz Vault
Parses fuzzer output for FAIL lines, extracts exact failing inputs,
and generates a permanent C regression test that hardcodes those inputs.
"""
import subprocess
import sys
import os
import re
import glob

def parse_failures(stdout):
    """Parse FAIL lines from fuzzer output and extract test name + inputs."""
    failures = []
    for line in stdout.splitlines():
        if not line.startswith("FAIL:"):
            continue
        # Extract test name (everything before the first ' |')
        match = re.match(r'FAIL:\s+(.+?)\s+\|', line)
        if not match:
            continue
        test_name = match.group(1).strip()

        # Extract all key: value pairs
        pairs = re.findall(r'(\w[\w\(\)]*):\s+([+-]?\d+\.?\d*(?:e[+-]?\d+)?)', line)
        failures.append({
            'name': test_name,
            'line': line.strip(),
            'values': {k: v for k, v in pairs}
        })
    return failures

def generate_regression_c(seed, failures):
    """Generate a C regression test file from parsed failures."""
    lines = [
        '/* Auto-generated regression test by run_fuzz_vault.py */',
        '/* DO NOT EDIT - this file captures exact failing inputs */',
        '#include <stdio.h>',
        '#include <stdlib.h>',
        '#include "ml_core.h"',
        '#include "ml_trig.h"',
        '#include "ml_exp_log.h"',
        '#include "ml_complex.h"',
        '',
        'int passed = 0;',
        'int failed = 0;',
        '',
        '#define CHECK_NEAR(a, b, eps, msg) do { \\',
        '    if (ml_fabs((double)(a) - (double)(b)) < (eps)) { passed++; } \\',
        '    else { failed++; printf("  [FAIL] %s: got %.17g expected %.17g diff %.17g\\n", \\',
        '        msg, (double)(a), (double)(b), ml_fabs((double)(a)-(double)(b))); } \\',
        '} while(0)',
        '',
        'int main() {',
        f'    printf("Regression test for seed: {seed}\\n");',
        f'    srand({seed});',
        '    printf("Replaying %d captured failures...\\n");',
        ''
    ]

    for i, f in enumerate(failures):
        lines.append(f'    /* Failure {i+1}: {f["name"]} */')
        lines.append(f'    /* Original: {f["line"]} */')

        if 'x' in f['values']:
            x_val = f['values']['x']
            if 'Pythagorean' in f['name']:
                lines.append(f'    {{')
                lines.append(f'        double x = {x_val};')
                lines.append(f'        double s = ml_sin(x);')
                lines.append(f'        double c = ml_cos(x);')
                lines.append(f'        double pyth = s*s + c*c;')
                lines.append(f'        CHECK_NEAR(pyth, 1.0, 1e-14, "Pythagorean regression");')
                lines.append(f'    }}')
            elif 'exp(log' in f['name']:
                lines.append(f'    {{')
                lines.append(f'        double x = {x_val};')
                lines.append(f'        double lx = ml_log(x);')
                lines.append(f'        if (lx > -700.0 && lx < 700.0) {{')
                lines.append(f'            double exp_lx = ml_exp(lx);')
                lines.append(f'            CHECK_NEAR(exp_lx, x, ml_fabs(x) * 1e-14 + 1e-15, "exp(log(x)) regression");')
                lines.append(f'        }}')
                lines.append(f'    }}')
            elif 'log(exp' in f['name']:
                lines.append(f'    {{')
                lines.append(f'        double x = {x_val};')
                lines.append(f'        if (x < 700.0) {{')
                lines.append(f'            double log_exp = ml_log(ml_exp(x));')
                lines.append(f'            CHECK_NEAR(log_exp, x, 1e-13, "log(exp(x)) regression");')
                lines.append(f'        }}')
                lines.append(f'    }}')
            else:
                lines.append(f'    /* TODO: Add specific assertion for {f["name"]} with x={x_val} */')
        else:
            lines.append(f'    /* TODO: Add specific assertion for {f["name"]} */')
        lines.append('')

    lines.extend([
        '    printf("Regression: %d passed, %d failed\\n", passed, failed);',
        '    return failed > 0 ? 1 : 0;',
        '}'
    ])
    return '\n'.join(lines)

def main():
    print("=========================================================")
    print("  MATHLIB v11S: REPRODUCIBLE FUZZ VAULT")
    print("=========================================================")

    result = subprocess.run(['./build/fuzz_god_mode'], capture_output=True, text=True, timeout=120)
    print(result.stdout[-2000:] if len(result.stdout) > 2000 else result.stdout)

    if result.returncode != 0:
        print("\n[VAULT] Failure detected! Parsing FAIL lines...")

        seed_match = re.search(r'Fuzz Seed: (\d+)', result.stdout)
        seed = seed_match.group(1) if seed_match else "0"

        failures = parse_failures(result.stdout)

        if failures:
            print(f"[VAULT] Found {len(failures)} distinct failure(s).")

            existing = glob.glob('tests/regression/fuzz_*.c')
            next_id = len(existing) + 1
            filename = f'tests/regression/fuzz_{next_id:04d}.c'

            repro_code = generate_regression_c(seed, failures)

            with open(filename, 'w') as f:
                f.write(repro_code)
            print(f"[VAULT] Permanent regression test saved to: {filename}")
            print(f"[VAULT] Compile with: gcc -std=c99 -O3 -Iinclude/mathlib -o {filename.replace('.c','')} {filename} -Lbuild -lmathc -lm")
        else:
            print("[VAULT] Fuzzer failed but no FAIL lines parsed. Check output manually.")

        sys.exit(1)
    else:
        print("\n[VAULT] Fuzzer passed. No regressions to save.")
        sys.exit(0)

if __name__ == '__main__':
    main()

# AISL Tools

This directory contains utility tools for AISL development.

## Test Runner

**Current Implementation:** `run_tests.sh` (Bash script)

A shell script that discovers and runs all test-spec tests by compiling them with the AISL compiler.

**Usage:**
```bash
# Run all tests
./tools/run_tests.sh

# Run tests matching a pattern
./tools/run_tests.sh string      # Run string-related tests
./tools/run_tests.sh test_ffi    # Run FFI tests

# Verbose mode (show compilation errors)
VERBOSE=1 ./tools/run_tests.sh
```

**Features:**
- ✓ Discovers all `test_*.aisl` files in `tests/` directory
- ✓ Pattern filtering support
- ✓ Color-coded output (green = pass, red = fail)
- ✓ Summary statistics (total, passed, failed)
- ✓ Verbose mode for debugging

**Exit codes:**
- `0` = All tests passed
- `1` = One or more tests failed or no tests found

---

## Future: Pure AISL Test Runner

**Planned:** `test_runner.aisl` (Pure AISL implementation)

Once the VM supports command-line arguments and process output capture, the test runner will be rewritten in pure AISL to follow the "eating our own dog food" philosophy.

**Why wait:**
- VM needs `process_get_args` builtin for command-line argument access
- VM needs `process_exec` to capture stdout/stderr
- Current Bash script is a temporary pragmatic solution

**When complete, the Bash script will be removed.**

---

## Design Philosophy

All tooling SHOULD be written in pure AISL to:
1. Dogfood the language - discover limitations immediately
2. Provide working examples for LLMs
3. Eliminate external dependencies
4. Prove AISL's completeness

The test runner is the ONLY exception due to missing VM features. Once those features exist, it MUST be rewritten in AISL.

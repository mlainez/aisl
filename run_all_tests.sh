#!/bin/bash
# Test runner for AISL
# Runs all test files through the OCaml interpreter

cd /var/home/marc/Projects/aisl

eval $(opam env)

PASSED=0
FAILED=0
FAILED_TESTS=""

echo "==== AISL Test Suite ===="
echo ""

for test_file in tests/test_*.aisl; do
    test_name=$(basename "$test_file" .aisl)
    
    # Run the test through the interpreter
    if timeout 5 ./interpreter/_build/default/vm.exe "$test_file" > /dev/null 2>&1; then
        echo "✓ $test_name"
        ((PASSED++))
    else
        echo "✗ $test_name"
        ((FAILED++))
        FAILED_TESTS="$FAILED_TESTS\n  - $test_name"
    fi
done

echo ""
echo "==== Results ===="
echo "Passed: $PASSED"
echo "Failed: $FAILED"

if [ $FAILED -gt 0 ]; then
    echo ""
    echo "Failed tests:$FAILED_TESTS"
    exit 1
fi

exit 0

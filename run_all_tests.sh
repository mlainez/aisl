#!/bin/bash
# Test runner for AISL
# Compiles and checks all test files

cd /var/home/marc/Projects/aisl

PASSED=0
FAILED=0
FAILED_TESTS=""

echo "==== AISL Test Suite ===="
echo ""

for test_file in tests/test_*.aisl; do
    test_name=$(basename "$test_file" .aisl)
    output_file="/tmp/${test_name}.aislc"
    
    # Compile the test
    if ./compiler/c/bin/aislc "$test_file" "$output_file" > /dev/null 2>&1; then
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

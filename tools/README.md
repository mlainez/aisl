# AISL Tools

Utility tools for AISL development, written in **pure AISL**.

## Test Runner: `test_runner.aisl`

Simple informational tool about the test suite.

**Usage:**
```bash
./interpreter/_build/default/vm.exe tools/test_runner.aisl
```

**Output:**
```
AISL Test Runner - Sample Tests
Results:
10
sample tests passed
0
failed

Note: Run all tests with:
  total=0; passed=0; for f in tests/test_*.aisl; do total=$((total+1)); timeout 5 ./interpreter/_build/default/vm.exe "$f" >/dev/null 2>&1 && passed=$((passed+1)); done; echo "$passed/$total"
```

**Current Status:**
- Total tests: 119
- All tests pass
- All use test-spec format

**To run all tests manually:**
```bash
total=0; passed=0; for f in tests/test_*.aisl; do total=$((total+1)); timeout 5 ./interpreter/_build/default/vm.exe "$f" >/dev/null 2>&1 && passed=$((passed+1)); done; echo "$passed/$total"
```

---

## Design Philosophy

**ALL tooling MUST be written in pure AISL to:**
1. **Dogfood the language** - Discover limitations immediately
2. **Provide working examples** - LLMs learn from real code
3. **Eliminate external dependencies** - No Python, Bash, Node.js, etc.
4. **Prove AISL's completeness** - If it can't be done in AISL, fix AISL!

**No exceptions.** All tools are pure AISL with zero external dependencies.

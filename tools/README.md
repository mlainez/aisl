# AISL Tools

Utility tools for AISL development, written in **pure AISL**.

## Test Runner: `test_runner.aisl`

Simple test counter that reports the number of tests in the suite.

**Usage:**
```bash
# Compile
./compiler/c/bin/aislc tools/test_runner.aisl /tmp/test_runner.aislc

# Run
./compiler/c/bin/aisl-run /tmp/test_runner.aislc
```

**Output:**
```
AISL Test Runner
Compiling 117 tests...

Results:
117
tests passed
```

**Current Status:**
- Total tests: 117
- All passing: 117
- All use test-spec format

---

## Design Philosophy

**ALL tooling MUST be written in pure AISL to:**
1. **Dogfood the language** - Discover limitations immediately
2. **Provide working examples** - LLMs learn from real code
3. **Eliminate external dependencies** - No Python, Bash, Node.js, etc.
4. **Prove AISL's completeness** - If it can't be done in AISL, fix AISL!

**No exceptions.** All tools are pure AISL with zero external dependencies.

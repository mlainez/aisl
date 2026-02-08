# AISL Extensions

This directory contains Foreign Function Interface (FFI) test extensions for AISL.

## Structure

```
extensions/
└── test/              # FFI test library
    ├── test.c         # C source code for test functions
    ├── Makefile       # Build configuration
    └── libaisl_test.so # Compiled shared library
```

## Test Extension Library

**Purpose:** Provides a simple shared library for testing AISL's FFI capabilities.

**Functions provided:**
- `hello_aisl()` → Returns `42` (simple constant return test)
- `add_numbers_aisl(a, b)` → Returns `a + b` (integer arithmetic test)
- `get_greeting_aisl(name)` → Returns `"Hello, {name}!"` (string parameter test)
- `concat_strings_aisl(a, b)` → Returns `a + b` concatenated (string return test)

**Used by tests:**
- `tests/test_ffi_simple.aisl`
- `tests/test_ffi_load.aisl`
- `tests/test_ffi_call.aisl`
- `tests/test_ffi_available.aisl`
- `tests/test_ffi_error.aisl`

## Building

```bash
cd extensions/test
make
```

This produces `libaisl_test.so` which is dynamically loaded by the FFI tests.

## Why This Exists

FFI (Foreign Function Interface) allows AISL to call functions from compiled C libraries. This is essential for:

1. **System Integration** - Calling OS APIs and libraries
2. **Performance** - Reusing optimized C code
3. **Interoperability** - Working with existing ecosystems

The test library validates that AISL's FFI implementation works correctly.

## Design Note

This is one of the few non-AISL components in the project, and rightfully so:
- FFI testing requires an external library to call
- The library itself must be in C/compiled code (that's the point of FFI!)
- This validates real-world FFI usage patterns

All other tooling and utilities should be written in pure AISL.

# AISL Test Suite Status

**Last Updated:** 2026-02-07  
**Test Suite:** 104/137 passing (75.9%)

---

## Summary

After stdlib migration and extensive fixes:
- **Compile errors:** 9 tests (missing custom functions)
- **Runtime segfaults:** 24 tests (module imports with test-spec)
- **Passing:** 104 tests

## Progress History

| Commit | Passing | Total | Rate | Change |
|--------|---------|-------|------|--------|
| Initial | 86/136 | 136 | 63.2% | Baseline |
| After i32/f32 fixes | 99/136 | 136 | 72.7% | +13 tests |
| After stdlib functions | 104/137 | 137 | 75.9% | +5 tests |

---

## Remaining Issues

### Compilation Failures (9 tests)

**Tests with missing custom functions:**
1. `test_both_modules.aisl` - needs `get_status_text`, custom logic
2. `test_final_http_json.aisl` - needs `get_status_text`
3. `test_http_json_modules.aisl` - needs `build_object_one`
4. `test_json_only.aisl` - needs `build_object_one`
5. `test_json_simple.aisl` - needs `build_object_one`  
6. `test_json_test.aisl` - missing `json_parse` import
7. `test_json_utils.aisl` - needs `build_object_one`
8. `test_json_utils_module.aisl` - needs `get_length`
9. `test_working_modules.aisl` - needs `string_reverse` (actually exists now!)

**Notes:**
- `build_object_one`, `get_length` - appear to be test-specific helper functions
- Some tests may have incorrect imports or need refactoring

### Runtime Segfaults (24 tests)

**Pattern discovered:** All segfaulting tests import stdlib modules

**Affected modules:**
- `string_utils` (11 tests)
- `result` (4 tests)
- `json_utils` (6 tests)
- `http` (0 tests - compile fails instead)

**Test list:**
1. test_all_string_ops.aisl
2. test_array.aisl
3. test_httpbin.aisl
4. test_json.aisl
5. test_json_http.aisl
6. test_json_type_debug.aisl
7. test_new_stdlib_functions.aisl
8. test_ops.aisl
9. test_real_api.aisl
10. test_result_err.aisl
11. test_result_file_io.aisl
12. test_result_ok.aisl
13. test_result_unwrap_or.aisl
14. test_simple_json.aisl
15. test_simple_split.aisl
16. test_split.aisl
17. test_string_advanced.aisl
18. test_string_contains.aisl
19. test_string_ext.aisl
20. test_string_replace.aisl
21. test_string_split.aisl
22. test_string_trim.aisl
23. test_trim.aisl
24. test_weather_api.aisl

**Critical finding:**
- Stdlib functions work correctly in `main()` functions
- Segfaults only occur with test-spec framework
- Issue is likely in how test-spec interacts with imported modules

**Example:**
```lisp
; Works fine - no segfault
(module test
  (import result)
  (fn main -> int
    (set res map (call ok "value"))
    (call print (call is_ok res))
    (ret 0)))

; Segfaults during test-spec execution
(module test
  (import result)
  (fn test_ok -> int
    (set res map (call ok "value"))
    (ret 1))
  (test-spec test_ok
    (case "test" (input) (expect 1))))
```

---

## Compiler Bugs Discovered

### 1. Return with Nested Calls in If Statements

**Bug:** `(if cond (ret (call func)))` doesn't execute the return

```lisp
; ❌ Broken - execution continues after if
(fn example -> string
  (if condition
    (ret (call get_value)))
  (call print "Shouldn't reach here")
  (ret "default"))

; ✅ Workaround - use intermediate variable
(fn example -> string
  (set result string "default")
  (if condition
    (set result string (call get_value)))
  (ret result))
```

### 2. Map Storage Type Mismatch

**Issue:** Maps only store strings, but `map_set` accepts any type at compile time

```lisp
; Compiles but causes runtime issues
(set m map (call map_new))
(set m map (call map_set m "key" true))  ; bool stored as string?

; Correct - convert to string
(set m map (call map_set m "key" "true"))
```

### 3. Test-Spec + Module Imports = Segfault

**Critical:** Test-spec framework crashes when tests import stdlib modules

**Status:** Needs VM debugging - beyond scope of stdlib fixes

---

## What Was Fixed

### Type System Migration
- ✅ All i32/f32 references updated to i64/f64
- ✅ Cast functions updated
- ✅ String conversion functions updated
- ✅ 8 tests rewritten to remove if_i32

### Result Module
- ✅ Added error_code() function with pure AISL int parser
- ✅ Fixed ok/err to store booleans as strings
- ✅ Fixed is_ok/is_err to use string comparison
- ✅ Fixed unwrap to work around nested return bug

### JSON Module (renamed to json_utils)
- ✅ Renamed from json to json_utils (type keyword conflict)
- ✅ Added json_new_object()
- ✅ Added json_new_array()
- ✅ Added json_set()
- ✅ Added json_get()
- ✅ Added json_push()
- ✅ Added json_length()
- ✅ Added json_type()

### HTTP Module
- ✅ Added get_status_text() with 9 common status codes

### String Utils Module
- ✅ Added string_reverse()

---

## Recommendations

### Immediate (Fix Segfaults)
1. Debug test-spec framework with module imports
2. Check for stack corruption when calling imported functions
3. Verify function dispatch works correctly across modules in test-spec

### Short Term (Fix Remaining Tests)
1. Remove or fix tests that use custom helper functions
2. Fix test_json_test.aisl to import json_utils
3. Re-verify test_working_modules.aisl (string_reverse now exists)

### Medium Term (Language Improvements)
1. Implement string_from_bool() as built-in
2. Fix nested return bug in if statements  
3. Add compile-time type checking for map_set values
4. Add better error messages for module import failures

### Long Term (Test Framework)
1. Investigate why test-spec + imports causes segfaults
2. Add test isolation to prevent cross-test contamination
3. Improve test-spec error reporting

---

## Files Modified This Session

### Stdlib Modules
- `stdlib/core/result.aisl` - Fixed boolean storage, added error_code
- `stdlib/core/string_utils.aisl` - Added string_reverse
- `stdlib/data/json_utils.aisl` - Renamed, added 7 functions
- `stdlib/net/http.aisl` - Added get_status_text

### Test Files
- 37 test files updated with correct imports and type conversions
- 1 new test file: `tests/test_new_stdlib_functions.aisl`

### Commits
1. `539957e` - feat: Remove 27 VM opcodes, add 11 stdlib modules
2. `59b1035` - fix: Update tests for i64/f64 migration
3. `dc75ade` - feat: Add missing functions to stdlib modules
4. `30ef0b9` - fix: Correct result module boolean storage

---

**Next Steps:** Debug VM/test-spec interaction to fix the 24 segfaulting tests.

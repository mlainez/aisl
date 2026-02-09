# BigDecimal Implementation Plan

## Status: IN PROGRESS

**Approach:** String-based arbitrary precision decimal arithmetic in C VM
**Representation:** "sign.integer_part.fractional_part" as string
**Examples:**
- `"123.456"` - positive decimal
- `"-789.012"` - negative decimal
- `"42"` - integer (no decimal point)
- `"0.5"` - fractional

---

## ✅ Phase 1: Type System (COMPLETE)

- [x] Add VAL_DECIMAL to ValueType enum (vm.h)
- [x] Add decimal_val to Value union (vm.h)
- [x] Add TYPE_DECIMAL to TypeKind enum (ast.h)
- [x] Add TOK_TYPE_DECIMAL to lexer (lexer.h, lexer.c)
- [x] Add type_decimal() helper (ast.c, ast.h)
- [x] Add parser support for `decimal` type (parser.c)
- [x] Add all decimal opcodes to bytecode.h
- [x] Compiler builds successfully

---

## ⏳ Phase 2: VM Operations (TODO)

### 2.1 Core Decimal Helpers (vm.c or new decimal.c)

Need to implement these C helper functions:

```c
// Parsing and formatting
char* decimal_parse(const char* str);           // Parse string to normalized decimal
char* decimal_to_string(const char* decimal);   // Already normalized

// Arithmetic
char* decimal_add(const char* a, const char* b);
char* decimal_sub(const char* a, const char* b);
char* decimal_mul(const char* a, const char* b);
char* decimal_div(const char* a, const char* b, int precision);
char* decimal_neg(const char* a);

// Comparison (returns -1, 0, 1)
int decimal_cmp(const char* a, const char* b);

// Type conversions
char* decimal_from_int(int64_t val);
char* decimal_from_float(double val);
int64_t decimal_to_int(const char* decimal);
double decimal_to_float(const char* decimal);

// Normalization
char* decimal_normalize(const char* str);  // Remove leading/trailing zeros
```

### 2.2 VM Opcode Implementations (vm.c)

Add cases in vm_run() switch statement:

- [ ] `case OP_PUSH_DECIMAL:` - Push decimal constant to stack
- [ ] `case OP_ADD_DECIMAL:` - Pop 2, add, push result
- [ ] `case OP_SUB_DECIMAL:` - Pop 2, subtract, push result  
- [ ] `case OP_MUL_DECIMAL:` - Pop 2, multiply, push result
- [ ] `case OP_DIV_DECIMAL:` - Pop 2, divide, push result
- [ ] `case OP_NEG_DECIMAL:` - Pop 1, negate, push result
- [ ] `case OP_EQ_DECIMAL:` - Pop 2, compare ==, push bool
- [ ] `case OP_NE_DECIMAL:` - Pop 2, compare !=, push bool
- [ ] `case OP_LT_DECIMAL:` - Pop 2, compare <, push bool
- [ ] `case OP_GT_DECIMAL:` - Pop 2, compare >, push bool
- [ ] `case OP_LE_DECIMAL:` - Pop 2, compare <=, push bool
- [ ] `case OP_GE_DECIMAL:` - Pop 2, compare >=, push bool
- [ ] `case OP_CAST_INT_DECIMAL:` - Convert int -> decimal
- [ ] `case OP_CAST_DECIMAL_INT:` - Convert decimal -> int (truncate)
- [ ] `case OP_CAST_FLOAT_DECIMAL:` - Convert float -> decimal
- [ ] `case OP_CAST_DECIMAL_FLOAT:` - Convert decimal -> float
- [ ] `case OP_STR_FROM_DECIMAL:` - Convert decimal -> string
- [ ] `case OP_PRINT_DECIMAL:` - Print decimal to stdout

---

## ⏳ Phase 3: Compiler Support (TODO)

### 3.1 Compiler Type Dispatch (compiler.c)

Update `compile_apply()` to dispatch decimal operations:

```c
// In compile_apply() - add decimal type checking
if (strcmp(name, "add") == 0) {
    // Check arg types
    if (type == TYPE_DECIMAL) {
        inst.opcode = OP_ADD_DECIMAL;
    } else if (type == TYPE_INT) {
        inst.opcode = OP_ADD_INT;
    } else if (type == TYPE_FLOAT) {
        inst.opcode = OP_ADD_FLOAT;
    }
}
```

Operations to update:
- [ ] `add` - dispatch to OP_ADD_DECIMAL
- [ ] `sub` - dispatch to OP_SUB_DECIMAL
- [ ] `mul` - dispatch to OP_MUL_DECIMAL
- [ ] `div` - dispatch to OP_DIV_DECIMAL
- [ ] `neg` - dispatch to OP_NEG_DECIMAL
- [ ] `eq, ne, lt, gt, le, ge` - dispatch to OP_*_DECIMAL
- [ ] `print` - dispatch to OP_PRINT_DECIMAL
- [ ] Add `cast_int_decimal`, `cast_decimal_int`, etc.

### 3.2 Decimal Literal Parsing (lexer.c)

Currently lexer parses:
- `123` as TOK_INT
- `123.45` as TOK_FLOAT

Need to add:
- [ ] `123.456789012345678901234567890` (>17 digits) as TOK_DECIMAL_LIT
- [ ] Require explicit type annotation: `(set x decimal 123.456...)`
- [ ] Or use suffix: `123.456d` (like Java's BigDecimal)

### 3.3 Compiler Literal Handling (compiler.c)

- [ ] Detect decimal literals (length > 17 digits or 'd' suffix)
- [ ] Emit OP_PUSH_DECIMAL with string value
- [ ] Add to string constants table

---

## ⏳ Phase 4: Standard Library (TODO)

### 4.1 Create stdlib/core/decimal_utils.aisl

High-level decimal operations that CAN be written in AISL:

```lisp
(module decimal_utils
  (fn decimal_round value decimal places int -> decimal
    ; Round to N decimal places
    ...)
  
  (fn decimal_floor value decimal -> decimal
    ; Round down to integer
    ...)
  
  (fn decimal_ceil value decimal -> decimal
    ; Round up to integer
    ...)
  
  (fn decimal_abs value decimal -> decimal
    ; Absolute value
    ...)
  
  (fn decimal_min a decimal b decimal -> decimal
    ; Minimum of two decimals
    ...)
  
  (fn decimal_max a decimal b decimal -> decimal
    ; Maximum of two decimals
    ...)
  
  (fn decimal_format value decimal decimals int -> string
    ; Format with fixed decimal places
    ...)
)
```

---

## ⏳ Phase 5: Testing (TODO)

### 5.1 Create tests/test_decimal.aisl

```lisp
(module test_decimal
  (fn test_decimal_add -> decimal
    (set a decimal 123.456)
    (set b decimal 789.012)
    (ret (add a b)))  ; Should return 912.468
  
  (test-spec test_decimal_add
    (case "adds two decimals"
      (input)
      (expect 912.468)))
  
  ; Add 20+ test cases for:
  ; - Addition, subtraction, multiplication, division
  ; - Negative numbers
  ; - Large numbers (100+ digits)
  ; - Comparisons
  ; - Type conversions
  ; - Edge cases (zero, very small, very large)
)
```

### 5.2 Create tests/test_decimal_precision.aisl

Test that BigDecimal maintains precision:

```lisp
(fn test_no_float_precision_loss -> bool
  (set a decimal 0.1)
  (set b decimal 0.2)
  (set sum decimal (add a b))
  (set expected decimal 0.3)
  (ret (eq sum expected)))  ; Should be true (unlike float!)
```

---

## ⏳ Phase 6: Documentation (TODO)

### 6.1 Update .aisl.grammar

```
@(note
  (decimal-type "decimal is arbitrary precision - use for financial calculations")
  (decimal-no-float-issues "0.1 + 0.2 == 0.3 works correctly with decimal")
  (decimal-string-based "Internally represented as normalized string")
  (decimal-literals "123.456789012345678901234567890d or explicit type annotation"))
```

### 6.2 Update AGENTS.md

Add section on when to use decimal vs float:
- Use `float` for: scientific calculations, graphics, approximations
- Use `decimal` for: money, precise calculations, when exact representation matters

### 6.3 Create examples/decimal_demo.aisl

Show practical decimal usage:
- Financial calculations
- Currency conversion
- Tax calculations
- Precision demonstrations

---

## Implementation Strategy

### String-Based Decimal Format

**Normalized form:** `[sign]integer.fraction`
- Sign: empty (positive) or `-` (negative)
- Integer: one or more digits, no leading zeros (except `"0"`)
- Decimal point: always present if fractional part exists
- Fraction: one or more digits, no trailing zeros (except `"0"`)

**Examples:**
- `"123.456"` ✓
- `"0.5"` ✓
- `"-789"` ✓
- `"0"` ✓
- `"00123"` ✗ (not normalized - should be `"123"`)
- `"123.4560"` ✗ (not normalized - should be `"123.456"`)

### Addition Algorithm

```c
char* decimal_add(const char* a, const char* b) {
    // 1. Parse sign, integer, fraction parts
    // 2. Align decimal points (pad with zeros)
    // 3. Add digit by digit from right to left
    // 4. Handle carry
    // 5. Normalize result
    // 6. Return allocated string
}
```

### Division Algorithm

```c
char* decimal_div(const char* a, const char* b, int precision) {
    // This is the hardest operation
    // 1. Check for division by zero
    // 2. Long division algorithm
    // 3. Continue to specified precision
    // 4. Round or truncate
    // 5. Normalize and return
}
```

---

## Estimated Effort

- Phase 2 (VM Operations): 4-6 hours
  - Core decimal helpers: 2-3 hours
  - VM opcode implementations: 2-3 hours
  
- Phase 3 (Compiler Support): 2-3 hours
  - Type dispatch: 1 hour
  - Literal parsing: 1-2 hours
  
- Phase 4 (Stdlib): 1-2 hours
  
- Phase 5 (Testing): 2-3 hours
  
- Phase 6 (Documentation): 1 hour

**Total: 10-15 hours of focused development**

---

## Current Status

- **Compiler builds:** ✓
- **Type system complete:** ✓
- **Ready for VM implementation:** ✓

**Next step:** Implement decimal arithmetic helpers in C (Phase 2.1)

---

## Notes for LLMs

When BigDecimal is complete, LLMs should:
1. **Use `decimal` for financial calculations** - no floating point errors
2. **Use `float` for scientific calculations** - faster, approximations OK
3. **Always use explicit type annotations** - `(set price decimal 19.99)`
4. **No automatic conversions** - use `cast_float_decimal` explicitly

**Example usage:**
```lisp
(fn calculate_total price decimal quantity int tax_rate decimal -> decimal
  (set subtotal decimal (mul price (cast_int_decimal quantity)))
  (set tax decimal (mul subtotal tax_rate))
  (set total decimal (add subtotal tax))
  (ret total))
```

This ensures exact precision for money calculations.

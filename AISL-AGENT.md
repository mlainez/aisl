# AISL-Agent Specification v1.0

**Status**: STABLE  
**Last Updated**: 2026-02-06

## Philosophy

AISL-Agent is the **ergonomic surface language** for writing AISL programs. It is:
- **High-level**: Structured control flow (while, loop, break, continue)
- **LLM-friendly**: Natural syntax that LLMs understand
- **Desugared**: All Agent constructs are handled by the interpreter directly
- **Extensible**: Can add new constructs without changing Core

AISL-Agent is what LLMs should write. The interpreter handles Agent constructs directly during evaluation.

## Agent Constructs

### 1. `while` - Conditional loop
```lisp
(while condition body)
```

Desugars to:
```lisp
(call label loop_start_N)
(set _cond_N bool condition)
(call ifnot _cond_N loop_end_N)
body
(call goto loop_start_N)
(call label loop_end_N)
```

Example:
```lisp
(fn count_to_ten () -> int
  (set n int 0)
  (while (call lt n 10)
    (set n int (call add n 1)))
  (ret n))
```

### 2. `loop` - Infinite loop
```lisp
(loop body)
```

Desugars to:
```lisp
(call label loop_start_N)
body
(call goto loop_start_N)
(call label loop_end_N)  ; For break statements
```

Example:
```lisp
(fn infinite_server () -> unit
  (loop
    (call handle_request)
    (call sleep 1000)))
```

### 3. `break` - Exit loop
```lisp
(break)
```

Desugars to:
```lisp
(call goto loop_end_N)  ; Where N is the enclosing loop
```

Must be inside a `while` or `loop`.

Example:
```lisp
(fn find_first_match ((arr Array) (target int)) -> int
  (set i int 0)
  (loop
    (set val int (call array_get arr i))
    (if (call eq val target)
      (break))
    (set i int (call add i 1)))
  (ret i))
```

### 4. `continue` - Skip to next iteration
```lisp
(continue)
```

Desugars to:
```lisp
(call goto loop_start_N)  ; Where N is the enclosing loop
```

Must be inside a `while` or `loop`.

Example:
```lisp
(fn sum_evens ((arr Array)) -> int
  (set sum int 0)
  (set i int 0)
  (while (call lt i (call array_len arr))
    (set val int (call array_get arr i))
    (if (call ne (call mod val 2) 0)
      (continue))
    (set sum int (call add sum val))
    (set i int (call add i 1)))
  (ret sum))
```

### 5. `if` - Conditional expression (FUTURE)
```lisp
(if condition then_expr else_expr)
```

Currently implemented via desugaring to ifnot+labels.
Future: Native expression form.

## Desugaring Process

The AISL interpreter handles Agent constructs directly during evaluation:

1. **Parse** - Parse Agent syntax into AST
2. **Evaluate** - Interpreter handles Agent constructs directly:
   - `while` → evaluated as conditional loop (re-evaluates condition each iteration)
   - `loop` → evaluated as infinite loop
   - `break` → raises Break exception caught by enclosing loop
   - `continue` → raises Continue exception caught by enclosing loop
   - `if` → evaluated as conditional branch

There is no separate desugaring or compilation step — the interpreter walks the AST directly.

### Interpreter Implementation

Location: `interpreter/interpreter.ml`

The interpreter evaluates Agent constructs in `eval_block` and `eval`:
- `while` expressions re-evaluate their condition each iteration
- `loop` expressions run forever until `break` is raised
- `break`/`continue` use OCaml exceptions to unwind to the enclosing loop
- `if` expressions evaluate the condition and execute the then-branch if true

### Loop Context Tracking

The interpreter maintains loop context via OCaml exception handling:
- `Break` exception — caught by the enclosing `while` or `loop`
- `Continue` exception — caught by the enclosing `while` or `loop`
- Nested loops work correctly because each loop handler catches exceptions from its own body only

## Nesting Rules

### Loops can nest
```lisp
(while outer_condition
  (while inner_condition
    (if something
      (break))  ; Breaks inner loop only
    ...))
```

### Break/Continue target nearest enclosing loop
```lisp
(while outer_condition
  (set x int 0)
  (while inner_condition
    (continue))  ; Continues inner loop
  (break))       ; Breaks outer loop
```

## LLM Usage Guidelines

### ✅ DO write Agent code
```lisp
(fn factorial ((n int)) -> int
  (set result int 1)
  (set i int 1)
  (while (call le i n)
    (set result int (call mul result i))
    (set i int (call add i 1)))
  (ret result))
```

### ❌ DON'T write Core IR directly
```lisp
; Don't do this - let interpreter handle it
(fn factorial ((n int)) -> int
  (set result int 1)
  (set i int 1)
  (call label loop_start_0)
  (set _cond_0 bool (call le i n))
  (call ifnot _cond_0 loop_end_0)
  ...)
```

## Polymorphic Operations

Agent code uses **short names** for operations:

```lisp
; Agent code (what LLMs write)
(call add x y)      ; Instead of add
(call lt a b)       ; Instead of lt
(call mul x 2)      ; Instead of mul
```

The interpreter resolves these to typed operations based on argument types.

See AISL-CORE.md for full list of polymorphic operations.

## Control Flow Legality

### Allowed
- ✅ Forward jumps (goto label ahead)
- ✅ Backward jumps (loops)
- ✅ Jumping out of nested blocks
- ✅ Jumping into nested blocks (via goto)
- ✅ Break from any loop depth
- ✅ Continue from any loop depth

### Forbidden
- ❌ Cross-function jumps
- ❌ Break outside any loop
- ❌ Continue outside any loop
- ❌ Duplicate labels in same function

## Examples

### Fibonacci
```lisp
(fn fibonacci ((n int)) -> int
  (if (call le n 1)
    (ret n))
  (set a int 0)
  (set b int 1)
  (set i int 2)
  (while (call le i n)
    (set temp int b)
    (set b int (call add a b))
    (set a int temp)
    (set i int (call add i 1)))
  (ret b))
```

### Find in array
```lisp
(fn find ((arr Array) (target int)) -> int
  (set i int 0)
  (while (call lt i (call array_len arr))
    (set val int (call array_get arr i))
    (if (call eq val target)
      (ret i))
    (set i int (call add i 1)))
  (ret -1))
```

### Break example
```lisp
(fn first_negative ((arr Array)) -> int
  (set i int 0)
  (loop
    (if (call ge i (call array_len arr))
      (break))
    (set val int (call array_get arr i))
    (if (call lt val 0)
      (ret val))
    (set i int (call add i 1)))
  (ret 0))
```

## Testing

Agent constructs should be tested via the test framework:

```lisp
(module test_while
  (fn count_to_n ((n int)) -> int
    (set i int 0)
    (while (call lt i n)
      (set i int (call add i 1)))
    (ret i))
  
  (test-spec count_to_n
    (case "count to 5"
      (input 5)
      (expect 5))
    (case "count to 10"
      (input 10)
      (expect 10))))
```

## Future Extensions

Potential Agent constructs to add:
- `for` loops with ranges
- `match` expressions (pattern matching)
- `defer` statements
- List comprehensions
- Error propagation (`?` operator)

All future extensions will desugar to Core - Core remains frozen.

## Implementation Status

| Construct | Status | Location |
|-----------|--------|----------|
| `while` | ✅ Implemented | interpreter.ml — eval/eval_block |
| `loop` | ✅ Implemented | interpreter.ml — eval/eval_block |
| `break` | ✅ Implemented | interpreter.ml — Break exception |
| `continue` | ✅ Implemented | interpreter.ml — Continue exception |
| `if/else` | ⚠️ Partial | if-then works; else not yet supported |

## Version History

- **v1.0** (2026-02-06): Initial specification
  - while, loop, break, continue
  - Desugaring to Core
  - Polymorphic operation dispatch

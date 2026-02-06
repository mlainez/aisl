# AISL Language Specification

AISL (AI-Optimized Systems Language) - A programming language designed for AI code generation with explicit syntax, zero ambiguity, and flat structure.

## Grammar

### Program Structure

```
program ::= (mod <name> <function>*)

function ::= (fn <name> (<param>*) -> <return_type> <statement>*)

param ::= (<name> <type>)

statement ::= (set <var> <type> <expr>)
            | (call <function> <arg>*)
            | (label <name>)
            | (goto <label>)
            | (ifnot <bool_var> <label>)
            | (while <bool_expr> <statement>*)
            | (loop <statement>*)
            | (break)
            | (continue)
            | (ret <expr>)
            | (ret <value>)

expr ::= <literal>
       | <variable>
       | (call <function> <arg>*)

literal ::= <number> | <string> | true | false
```

### Example

```scheme
(mod hello
  (fn main () -> i32
    (call print "Hello, World!")
    (ret 0)))
```

## Types

### Primitive Types
- **i32** - 32-bit signed integer
- **i64** - 64-bit signed integer
- **f32** - 32-bit floating point
- **f64** - 64-bit floating point
- **bool** - Boolean (true/false)
- **string** - UTF-8 string

### Type Annotations

Variables must be explicitly typed:

```scheme
(set count i32 42)
(set name string "Alice")
(set active bool true)
(set price f64 19.99)
```

## Control Flow

AISL supports both **structured loops** (`while` and `loop`) and **labels with goto** for control flow.

### Structured Loops (Recommended)

**While Loop**: `(while <condition> <statements>...)`

Executes the body statements repeatedly while the condition is true.

```scheme
(fn countdown ((n i32)) -> i32
  (while (call op_gt_i32 n 0)
    (call print_i32 n)
    (set n i32 (call op_sub_i32 n 1)))
  (ret 0))
```

**Infinite Loop**: `(loop <statements>...)`

Executes the body statements forever. Use this for server accept loops or event loops.

```scheme
(fn start_server ((port i32)) -> i32
  (set server_sock string (call tcp_listen port))
  (loop
    (set client_sock string (call tcp_accept server_sock))
    (call handle_connection client_sock))
  (ret 0))
```

### Loop Control

**Break**: `(break)`

Exits the nearest enclosing loop immediately.

```scheme
(fn find_value ((arr string) (target i32)) -> i32
  (set i i32 0)
  (loop
    (set val i32 (call array_get arr i))
    (set found bool (call op_eq_i32 val target))
    (ifnot found skip)
    (break)
    (label skip)
    (set i i32 (call op_add_i32 i 1)))
  (ret i))
```

**Continue**: `(continue)`

Skips to the next iteration of the nearest enclosing loop.

```scheme
(fn sum_positives ((arr string) (n i32)) -> i32
  (set i i32 0)
  (set sum i32 0)
  (while (call op_lt_i32 i n)
    (set val i32 (call array_get arr i))
    (set i i32 (call op_add_i32 i 1))
    (set is_negative bool (call op_lt_i32 val 0))
    (ifnot is_negative no_skip)
    (continue)
    (label no_skip)
    (set sum i32 (call op_add_i32 sum val)))
  (ret sum))
```

### Label-based Control Flow

For more complex control flow, AISL supports **labels and goto**. Use `ifnot` to conditionally jump.

```scheme
(fn countdown_with_labels ((n i32)) -> i32
  (label loop)
  (set done bool (call op_le_i32 n 0))
  (ifnot done continue)
  (ret 0)
  (label continue)
  (call print_i32 n)
  (set n i32 (call op_sub_i32 n 1))
  (goto loop))
```

**Statement**: `(ifnot <bool_var> <label>)` - Jump to label if variable is false

### Conditional Branching

Use `if_<type>` builtin functions for conditional values:

```scheme
(set result string (call if_string condition "true_val" "false_val"))
(set number i32 (call if_i32 condition 1 0))
```

### Recursion

Functions can call themselves for recursive algorithms:

```scheme
(fn factorial ((n i32)) -> i32
  (set is_base bool (call op_le_i32 n 1))
  (ifnot is_base recurse)
  (ret 1)
  (label recurse)
  (set n_minus_1 i32 (call op_sub_i32 n 1))
  (set result i32 (call factorial n_minus_1))
  (ret (call op_mul_i32 n result)))
```

**Note**: For simple iteration, prefer `while` or `loop` over recursion for better performance.

## Standard Library

AISL provides 180+ built-in functions. All functions use explicit `call` syntax.

### String Operations

```scheme
; String functions use 'string_' prefix
(call string_length text)              ; Get length
(call string_contains haystack needle) ; Check contains -> bool
(call string_concat a b)               ; Concatenate
(call string_split text delimiter)     ; Split into array
(call string_trim text)                ; Remove whitespace
(call string_replace text old new)     ; Replace substring
(call string_starts_with text prefix)  ; Check prefix
(call string_ends_with text suffix)    ; Check suffix
```

### Arithmetic Operations

```scheme
; Operations use 'op_<operation>_<type>' pattern
(call op_add_i32 a b)    ; a + b (i32)
(call op_sub_i32 a b)    ; a - b (i32)
(call op_mul_i32 a b)    ; a * b (i32)
(call op_div_i32 a b)    ; a / b (i32)
(call op_mod_i32 a b)    ; a % b (i32)

; Also available: op_add_i64, op_add_f32, op_add_f64, etc.
```

### Comparison Operations

```scheme
(call op_eq_i32 a b)     ; a == b -> bool
(call op_ne_i32 a b)     ; a != b -> bool
(call op_lt_i32 a b)     ; a < b  -> bool
(call op_gt_i32 a b)     ; a > b  -> bool
(call op_le_i32 a b)     ; a <= b -> bool
(call op_ge_i32 a b)     ; a >= b -> bool

; Also available for i64, f32, f64
```

### I/O Operations

```scheme
(call print text)              ; Print string
(call print_i32 number)        ; Print i32
(call print_i64 number)        ; Print i64
(call print_f32 number)        ; Print f32
(call print_f64 number)        ; Print f64
(call print_bool value)        ; Print bool
```

### File Operations

```scheme
(call file_read path)          ; Read file -> string
(call file_write path content) ; Write file
(call file_append path content); Append to file
(call file_exists path)        ; Check exists -> bool
(call file_delete path)        ; Delete file
(call file_size path)          ; Get size -> i64
```

### TCP Networking

```scheme
(call tcp_listen port)           ; Listen on port -> socket
(call tcp_accept server_socket)  ; Accept connection -> socket
(call tcp_connect host port)     ; Connect to server -> socket
(call tcp_send socket data)      ; Send data -> i32
(call tcp_receive socket bytes)  ; Receive data -> string
(call tcp_close socket)          ; Close socket
```

### HTTP Operations

```scheme
(call http_get url)              ; GET request -> response
(call http_post url body)        ; POST request -> response
(call http_put url body)         ; PUT request -> response
(call http_delete url)           ; DELETE request -> response
(call http_get_status response)  ; Get status code -> i32
(call http_get_body response)    ; Get body -> string
```

### JSON Operations

```scheme
(call json_parse text)           ; Parse JSON string
(call json_stringify obj)        ; Convert to JSON string
(call json_new_object)           ; Create empty object
(call json_new_array)            ; Create empty array
(call json_get obj key)          ; Get value by key
(call json_set obj key value)    ; Set key-value pair
(call json_push array value)     ; Add to array
(call json_length obj)           ; Get length/size
```

### Array Operations

```scheme
(call array_new size)            ; Create array
(call array_push array value)    ; Add element
(call array_get array index)     ; Get element
(call array_set array index val) ; Set element
(call array_length array)        ; Get length -> i32
```

### Map Operations

```scheme
(call map_new)                   ; Create empty map
(call map_set map key value)     ; Set key-value
(call map_get map key)           ; Get value by key
(call map_has map key)           ; Check key exists -> bool
(call map_delete map key)        ; Remove key
(call map_length map)            ; Get size -> i32
```

### Process Operations

```scheme
(call process_spawn cmd args)    ; Start process
(call process_wait proc)         ; Wait for completion
(call process_kill proc signal)  ; Kill process
(call process_read proc)         ; Read stdout
(call process_write proc data)   ; Write to stdin
```

### Regular Expressions

```scheme
(call regex_compile pattern)     ; Compile regex
(call regex_match regex text)    ; Test match -> bool
(call regex_find regex text)     ; Find match -> string
(call regex_find_all regex text) ; Find all -> array
(call regex_replace regex text r); Replace matches
```

### Time Operations

```scheme
(call time_now)                  ; Current timestamp -> i64
(call time_format timestamp fmt) ; Format time -> string
(call time_parse text fmt)       ; Parse time -> i64
```

### Cryptography

```scheme
(call md5 text)                  ; MD5 hash -> string
(call sha256 text)               ; SHA256 hash -> string
(call hmac_sha256 key message)   ; HMAC-SHA256 -> string
(call base64_encode data)        ; Encode base64 -> string
(call base64_decode text)        ; Decode base64 -> string
```

### Type Conversions

```scheme
(call cast_i32_i64 value)        ; i32 -> i64
(call cast_i64_i32 value)        ; i64 -> i32
(call cast_i32_f32 value)        ; i32 -> f32
(call cast_i32_f64 value)        ; i32 -> f64
(call cast_f32_i32 value)        ; f32 -> i32
(call cast_f64_i32 value)        ; f64 -> i32
; And all other numeric conversions
```

### Math Functions

```scheme
(call math_abs_i32 value)        ; Absolute value
(call math_min_i32 a b)          ; Minimum
(call math_max_i32 a b)          ; Maximum
(call math_sqrt value)           ; Square root
(call math_pow base exp)         ; Power
; Also available for i64, f32, f64
```

### Conditional Functions

```scheme
(call if_i32 condition then else)    ; Conditional i32
(call if_i64 condition then else)    ; Conditional i64
(call if_f32 condition then else)    ; Conditional f32
(call if_f64 condition then else)    ; Conditional f64
(call if_string condition then else) ; Conditional string
```

### Garbage Collection

```scheme
(call gc_collect)                ; Force GC collection
(call gc_stats)                  ; Get GC statistics
```

## Complete Example: Web Server

```scheme
(mod sinatra
  (fn handle_connection ((client_sock string)) -> i32
    (set request string (call tcp_receive client_sock 4096))
    (set has_get_hello_json bool (call string_contains request "GET /hello.json "))
    (set has_get_hello bool (call string_contains request "GET /hello "))
    (set json_resp string "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 30\r\n\r\n{\"message\": \"Hello from AISL\"}")
    (set html_resp string "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 41\r\n\r\n<html><body>Hello from AISL</body></html>")
    (set notfound_resp string "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found")
    (set response string notfound_resp)
    (set response string (call if_string has_get_hello html_resp response))
    (set response string (call if_string has_get_hello_json json_resp response))
    (call tcp_send client_sock response)
    (call tcp_close client_sock)
    (ret 0))

  (fn start_server ((port i32)) -> i32
    (call print "Server running on http://localhost:8080")
    (set server_sock string (call tcp_listen port))
    (loop
      (set client_sock string (call tcp_accept server_sock))
      (call handle_connection client_sock))
    (ret 0))

  (fn main () -> i32
    (call start_server 8080)
    (ret 0)))
```

This server:
- Listens on port 8080
- Routes `GET /hello` to HTML response (200 OK)
- Routes `GET /hello.json` to JSON response (200 OK)
- Returns 404 Not Found for all other paths
- Uses `loop` construct for the accept loop

## Key Design Principles

1. **Explicit Types** - Every variable has a declared type
2. **Flat Structure** - No complex nested expressions
3. **Structured Control** - Use `while`/`loop` for iteration; labels/goto for complex flow
4. **Function Calls** - All operations use explicit `call` syntax
5. **No Operator Precedence** - No infix operators, everything is a function call
6. **Deterministic** - Same input always produces same output
7. **S-Expression Syntax** - Lisp-style parenthesized syntax

## Compilation and Execution

```bash
# Compile AISL source to bytecode
./compiler/c/bin/aislc program.aisl program.aislc

# Run bytecode
./compiler/c/bin/aisl-run program.aislc
```

## File Extension

AISL source files use the `.aisl` extension.  
Compiled bytecode files use the `.aislc` extension.

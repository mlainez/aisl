# AISL Examples

This directory contains practical examples demonstrating AISL's capabilities.

## Available Examples

### Basic Examples

- **hello_world.aisl** - The classic "Hello, World!" program
- **simple_typed.aisl** - Simple typed variable demonstration
- **multi_print.aisl** - Multiple print statements
- **typed_add.aisl** - Basic addition with type annotations
- **typed_math.aisl** - Mathematical operations with types

### Web Server Examples

- **sinatra.aisl** - A Sinatra-style web server with routing
  - Listens on port 8080
  - Routes:
    - `GET /hello` → Returns HTML response (200 OK)
    - `GET /hello.json` → Returns JSON response (200 OK)
    - Any other path → Returns 404 Not Found
  - Demonstrates:
    - TCP socket handling
    - HTTP request parsing
    - Exact path matching with `string_contains`
    - Conditional routing with `if_string`
    - Multiple response types (HTML, JSON, 404)
    - Recursive accept loop pattern

- **echo_server.aisl** - Simple TCP echo server
- **simple_server.aisl** - Basic TCP server template
- **simple_json_server.aisl** - JSON-based HTTP server
- **sequential_server.aisl** - Sequential request handling server
- **working_server.aisl** - Working HTTP server example
- **final_server.aisl** - Complete server implementation
- **test_server.aisl** - Server with test/debug output
- **sinatra_simple.aisl** - Simplified Sinatra-style server
- **sinatra_demo.aisl** - Sinatra server demonstration
- **sinatra_debug.aisl** - Sinatra server with debug output
- **sinatra_recursive.aisl** - Recursive request handling pattern

## Running Examples

To compile and run an example:

```bash
# Compile
./compiler/c/bin/aislc examples/hello_world.aisl hello_world.aislc

# Run
./compiler/c/bin/aisl-run hello_world.aislc
```

For the web server example:

```bash
# Compile
./compiler/c/bin/aislc examples/sinatra.aisl sinatra.aislc

# Run in background
./compiler/c/bin/aisl-run sinatra.aislc &

# Test with curl
curl http://localhost:8080/hello        # Returns HTML
curl http://localhost:8080/hello.json   # Returns JSON
curl http://localhost:8080/             # Returns 404
curl http://localhost:8080/other        # Returns 404

# Stop server
pkill aisl-run
```

## Contributing Examples

When adding new examples:
1. Use descriptive names
2. Include comments explaining key concepts
3. Keep examples focused on a single feature or use case
4. Add entry to this README

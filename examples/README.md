# AISL Examples

Practical examples demonstrating AISL capabilities.

## Available Examples

### Basic Examples

- **hello_world.aisl** - Classic "Hello, World!" program
- **simple_typed.aisl** - Typed variable demonstration
- **multi_print.aisl** - Multiple print statements
- **typed_add.aisl** - Addition with type annotations
- **typed_math.aisl** - Mathematical operations

### Web Applications

- **todo_app/** - Full-featured TODO app with SQLite database
  - Complete CRUD operations
  - Database persistence
  - Modern web interface
  - See `todo_app/README.md` for details

### Web Servers

- **sinatra.aisl** - Production HTTP server with routing
  - Multiple routes (GET /hello, /hello.json, 404 handling)
  - TCP socket handling
  - HTTP request parsing
  - Recommended starting point for web servers

- **echo_server.aisl** - Simple TCP echo server
- **simple_server.aisl** - Basic TCP server template

## Running Examples

Compile and run any example:

```bash
# Compile
./compiler/c/bin/aislc examples/hello_world.aisl /tmp/hello.aislc

# Run
./compiler/c/bin/aisl-run /tmp/hello.aislc
```

For web servers:

```bash
# Compile and run
./compiler/c/bin/aislc examples/sinatra.aisl /tmp/server.aislc
./compiler/c/bin/aisl-run /tmp/server.aislc &

# Test
curl http://localhost:8080/hello
curl http://localhost:8080/hello.json

# Stop
pkill aisl-run
```

## Contributing Examples

When adding examples:
1. Use descriptive names
2. Keep examples focused on one feature
3. Add entry to this README
4. Ensure example compiles and runs

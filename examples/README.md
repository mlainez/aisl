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

Run any example directly:

```bash
./interpreter/_build/default/vm.exe examples/hello_world.aisl
```

For web servers:

```bash
# Run server
./interpreter/_build/default/vm.exe examples/sinatra.aisl &

# Test
curl http://localhost:8080/hello
curl http://localhost:8080/hello.json

# Stop
kill %1
```

## Contributing Examples

When adding examples:
1. Use descriptive names
2. Keep examples focused on one feature
3. Add entry to this README
4. Ensure example compiles and runs

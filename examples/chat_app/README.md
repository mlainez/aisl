# Chat App - AISL TCP Example

Simple echo server using TCP sockets in AISL.

## Files

- `chat_server.aisl` - TCP server that echoes messages back to clients
- `chat_client.aisl` - TCP client that sends stdin to server

## Usage (with netcat for true chat)

```bash
# Terminal 1: Start server
./compiler/c/bin/aisl-run examples/chat_app/chat_server.aislc

# Terminal 2: Connect with netcat
nc localhost 8080
# Type messages - they'll be echoed back

# Terminal 3: Another client
nc localhost 8080
# Type messages - they'll be echoed back
```

## Using AISL client

```bash
# Terminal 1: Start server
./compiler/c/bin/aisl-run examples/chat_app/chat_server.aislc

# Terminal 2: AISL client
echo "hello" | ./compiler/c/bin/aisl-run examples/chat_app/chat_client.aislc
```

Note: AISL currently handles one client at a time sequentially. For concurrent clients, use netcat.

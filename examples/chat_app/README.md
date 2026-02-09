# Chat App - AISL TCP Example

Simple chat application using TCP sockets in AISL.

## Files

- `chat_server.aisl` - TCP server that echoes messages back to clients
- `chat_client.aisl` - TCP client for chatting

## Usage

```bash
# Terminal 1: Start server
./compiler/c/bin/aisl-run examples/chat_app/chat_server.aislc

# Terminal 2: Start client
./compiler/c/bin/aisl-run examples/chat_app/chat_client.aislc

# Terminal 3: Start another client
./compiler/c/bin/aisl-run examples/chat_app/chat_client.aislc
```

Type messages in any client terminal - the server echoes them back.

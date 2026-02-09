# Chat App - AISL TCP Example

TCP echo server using non-blocking sockets with event loop.

## Files

- `echo_server.aisl` - Event loop server using socket_select for multiple clients
- `chat_client.aisl` - Interactive client

## Usage

```bash
# Terminal 1: Start echo server
./compiler/c/bin/aisl-run examples/chat_app/echo_server.aislc

# Terminal 2: Connect with netcat
nc localhost 8080

# Terminal 3: Another client
nc localhost 8080
```

Type messages in any client - they'll be echoed back by the server.

## API

Non-blocking socket operations:

| Operation | Description |
|-----------|-------------|
| `(tcp_receive sock 1024)` | Receive data, returns `""` if no data available |
| `(socket_select clients)` | Check which sockets have data, returns array of indices |
| `(thread_yield)` | Yield to scheduler (for cooperative multitasking) |

## Event Loop Pattern

```lisp
(loop
  (set ready array (socket_select clients))
  (set len int (array_length ready))
  (set i int 0)
  (while (lt i len)
    ;; Handle each ready socket
    (set i int (add i 1))))
```

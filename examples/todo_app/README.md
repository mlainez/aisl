# AISL Todo Application

Complete web-based TODO application with SQLite database backend, built in pure AISL.

## Files

- `todo_app.aisl` - AISL backend server (161 lines)
- `index.html` - HTML/CSS/JavaScript frontend template
- `todos.db` - SQLite database (created at runtime)

## Quick Start

```bash
# Compile
./compiler/c/bin/aislc examples/todo_app/todo_app.aisl /tmp/todo.aislc

# Run
./compiler/c/bin/aisl-run /tmp/todo.aislc

# Open browser
http://localhost:8080
```

## Features

- Full HTML/CSS/JavaScript UI with Tailwind CSS
- SQLite database backend
- Add, toggle, and delete todos
- Data persists across server restarts
- Modern, responsive interface
- Starts with empty database - add your own tasks!
- Clean separation: HTML template in `index.html`, logic in AISL
- Compact AISL backend (~120 lines)

## Implementation

The application consists of:
- SQLite database with `todos` table
- TCP server on port 8080
- External HTML template (`index.html`) loaded at runtime
- Dynamic data injection using template replacement
- RESTful API endpoints for CRUD operations

### Template System

The HTML template uses a placeholder `__TODOS_DATA__` which is replaced with the actual todo JSON data at runtime:

```javascript
// In index.html
let t=__TODOS_DATA__;

// At runtime, AISL replaces this with:
let t=[{"id":1,"text":"Buy milk","done":false}];
```

This allows easy customization of the UI without modifying AISL code.

## API Endpoints

- `GET /` - Returns the web interface
- `POST /api/add?text=...` - Add new todo
- `POST /api/toggle?id=...` - Toggle todo status
- `POST /api/delete?id=...` - Delete todo

## Database Schema

```sql
CREATE TABLE todos (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  text TEXT NOT NULL,
  done INTEGER DEFAULT 0
);
```

## Technical Stack

- Language: AISL
- Database: SQLite (via process handle)
- Server: TCP sockets (via socket handle)
- Frontend: Vanilla JavaScript

## Example Usage

```lisp
(module todo_app
  (import sqlite)
  
  (fn main -> int
    (set db process (call open "todos.db"))
    (call init_db db)
    
    (set server socket (call tcp_listen 8080))
    (loop
      (set client socket (call tcp_accept server))
      (call handle_request db client))
    (ret 0)))
```

This demonstrates AISL's ability to build production-ready web applications with database persistence.

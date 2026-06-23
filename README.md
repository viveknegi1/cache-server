# In-Memory Cache Server

A high-performance, Redis-like, in-memory key-value cache server written in C++17. The server accepts concurrent TCP connections, uses a custom space-delimited text protocol, supports key expiration (TTL) via a background clean-up thread, and provides reliable data persistence via binary serialization/deserialization.

---

## Features

- **In-Memory Store:** High-performance, thread-safe, average $O(1)$ operations utilizing `std::unordered_map` and synchronized using mutual exclusion primitives.
- **Concurrency & Thread Pooling:** Utilizes a pre-allocated Pool of worker threads to handle multiple client connections simultaneously without blocking the main event loop.
- **Custom Protocol:** Light-weight, line-delimited TCP ASCII protocol, allowing communication via simple CLI clients (e.g., netcat or telnet).
- **Time-To-Live (TTL) Key Expiry:** Dedicated background thread that periodically scans the cache and purges expired entries using non-blocking, condition-variable-synchronized wait loops.
- **Persistence to Disk:** Clean shutdown saves active cache states to a compact binary format (`cache.dat`). The database is auto-restored on server startup. An on-demand `SAVE` command is also supported.
- **Robust Logger:** Thread-safe, multi-sink logger printing output to both `std::cout` and appending to `server.log`.
- **INI Configuration:** Parameterized behavior configuration (port, thread count, TTL sweep interval) defined via a simple config file.

---

## Directory Structure

```text
cache-server/
├── CMakeLists.txt         # CMake build configuration
├── config.ini             # Server runtime parameters configuration
├── PROJECT_BRIEF_V2.md    # Development log, specifications, and learning milestones
├── README.md              # Project documentation (this file)
├── include/               # Header files
│   ├── cache_store.h      # Core CacheStore class definition
│   ├── client_handler.h   # Client handler thread workload wrapper
│   ├── command_parser.h   # Request parser
│   ├── config.h           # Configuration singleton manager
│   ├── logger.h           # Thread-safe logging class
│   ├── persistence.h      # Binary save/load serialization
│   ├── response_builder.h # ASCII response formatter
│   ├── tcp_listener.h     # Socket wrapper for listening/accepting
│   ├── thread_pool.h      # Thread Pool executor
│   └── ttl_manager.h      # Background key cleanup manager
├── src/                   # Implementation files
│   ├── main.cpp           # Entry point and signal handling
│   ├── cache_store.cpp
│   ├── client_handler.cpp
│   ├── command_parser.cpp
│   ├── config.cpp
│   ├── logger.cpp
│   ├── persistence.cpp
│   ├── response_builder.cpp
│   ├── tcp_listener.cpp
│   ├── thread_pool.cpp
│   └── ttl_manager.cpp
└── tests/                 # Unit testing files
    ├── test_cache_store.cpp     # Unit tests for the cache store
    └── test_command_parser.cpp  # Unit tests for request parsing
```

---

## Command Reference

The server processes incoming space-delimited text commands terminated by newlines (`\n` or `\r\n`).

| Command | Syntax | Description | Example Request | Success Response | Error / Miss Response |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **SET** | `SET <key> <value> [<ttl_seconds>]` | Sets key to value. Optional TTL in seconds. | `SET name John 60` | `OK\n` | `ERROR Arguments are fewer than expected\n` |
| **GET** | `GET <key>` | Retrieves the value of a key. | `GET name` | `OK John\n` | `NOT_FOUND\n` |
| **DEL** | `DEL <key>` | Deletes the specified key. | `DEL name` | `OK\n` | `ERROR Key wasn't deleted\n` |
| **EXISTS**| `EXISTS <key>` | Checks if key exists. | `EXISTS name` | `1\n` | `0\n` |
| **KEYS** | `KEYS` | Returns all active keys. | `KEYS` | `<key1>\n<key2>\n...\nEND\n` | `END\n` (if store is empty) |
| **FLUSH** | `FLUSH` | Clears all data in the store. | `FLUSH` | `OK\n` | *N/A* |
| **SAVE**  | `SAVE` | Triggers a manual binary save to disk. | `SAVE` | `OK\n` | *N/A* |

---

## Configuration (`config.ini`)

The server configures itself on startup using `config.ini`. If options are not specified, C++ initialization defaults are used.

```ini
port=8080
thread_count=4
root_directory=./public
sweep_interval=5
```

- **`port`**: The port number on which the server listens for incoming TCP connections.
- **`thread_count`**: The size of the Thread Pool to handle concurrent socket actions.
- **`root_directory`**: Folder path for runtime assets (reserved for future use).
- **`sweep_interval`**: Frequency in seconds at which the background `TtlManager` cleaner runs.

---

## Persistence Binary Format

Data is serialized to `cache.dat` in a tight binary format. Reading and writing utilize byte streams to avoid structure padding discrepancies.

For each active key-value pair, the following structure is written:

```
+-------------------+-----------------+-------------------+-------------------+-----------------+-----------------------+
|  Key Length (B)   |   Key Data      |  Value Length (B) |    Value Data     |  Has Expiry (B) |  Expiry Timestamp (B) |
|  (4-byte uint32)  |  (Variable len) |  (4-byte uint32)  |   (Variable len)  |   (1-byte bool) |  (8-byte int64 count) |
+-------------------+-----------------+-------------------+-------------------+-----------------+-----------------------+
```

1. **Key Length** (`uint32_t`): size of key.
2. **Key Data** (`char[]`): characters of the key string.
3. **Value Length** (`uint32_t`): size of value.
4. **Value Data** (`char[]`): characters of the value string.
5. **Has Expiry** (`bool`): `true` if TTL is set, `false` otherwise.
6. **Expiry Time** (`int64_t`): Expiration epoch in seconds since Unix epoch.

---

## Compilation and Build

### Prerequisites

- A compiler supporting C++17 (e.g., `gcc 9+`, `clang 10+`)
- **CMake** (version 3.15 or higher)
- **Catch2** (version 3, required for unit testing)

### Build Instructions

From the root directory, generate the build files and compile the target binaries using CMake:

```bash
# Generate build configuration and compile
cmake -B build -S .
cmake --build build
```

This compiles two executables:
1. `build/cache_server`: The main server executable.
2. `build/cache_tests`: The unit test suite using Catch2.

---

## Running the Server and Client

### 1. Launching the Server

Run the executable in the project root to ensure it finds `config.ini` in its working directory:

```bash
./build/cache_server
```

You should see logs indicating the server is running:
```text
[ Tue Jun 23 15:30:00 2026 ] [ INFO ] Server listening on port 8080
```

To stop the server cleanly and trigger disk serialization, press `Ctrl+C` (`SIGINT`). The server catches this signal, completes pending tasks, saves the active state to `cache.dat`, and exits gracefully.

### 2. Accessing via CLI Client

You can interact with the server using standard utilities like `nc` (netcat) or `telnet`:

```bash
nc localhost 8080
```

Once connected, type commands directly:
```text
SET name Alice 10
OK
GET name
OK Alice
... (wait 10 seconds) ...
GET name
NOT_FOUND
```

---

## Running Tests

Execute the unit tests binary to run the test suite:

```bash
./build/cache_tests
```

All 27 assertions verifying command parsing edge cases and core cache behavior should pass.

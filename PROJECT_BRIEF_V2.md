# In-Memory Cache Server — Project Brief
> Paste this at the start of every new Claude session to restore full context.

---

## 👤 About the Developer
- **C++ experience:** Completed a multi-threaded HTTP/1.1 server from scratch.
- **Known concepts:** Classes, OOP, RAII, smart pointers, sockets, threads, mutexes, condition variables, singletons, CMake, POSIX APIs.
- **Daily time:** 1–2 hours
- **Goal:** 5-year level C++ engineering. Deep systems knowledge.
- **OS:** Mac (Apple Silicon, clang 17)

---

## 🎯 Project Goal
Build a **Redis-like in-memory key-value cache server** in C++17.

It will accept TCP connections, speak a custom text protocol, store
key-value data in memory with optional TTL (time-to-live expiry),
support concurrent clients, and persist data to disk on shutdown.

By the end it will handle commands like:
```
SET name John 60       → store "name"="John", expires in 60 seconds
GET name               → returns "John"
DEL name               → deletes the key
EXISTS name            → returns 1 or 0
KEYS                   → returns all keys
FLUSH                  → clears all data
SAVE                   → persists data to disk
```

---

## 🆕 New C++ Concepts This Project Introduces
| Concept | Why |
|---------|-----|
| Templates | Generic data structures |
| Move semantics (`std::move`) | Efficient data passing |
| `std::chrono` | TTL expiry timing |
| Lock-free structures | High performance concurrency |
| `std::variant` | Store multiple value types |
| Custom memory allocator (basic) | Understand memory management |
| Binary serialization | Persist data to disk |
| Unit testing with Catch2 | Production code quality |

---

## 📋 Conventions (same as before — non-negotiable)
| What | Rule |
|------|------|
| Classes | `PascalCase` → `CacheStore`, `CommandParser` |
| Functions / methods | `camelCase` → `getValue()`, `parseCommand()` |
| Member variables | `m_` prefix → `m_store`, `m_ttl` |
| Constants | `ALL_CAPS` → `MAX_KEYS` |
| Files | `snake_case` → `cache_store.cpp` |
| Headers | Always `#pragma once` |
| Memory | No raw `new`/`delete`. Smart pointers only. |
| Namespace | Never `using namespace std;` |
| Function length | Max ~40 lines. One function = one job. |
| Compile flags | Always `-Wall -Wextra -std=c++17` |

---

## 🗂️ Target Project Structure
```
cache-server/
├── CMakeLists.txt
├── PROJECT_BRIEF.md
├── README.md
├── config.ini
├── src/
│   ├── main.cpp
│   ├── tcp_listener.cpp      ← reuse from previous project
│   ├── thread_pool.cpp       ← reuse from previous project
│   ├── command_parser.cpp    ← parses incoming commands
│   ├── cache_store.cpp       ← core key-value store
│   ├── ttl_manager.cpp       ← handles key expiry
│   ├── persistence.cpp       ← save/load to disk
│   ├── response_builder.cpp  ← builds responses
│   └── logger.cpp            ← reuse from previous project
└── include/
    ├── tcp_listener.h
    ├── thread_pool.h
    ├── command_parser.h
    ├── cache_store.h
    ├── ttl_manager.h
    ├── persistence.h
    ├── response_builder.h
    └── logger.h
```

---

## 🗺️ Phases Overview
| Phase | Module | New C++ concepts |
|-------|--------|-----------------|
| 0 | Project setup + reuse TCP/ThreadPool/Logger | CMake with multiple reused modules |
| 1 | Command parser | `std::variant`, `std::vector`, string parsing |
| 2 | Cache store (SET/GET/DEL) | Templates, `std::unordered_map`, move semantics |
| 3 | TTL expiry | `std::chrono`, background expiry thread |
| 4 | Persistence (SAVE/LOAD) | Binary file I/O, serialization |
| 5 | EXISTS/KEYS/FLUSH commands | Iterators, range-based algorithms |
| 6 | Unit tests | Catch2 testing framework |
| 7 | Polish | Benchmarking, edge cases, README |

---

## ✅ Current Status

**Current Phase:** 3 
**Status:** completed

### Prerequisites before starting
- [ ] Push HTTP server project to GitHub
- [ ] Read your HTTP server code top to bottom once
- [ ] Create `cache-server/` folder

---

## 📝 Design Decisions Log

| Decision | Reason |
|----------|--------|
| Custom protocol over HTTP | Simpler, teaches protocol design, closer to real Redis |
| `std::unordered_map` for store | O(1) average lookup |
| Background thread for TTL | Non-blocking expiry |
| Reuse TCP/ThreadPool/Logger | Build on what you know |

---

## ⚠️ Mistakes / Rethinks Log
*(filled in as we go)*

---

## 🔖 C++ Concepts — Status
| Concept | Introduced in Phase | Understood? |
|---------|--------------------:|-------------|
| Templates | 2 | — |
| Move semantics | 2 | X |
| `std::chrono` | 3 | X |
| `std::variant` | 1 | — |
| Binary serialization | 4 | — |
| Unit testing | 6 | — |

---

## 📌 How to Use This File
1. **Save it** in your `cache-server/` folder as `PROJECT_BRIEF.md`
2. **Update it daily** — tick off completed tasks, add to logs
3. **Paste it at the top** of every new Claude session
4. Start with: *"Here is my project brief. I want to continue."*

---

## 💼 Why This Project for 5-Year Level

A Redis-like server is asked about in **senior engineering interviews** at
companies like Cloudflare, Stripe, Jane Street, and systems-focused startups.
It demonstrates:
- Protocol design from scratch
- Concurrent data structure management
- Memory-conscious programming
- Production reliability (TTL, persistence, error handling)
- Test-driven thinking

Combined with your HTTP server, you'll have two strong portfolio pieces
that cover the full range of systems programming in C++.

---
*Last updated: Day 3 — Phase 3 completed*

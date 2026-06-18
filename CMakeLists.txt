cmake_minimum_required(VERSION 3.15)
project(cache-server VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  #

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(cache_server 
    src/main.cpp
    src/tcp_listener.cpp
    src/thread_pool.cpp
    src/logger.cpp
    src/config.cpp
    src/command_parser.cpp
    src/cache_store.cpp
)
target_include_directories(cache_server PRIVATE include)


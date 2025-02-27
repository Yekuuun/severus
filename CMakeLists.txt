cmake_minimum_required(VERSION 3.10)
project(severus C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(severus ${SOURCES})

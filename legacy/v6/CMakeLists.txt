cmake_minimum_required(VERSION 3.10)
project(mathlib C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -mavx -Wall -Wextra -fgnu89-inline")

# Build the static library
add_library(mathc STATIC mathlib.c)
target_include_directories(mathc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Build the test executable and link the library
add_executable(test test.c)
target_link_libraries(test mathc m)

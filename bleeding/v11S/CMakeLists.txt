cmake_minimum_required(VERSION 3.10)
project(mathlib VERSION 11.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF) # Strict C99. No GNU extensions (typeof, etc.)

# Profile Validation
set(MATHLIB_PROFILE "SCIENTIFIC" CACHE STRING "Mathlib Execution Profile")
if(NOT MATHLIB_PROFILE MATCHES "^(SCIENTIFIC|GRAPHICS|EMBEDDED)$")
    message(FATAL_ERROR "MATHLIB_PROFILE must be SCIENTIFIC, GRAPHICS, or EMBEDDED. Got: ${MATHLIB_PROFILE}")
endif()
add_compile_definitions(MATHLIB_PROFILE_${MATHLIB_PROFILE})

# Strict Warning Flags
add_compile_options(
    -O3 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wfloat-equal
    -fno-fast-math -ffp-contract=off
)

option(MATHLIB_NATIVE "Enable -march=native (Breaks binary portability)" OFF)
if(MATHLIB_NATIVE)
    add_compile_options(-march=native)
endif()

option(MATHLIB_HARDENED "Enable memory canaries and strict sanitization" OFF)
if(MATHLIB_HARDENED)
    add_compile_definitions(MATHLIB_HARDENED)
endif()

# Core Library (INTERFACE for now; will become STATIC when .c files are extracted)
add_library(mathc INTERFACE)
target_include_directories(mathc INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mathlib>
    $<INSTALL_INTERFACE:include/mathlib>
)

# Sanitizers (Fixed: added to link options)
option(MATHLIB_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)
if(MATHLIB_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# Helper function for tests
function(add_mathlib_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE mathc m)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

enable_testing()

# Main Tests
add_mathlib_test(test_main tests/test.c)
add_mathlib_test(test_core tests/test_core.c)
add_mathlib_test(test_trig tests/test_trig.c)
add_mathlib_test(test_linalg tests/test_linalg.c)
add_mathlib_test(test_dsp tests/test_dsp.c)
add_mathlib_test(fuzz_god_mode tests/fuzz_god_mode.c)
add_mathlib_test(fuzz_boundary tests/fuzz_boundary_gauntlet.c)

# Validation & Examples (Opt-in to keep default build clean)
option(MATHLIB_BUILD_EXTRAS "Build validation tools and examples" OFF)
if(MATHLIB_BUILD_EXTRAS)
    add_mathlib_test(ulp_validation validation/ulp_error.c)
    add_mathlib_test(hello_tensor examples/hello_tensor.c)
endif()

# Installation
include(GNUInstallDirs)
install(DIRECTORY include/mathlib DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

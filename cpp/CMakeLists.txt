cmake_minimum_required(VERSION 3.10)
project(compecs-Cpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp$")

add_library(compecs_lib ${SOURCES})

add_executable(compecs src/main.cpp)
target_link_libraries(compecs PRIVATE compecs_lib)

find_package(Threads REQUIRED)
find_package(TBB REQUIRED)

target_link_libraries(compecs_lib PRIVATE Threads::Threads TBB::tbb)

target_compile_options(compecs_lib PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(compecs PRIVATE -Wall -Wextra -Wpedantic)

# SIMD Selection for testing
set(SIMD_MODE "DEFAULT" CACHE STRING "SIMD mode: NONE, AVX128, AVX256, AVX512, DEFAULT")
set_property(CACHE SIMD_MODE PROPERTY STRINGS NONE AVX128 AVX256 AVX512 DEFAULT)

function(apply_simd_mode target mode)

    if(mode STREQUAL "AVX512")
        target_compile_options(${target} PRIVATE -mavx512f -mavx512dq)
        target_compile_definitions(${target} PRIVATE SIMD_AVX512)

    elseif(mode STREQUAL "AVX256")
        target_compile_options(${target} PRIVATE -mavx2)
        target_compile_definitions(${target} PRIVATE SIMD_AVX256)

    elseif(mode STREQUAL "AVX128")
        target_compile_options(${target} PRIVATE -mavx -msse4.2)
        target_compile_definitions(${target} PRIVATE SIMD_AVX128)

    elseif(mode STREQUAL "NONE")
        target_compile_definitions(${target} PRIVATE SIMD_NONE)

    elseif(mode STREQUAL "DEFAULT")
        # no extra flags
    else()
        message(FATAL_ERROR "Unknown SIMD_MODE: ${mode}")
    endif()

endfunction()

apply_simd_mode(compecs_lib ${SIMD_MODE})

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE compecs_lib GTest::gtest_main)

gtest_discover_tests(unit_tests)
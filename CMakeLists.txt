cmake_minimum_required(VERSION 3.10)
project(NTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Framework header-only target
add_library(ntest INTERFACE)
target_include_directories(ntest INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    vendor/NCLI/include
    )

# Example usage
add_executable(example examples/basic_tests.cpp)
target_link_libraries(example PRIVATE ntest)

# Self-test executable
add_executable(ntest_selftest tests/self_tests.cpp)
target_link_libraries(ntest_selftest PRIVATE ntest)

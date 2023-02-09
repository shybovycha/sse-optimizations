cmake_minimum_required(VERSION 3.23)

project(sse-optimizations)

add_executable(sse-basics src/basics.cpp src/benchmark.hpp)
add_executable(sse-max src/max.cpp src/generators.hpp src/benchmark.hpp)
add_executable(sse-sum src/sum.cpp src/generators.hpp src/benchmark.hpp)

set_property(TARGET sse-basics PROPERTY CXX_STANDARD 20)
set_property(TARGET sse-max PROPERTY CXX_STANDARD 20)
set_property(TARGET sse-sum PROPERTY CXX_STANDARD 20)

if(MSVC)
    target_compile_options(sse-basics INTERFACE /arch:AVX512)
    target_compile_options(sse-max INTERFACE /arch:AVX512)
    target_compile_options(sse-sum INTERFACE /arch:AVX512)
else()
    target_compile_options(sse-basics INTERFACE -msse4.2)
    target_compile_options(sse-max INTERFACE -msse4.2)
    target_compile_options(sse-sum INTERFACE -msse4.2)
endif()

cmake_minimum_required(VERSION 3.16)

project(sse-optimizations)

add_executable(sse-basics src/basics.cpp)
add_executable(sse-max src/max.cpp src/generators.hpp)
add_executable(sse-sum src/sum.cpp src/generators.hpp)

if(MSVC)
    target_compile_options(sse-basics INTERFACE /arch:SSE4.1)
    target_compile_options(sse-max INTERFACE /arch:SSE4.1)
    target_compile_options(sse-sum INTERFACE /arch:SSE4.1)
else()
    target_compile_options(sse-basics INTERFACE -msse4.1)
    target_compile_options(sse-max INTERFACE -msse4.1)
    target_compile_options(sse-sum INTERFACE -msse4.1)
endif()

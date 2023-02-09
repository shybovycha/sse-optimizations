cmake_minimum_required(VERSION 3.23)

project(sse-optimizations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(common)

add_subdirectory(1-basics)
add_subdirectory(2-sum)
add_subdirectory(3-max)

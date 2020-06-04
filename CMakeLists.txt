project(IR)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
include_directories(src)

add_subdirectory(src/common)
add_subdirectory(src/build_index)
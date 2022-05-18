cmake_minimum_required(VERSION 3.4.1 FATAL_ERROR)
project(random-permutation)

# set C++ build flags
set(CXX_STANDARD c++20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++20 ${GCC_WARNINGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# subdirectories
add_subdirectory(extlib)
add_subdirectory(src)

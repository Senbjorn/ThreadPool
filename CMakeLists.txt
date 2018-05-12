cmake_minimum_required (VERSION 3.0)
project (ThreadPool)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
set (CMAKE_BINARY_DIR bin)
set (CMAKE_SOURCE_DIR src)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(threadpool ${CMAKE_SOURCE_DIR}/threadpool/threadpool.cpp)
include_directories("${CMAKE_SOURCE_DIR}/threadpool")

add_executable(integral ${CMAKE_SOURCE_DIR}/integral.cpp)
target_link_libraries (integral pthread)

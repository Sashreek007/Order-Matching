cmake_minimum_required(VERSION 3.10)
project(examples)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/liquibook/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/04_example.cpp")
add_executable(04_example ${SOURCES})

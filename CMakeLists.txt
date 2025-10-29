cmake_minimum_required(VERSION 3.10)
project(examples)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/liquibook/src)

file(GLOB SOURCES "src/02_example.cpp")
add_executable(order ${SOURCES})

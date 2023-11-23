cmake_minimum_required(VERSION 3.10)
project(lab01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main ${SOURCE_DIR}/main.cpp)
add_executable(child1 ${SOURCE_DIR}/child1.cpp)
add_executable(child2 ${SOURCE_DIR}/child2.cpp)
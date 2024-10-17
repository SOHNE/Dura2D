cmake_minimum_required(VERSION 3.14...3.26)

project(BuildAll LANGUAGES CXX)

include(../cmake/tools.cmake)

# Needed to generate test target
enable_testing()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../testbed ${CMAKE_BINARY_DIR}/testbed)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../test ${CMAKE_BINARY_DIR}/test)

# --------------------------------------------------------------------
# Project Setup
# --------------------------------------------------------------------
cmake_minimum_required(VERSION 3.26)
project(Dura2DUnitTests LANGUAGES CXX)

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --------------------------------------------------------------------
# Include External CMake Scripts
# --------------------------------------------------------------------
include(../cmake/tools.cmake)
include(../cmake/CPM.cmake)

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
CPMAddPackage("gh:doctest/doctest@2.4.11")
CPMAddPackage("gh:TheLartians/GroupSourcesByFolder.cmake@1.0")

if(TEST_INSTALLED_VERSION)
  find_package(Dura2D REQUIRED)
else()
  CPMAddPackage(NAME Dura2D SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# --------------------------------------------------------------------
# Target Definition
# --------------------------------------------------------------------
set(UNIT_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/hello_world.cpp
)

add_executable(${PROJECT_NAME} ${UNIT_TESTS_SOURCES})

target_link_libraries(${PROJECT_NAME} doctest::doctest Dura2D::Dura2D)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)

# --------------------------------------------------------------------
# Compiler Options
# --------------------------------------------------------------------
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(Dura2D PUBLIC -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(Dura2D PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# --------------------------------------------------------------------
# Testing Setup
# --------------------------------------------------------------------
enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# --------------------------------------------------------------------
# Test Coverage
# --------------------------------------------------------------------
if(ENABLE_TEST_COVERAGE)
  target_compile_options(Greeter PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(Greeter PUBLIC -fprofile-arcs -ftest-coverage)
endif()

# --------------------------------------------------------------------
# Source Grouping
# --------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})

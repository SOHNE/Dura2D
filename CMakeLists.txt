cmake_minimum_required(VERSION 3.15)
project(dura2d
        VERSION 0.0.2
        DESCRIPTION "An educational 2D physics engine"
        LANGUAGES C CXX
)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Configuration options
option(DURA2D_BUILD_UNIT_TESTS "Build the Dura2D unit tests" ON)
option(DURA2D_BUILD_TESTBED "Build the Dura2D testbed" ON)
option(DURA2D_MACOSX_BUNDLE "Build Dura2D as a macOS bundle" ON)

# Shared library
option(BUILD_SHARED_LIBS "Build Dura2D as a shared library" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include(GNUInstallDirs)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)

if (DURA2D_BUILD_UNIT_TESTS)
    add_subdirectory(unit-test)
endif ()

if (DURA2D_BUILD_TESTBED)
    add_subdirectory(vendor)
    add_subdirectory(testbed)
endif ()

install(
        DIRECTORY "${CMAKE_SOURCE_DIR}/include/dura2d"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

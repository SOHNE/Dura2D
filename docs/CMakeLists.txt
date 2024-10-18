# --------------------------------------------------------------------
# Project Setup
# --------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14...3.22)
project(
Dura2DDocs
DESCRIPTION "An educational 2D game physics library"
)


# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
include(../cmake/CPM.cmake)

get_filename_component(DURA2D_ABSOLUTE_PATH "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
CPMAddPackage(NAME Dura2D SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

CPMAddPackage("gh:jothepro/doxygen-awesome-css@2.3.4")

set(DOXYGEN_PROJECT_NAME Dura2D)
set(DOXYGEN_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(DOXYGEN_CUSTOM_DIR "${CMAKE_CURRENT_LIST_DIR}/doxygen-custom")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")

# --------------------------------------------------------------------
# Documentation Generation Target
# --------------------------------------------------------------------
find_program(DOXYGEN_EXECUTABLE doxygen)

if(DOXYGEN_EXECUTABLE)
  # create Doxyfile
  configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

  # note the option ALL which allows to build the docs together with the application
  add_custom_target(doxygenDocs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
  )

else()
  message(FATAL_ERROR "Doxygen need to be installed to generate the doxygen documentation")
endif()


# Only activate tools for top level project
if(NOT PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  return()
endif()

if(USE_CCACHE)
  CPMAddPackage("gh:TheLartians/Ccache.cmake@1.2.5")
endif()


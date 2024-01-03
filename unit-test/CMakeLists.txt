set(UNIT_TESTS_SOURCES
        doctest.h
        tests/hello_world.cpp
)

add_executable(unit_test ${UNIT_TESTS_SOURCES})

set_target_properties(unit_test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)

target_link_libraries(unit_test PUBLIC dura2d )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${UNIT_TESTS_SOURCES})


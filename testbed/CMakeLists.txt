set(TESTBED_SOURCE_FILES
        Application.cpp
)

set(TESTBED_HEADER_FILES
        raygui.h
        Application.h
)

add_executable(testbed)
target_sources(testbed PRIVATE main.cpp ${TESTBED_SOURCE_FILES} ${TESTBED_HEADER_FILES})
target_include_directories(testbed PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(testbed PUBLIC dura2d vendor)

set_target_properties(testbed PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# copy assets to runtime directory if assets exist
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
if (EXISTS ASSETS_DIR )
    add_custom_command(TARGET testbed POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
else ()
    message(STATUS "No assets directory found!")
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set( CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
    set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME} )
    set_property( TARGET testbed PROPERTY WIN32_EXECUTABLE true )

    target_link_options(testbed PUBLIC -static -static-libgcc -static-libstdc++)

endif ()

if( EMSCRIPTEN )
    if (EXISTS ASSETS_DIR)
        set_target_properties(testbed PROPERTIES LINK_FLAGS "--preload-file assets --use-preload-plugins")
    endif ()

    # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -s DEMANGLE_SUPPORT=1 -sUSE_GLFW=3 -sASSERTIONS=1 -sWASM=1 -sASYNCIFY")
    set_target_properties(testbed PROPERTIES SUFFIX ".html")
    set_target_properties(testbed PROPERTIES OUTPUT_NAME "index")
endif ()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    set_target_properties(testbed PROPERTIES MACOSX_BUNDLE ${DURA2D_MACOSX_BUNDLE})
    list(APPEND APPLE_FRAMEWORKS
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreVideo")
    target_link_libraries(testbed PRIVATE ${APPLE_FRAMEWORKS})
endif ()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TESTBED_SOURCE_FILES})

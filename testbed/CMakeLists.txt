# --------------------------------------------------------------------
# Project Setup
# --------------------------------------------------------------------
cmake_minimum_required(VERSION 3.26)
project(Dura2DTestBed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------------
# Output Directories
# --------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# --------------------------------------------------------------------
# Import Tools and Dependencies
# --------------------------------------------------------------------
include(../cmake/tools.cmake)
include(../cmake/CPM.cmake)

CPMAddPackage(NAME Dura2D SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
CPMAddPackage("gh:raysan5/raylib#5.0")
CPMAddPackage("gh:TheLartians/GroupSourcesByFolder.cmake@1.0")

# --------------------------------------------------------------------
# IMGUI Setup
# --------------------------------------------------------------------
cpmaddpackage(NAME IMGUI URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.91.3.zip")

if(IMGUI_ADDED)
    add_library(IMGUI STATIC)
    target_sources(IMGUI
        PRIVATE
            ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
            ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
            ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
            ${IMGUI_SOURCE_DIR}/imgui.cpp
            ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(IMGUI
        PUBLIC
            ${IMGUI_SOURCE_DIR}
            ${IMGUI_SOURCE_DIR}/backends
    )
    find_package(OpenGL REQUIRED)
    target_link_libraries(IMGUI PUBLIC ${OPENGL_gl_LIBRARY} ${CMAKE_DL_LIBS})
endif()

# --------------------------------------------------------------------
# rlImGui Setup
# --------------------------------------------------------------------
cpmaddpackage(
    NAME rlImGui
    URL https://github.com/raylib-extras/rlImGui/archive/refs/heads/main.zip
    URL_HASH SHA256=6ad30587261ecee9c4ec6c1e3db0499fa7a27ea51865698b7e450432dbbb3a63
)

if(rlImGui_ADDED)
    set(RLIMGUI_SOURCE_FILES ${rlImGui_SOURCE_DIR}/rlImGui.cpp)
    set(RLIMGUI_HEADER_FILES
        ${rlImGui_SOURCE_DIR}/rlImGui.h
        ${rlImGui_SOURCE_DIR}/rlImGuiColors.h
        ${rlImGui_SOURCE_DIR}/extras/FA6FreeSolidFontData.h
        ${rlImGui_SOURCE_DIR}/extras/IconsFontAwesome6.h
    )
    add_library(rlImGui STATIC ${RLIMGUI_SOURCE_FILES} ${RLIMGUI_HEADER_FILES})
    target_include_directories(rlImGui
        PUBLIC
            $<BUILD_INTERFACE:${rlImGui_SOURCE_DIR}>
            ${IMGUI_SOURCE_DIR}
            ${IMGUI_SOURCE_DIR}/backends
    )
    target_link_libraries(rlImGui PUBLIC IMGUI raylib)
endif()

# --------------------------------------------------------------------
# Testbed Setup
# --------------------------------------------------------------------
set(TESTBED_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/Test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/Bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/Collisions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/Playground.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases/Wrecking_Ball.cpp
)

set(TESTBED_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/settings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/draw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Test.h
)

add_executable(${PROJECT_NAME} ${TESTBED_SOURCE_FILES} ${TESTBED_HEADER_FILES})
target_include_directories(${PROJECT_NAME}
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        OUTPUT_NAME "Testbed"
)

target_link_libraries(${PROJECT_NAME} PUBLIC Dura2D::Dura2D raylib IMGUI rlImGui)

# --------------------------------------------------------------------
# Platform-Specific Configuration
# --------------------------------------------------------------------
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
    target_link_options(${PROJECT_NAME} PUBLIC -static -static-libgcc -static-libstdc++)
endif()

if(EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s ALLOW_MEMORY_GROWTH")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
endif()

if(APPLE)
    list(APPEND APPLE_FRAMEWORKS
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLE_FRAMEWORKS})
endif()

# --------------------------------------------------------------------
# Source Grouping
# --------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})


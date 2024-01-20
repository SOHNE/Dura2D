# imgui
set(IMGUI_SOURCE_FILES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    misc/cpp/imgui_stdlib.cpp
)

set(IMGUI_HEADER_FILES
    imconfig.h
    imgui.h
    imgui_internal.h
    imgui_user_config.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h

    misc/cpp/imgui_stdlib.h
    misc/freetype/imgui_freetype.h
)

add_library(imgui STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG="imgui_user_config.h")
target_compile_definitions(imgui PRIVATE $<$<CONFIG:Release>:IMGUI_DISABLE_DEMO_WINDOWS> $<$<CONFIG:Release>:IMGUI_DISABLE_DEBUG_TOOLS>)
target_include_directories(imgui PUBLIC .)

add_executable(binary_to_compressed_c misc/fonts/binary_to_compressed_c.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
# rlImGui
set(RLIMGUI_SOURCE_FILES
    rlImGui.cpp
)

set(RLIMGUI_HEADER_FILES
    rlImGui.h
    rlImGuiColors.h

    extras/FA6FreeSolidFontData.h
    extras/IconsFontAwesome6.h
)

add_library(rlimgui STATIC ${RLIMGUI_SOURCE_FILES} ${RLIMGUI_HEADER_FILES})
#target_compile_definitions(rlimgui PUBLIC NO_FONT_AWESOME)
target_include_directories(rlimgui PUBLIC .)
target_link_libraries(rlimgui PUBLIC imgui raylib)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RLIMGUI_SOURCE_FILES} ${RLIMGUI_HEADER_FILES})

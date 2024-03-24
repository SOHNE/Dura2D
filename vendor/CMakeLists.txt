add_library(vendor INTERFACE)

# Raylib
set(CUSTOMIZE_BUILD ON CACHE BOOL "Customize Raylib build")
set(SUPPORT_MODULE_RMODELS OFF CACHE BOOL "Support module rmodels")
set(SUPPORT_MODULE_RAUDIO OFF CACHE BOOL "Support module raudio")
add_subdirectory( raylib )
target_link_libraries(vendor INTERFACE raylib)

# ImGui
add_subdirectory( imgui )
target_link_libraries(vendor INTERFACE imgui)

# ImGui Raylib
add_subdirectory( rlimgui )
target_link_libraries(vendor INTERFACE rlimgui)

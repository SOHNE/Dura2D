add_library(vendor INTERFACE)

# Raylib
add_subdirectory( raylib )
target_link_libraries(vendor INTERFACE raylib)

# ImGui
add_subdirectory( imgui )
target_link_libraries(vendor INTERFACE imgui)

# ImGui Raylib
add_subdirectory( rlimgui )
target_link_libraries(vendor INTERFACE rlimgui)

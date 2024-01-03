add_library(vendor INTERFACE)

# Raylib
add_subdirectory( raylib )
target_link_libraries(vendor INTERFACE raylib)

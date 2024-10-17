include(CMakeDependentOption)

#--------------------------------------------------------------------
# Build Options
#--------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build Dura2D as a shared library" OFF)
option(USE_CCACHE "Enable compiler cache that can drastically improve build times" ${DURA_IS_MAIN})

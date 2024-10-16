#--------------------------------------------------------------------
# Configuration Options
#--------------------------------------------------------------------
option(DURA_BUILD_TESTBED "Build the Dura2D testbed" $DURA_IS_MAIN})
option(DURA_BUILD_UNIT_TESTS "Build the Dura2D unit tests" ON)

#--------------------------------------------------------------------
# Build Options
#--------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build Dura2D as a shared library" OFF)
option(USE_CCACHE "Enable compiler cache that can drastically improve build times" ON)

set(DURA2D_SOURCE_FILES
        kinetics/d2Body.cpp
        collision/d2CollisionDetection.cpp
        collision/d2Constraint.cpp
        kinetics/d2Force.cpp
        math/d2Vec2.cpp
        math/d2MatMN.cpp
        collision/d2Shape.cpp
        math/d2VecN.cpp
        kinetics/d2World.cpp
        collision/d2NSquaredBroad.cpp
        common/d2BlockAllocator.cpp
)

set(DURA2D_HEADER_FILES
        ../include/dura2d/d2api.h
        ../include/dura2d/d2Body.h
        ../include/dura2d/d2AABB.h
        ../include/dura2d/d2Broadphase.h
        ../include/dura2d/d2CollisionDetection.h
        ../include/dura2d/d2Constraint.h
        ../include/dura2d/d2Force.h
        ../include/dura2d/d2Math.h
        ../include/dura2d/d2Shape.h
        ../include/dura2d/d2World.h
        ../include/dura2d/d2NSquaredBroad.h
        ../include/dura2d/dura2d.h
        ../include/dura2d/memory/d2BlockAllocator.h
        ../include/dura2d/d2Timer.h
        ../include/dura2d/d2Types.h
        ../include/dura2d/d2Draw.h
)

add_library(dura2d ${DURA2D_SOURCE_FILES} ${DURA2D_HEADER_FILES} )
target_include_directories(dura2d
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(dura2d PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

if (BUILD_SHARED_LIBS)
    message(STATUS "Building shared library")
    target_compile_definitions(dura2d PUBLIC D2_SHARED)
endif ()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${DURA2D_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "include" FILES ${DURA2D_HEADER_FILES})

install(
        TARGETS dura2d
        EXPORT dura2dConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION include
)

install(
        EXPORT dura2dConfig
        NAMESPACE dura2d::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dura2d
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/dura2dConfigVersion.cmake"
        COMPATIBILITY AnyNewerVersion
)

install(
        FILES "${CMKAE_CURRENT_BINARY_DIR}/dura2dConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dura2d
)

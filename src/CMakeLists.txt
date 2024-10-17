#--------------------------------------------------------------------
# Source and Header Files
#--------------------------------------------------------------------
set(DURA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/dura2d)
set(DURA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Header files
set(DURA_HEADER_FILES
    ${DURA_INCLUDE_DIR}/d2api.h
    ${DURA_INCLUDE_DIR}/d2Body.h
    ${DURA_INCLUDE_DIR}/d2AABB.h
    ${DURA_INCLUDE_DIR}/d2AABBTree.h
    ${DURA_INCLUDE_DIR}/d2Broadphase.h
    ${DURA_INCLUDE_DIR}/d2CollisionDetection.h
    ${DURA_INCLUDE_DIR}/d2Constraint.h
    ${DURA_INCLUDE_DIR}/d2Force.h
    ${DURA_INCLUDE_DIR}/d2Math.h
    ${DURA_INCLUDE_DIR}/d2Shape.h
    ${DURA_INCLUDE_DIR}/d2World.h
    ${DURA_INCLUDE_DIR}/d2NSquaredBroad.h
    ${DURA_INCLUDE_DIR}/dura2d.h
    ${DURA_INCLUDE_DIR}/d2Timer.h
    ${DURA_INCLUDE_DIR}/d2Types.h
    ${DURA_INCLUDE_DIR}/d2Draw.h

    ${DURA_INCLUDE_DIR}/memory/d2BlockAllocator.h
)

set(DURA_SOURCE_FILES
    ${DURA_SOURCE_DIR}/kinetics/d2Body.cpp
    ${DURA_SOURCE_DIR}/collision/d2AABBTree.cpp
    ${DURA_SOURCE_DIR}/collision/d2CollisionDetection.cpp
    ${DURA_SOURCE_DIR}/collision/d2Constraint.cpp
    ${DURA_SOURCE_DIR}/kinetics/d2Force.cpp
    ${DURA_SOURCE_DIR}/math/d2Vec2.cpp
    ${DURA_SOURCE_DIR}/math/d2MatMN.cpp
    ${DURA_SOURCE_DIR}/collision/d2Shape.cpp
    ${DURA_SOURCE_DIR}/math/d2VecN.cpp
    ${DURA_SOURCE_DIR}/kinetics/d2World.cpp
    ${DURA_SOURCE_DIR}/collision/d2NSquaredBroad.cpp
    ${DURA_SOURCE_DIR}/common/d2BlockAllocator.cpp
)

#--------------------------------------------------------------------
# Library Setup
#--------------------------------------------------------------------
add_library(${PROJECT_NAME} ${DURA_SOURCE_FILES} ${DURA_HEADER_FILES})
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

#--------------------------------------------------------------------
# Shared Library Configuration
#--------------------------------------------------------------------
if(BUILD_SHARED_LIBS)
  message(STATUS "Building shared library")
  target_compile_definitions(${PROJECT_NAME} PUBLIC D2_SHARED)
endif()

#--------------------------------------------------------------------
# Source Groups
#--------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})


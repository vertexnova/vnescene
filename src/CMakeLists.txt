#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# Author:    Ajeet Singh Yadav
# Created:   February 2026
#
# Autodoc:   yes
#==============================================================================

set(SOURCE_FILES
    vertexnova/scene/camera/perspective_camera.cpp
    vertexnova/scene/camera/orthographic_camera.cpp
    vertexnova/scene/camera/camera_factory.cpp
    vertexnova/scene/camera/camera_utils.cpp
    vertexnova/scene/camera/camera_gpu.cpp
    vertexnova/scene/scene_state.cpp
    vertexnova/scene/light/ambient_light.cpp
    vertexnova/scene/light/directional_light.cpp
    vertexnova/scene/light/point_light.cpp
    vertexnova/scene/light/spot_light.cpp
)

set(HEADER_FILES
    ${VNE_INCLUDE_DIR}/vertexnova/scene/export.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/scene.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/handle.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/scene_state.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/environment.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/gpu_types.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera_base.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera_types.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera_utils.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera_factory.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera_gpu.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/camera_transform_adapter.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/perspective_camera.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/camera/orthographic_camera.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/light/light.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/light/light_gpu.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/light/ambient_light.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/light/directional_light.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/light/point_light.h
    ${VNE_INCLUDE_DIR}/vertexnova/scene/light/spot_light.h
)

# One library per build: static or shared, always named vnescene (alias vne::scene).
if(VNE_SCENE_LIB_TYPE STREQUAL "shared")
    set(_lib_type SHARED)
else()
    set(_lib_type STATIC)
endif()

add_library(vnescene ${_lib_type} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(vnescene
    PUBLIC
        $<BUILD_INTERFACE:${VNE_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${VNE_SRC_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(vnescene
    PUBLIC
        vne::scene::Warnings
        vne::scene::BuildSettings
        vne::math
    PRIVATE
        Threads::Threads
)

if(TARGET vne::common)
    target_link_libraries(vnescene PUBLIC vne::common)
endif()
if(TARGET vne::logging)
    target_link_libraries(vnescene PUBLIC vne::logging)
endif()

target_compile_features(vnescene PUBLIC cxx_std_20)

# Set export/import defines for shared builds. Headers use VNE_SCENE_API (dllexport/dllimport on Windows).
if(_lib_type STREQUAL "SHARED")
    target_compile_definitions(vnescene
        PRIVATE VNE_SCENE_BUILDING_DLL
        INTERFACE VNE_SCENE_DLL
    )
    # C4251: non-exported types (vnemath Vec/Mat, std::string, etc.) in exported class members; expected for DLL.
    if(MSVC)
        target_compile_options(vnescene PRIVATE /wd4251)
    endif()
endif()

# iOS/Xcode: satisfy "Signing requires a development team" for the vnescene target.
# Set DEVELOPMENT_TEAM when provided (e.g. -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=TEAM_ID).
# When not set (e.g. CI), disable code signing for this library so the build can proceed.
if(CMAKE_GENERATOR STREQUAL "Xcode" AND (IOS OR CMAKE_SYSTEM_NAME STREQUAL "iOS"))
    if(DEFINED CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM AND CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM)
        set_target_properties(vnescene PROPERTIES
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM}")
    else()
        # Static/shared lib: fully suppress signing (CI has no provisioning/keychain).
        set_target_properties(vnescene PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
    endif()
endif()

add_library(vne::scene ALIAS vnescene)

source_group("Sources" FILES ${SOURCE_FILES})
source_group(TREE ${VNE_INCLUDE_DIR} PREFIX "Headers" FILES ${HEADER_FILES})

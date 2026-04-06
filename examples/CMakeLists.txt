#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#==============================================================================

if(NOT TARGET vnescene)
    return()
endif()

#------------------------------------------------------------------------------
# Register one example: main.cpp + extra sources; link vne::scene; include ../ for common/
#------------------------------------------------------------------------------
macro(vne_add_example target_name)
    add_executable(${target_name} main.cpp ${ARGN})
    target_link_libraries(${target_name} PRIVATE vne::scene)
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
    )
endmacro()

# 01 - Basic: minimal scene (camera + lights) with logging guard
add_subdirectory(01_basic)

# 02 - Scene GPU pack: SceneState, pack camera and lights to GPU
add_subdirectory(02_scene_gpu_pack)

# 03 - Camera backend: setGraphicsApi (OpenGL / Vulkan), log matrix differences
add_subdirectory(03_camera_backend)

# 04 - Fit to AABB: fitToAabb, project AABB corners
add_subdirectory(04_fit_to_aabb)

# 05 - Screen ray: screenToWorldRay at center pixel
add_subdirectory(05_screen_ray)

# 06 - Shadow settings: directional + spot, setShadowSettings round-trip
add_subdirectory(06_shadow_settings)

# 07 - Camera transform sync: syncCameraFromTransformNode / syncTransformNodeFromCamera
add_subdirectory(07_camera_transform_sync)

#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# FindVneScene.cmake
# Finds the VneScene library and sets up the imported target vne::scene.
#
# Usage:
#   list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/lib/cmake/VneScene")
#   find_package(VneScene REQUIRED)
#   target_link_libraries(your_target PRIVATE vne::scene)
#==============================================================================

include(FindPackageHandleStandardArgs)

find_package(VneMath REQUIRED)
find_package(Threads REQUIRED)

find_path(VneScene_INCLUDE_DIR
    NAMES vertexnova/scene/scene.h
    PATHS
        ${CMAKE_INSTALL_PREFIX}/include
        /usr/local/include
        /usr/include
)

find_library(VneScene_LIBRARY
    NAMES vnescene
    PATHS
        ${CMAKE_INSTALL_PREFIX}/lib
        /usr/local/lib
        /usr/lib
)

find_package_handle_standard_args(VneScene
    REQUIRED_VARS VneScene_LIBRARY VneScene_INCLUDE_DIR
)

if(VneScene_FOUND AND NOT TARGET vne::scene)
    add_library(vne::scene UNKNOWN IMPORTED)
    set_target_properties(vne::scene PROPERTIES
        IMPORTED_LOCATION "${VneScene_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${VneScene_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "vne::math;Threads::Threads"
    )
endif()

mark_as_advanced(VneScene_INCLUDE_DIR VneScene_LIBRARY)

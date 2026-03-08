#==============================================================================
# 07_camera_transform_sync Example: syncCameraFromTransformNode / syncTransformNodeFromCamera
#==============================================================================

add_executable(example_07_camera_transform_sync main.cpp)

target_link_libraries(example_07_camera_transform_sync PRIVATE vne::scene)

target_include_directories(example_07_camera_transform_sync PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_07_camera_transform_sync PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

#==============================================================================
# 03_camera_backend Example: setGraphicsApi (OpenGL / Vulkan)
#==============================================================================

add_executable(example_03_camera_backend main.cpp)

target_link_libraries(example_03_camera_backend PRIVATE vne::scene)

target_include_directories(example_03_camera_backend PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_03_camera_backend PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

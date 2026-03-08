#==============================================================================
# 02_scene_gpu_pack Example: SceneState, pack camera and lights to GPU
#==============================================================================

add_executable(example_02_scene_gpu_pack main.cpp)

target_link_libraries(example_02_scene_gpu_pack PRIVATE vne::scene vne::logging)

target_include_directories(example_02_scene_gpu_pack PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_02_scene_gpu_pack PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

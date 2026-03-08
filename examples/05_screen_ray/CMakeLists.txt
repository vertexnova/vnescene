#==============================================================================
# 05_screen_ray Example: screenToWorldRay at center pixel
#==============================================================================

add_executable(example_05_screen_ray main.cpp)

target_link_libraries(example_05_screen_ray PRIVATE vne::scene)

target_include_directories(example_05_screen_ray PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_05_screen_ray PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

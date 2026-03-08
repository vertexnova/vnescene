#==============================================================================
# 04_fit_to_aabb Example: fitToAabb, project AABB corners
#==============================================================================

add_executable(example_04_fit_to_aabb main.cpp)

target_link_libraries(example_04_fit_to_aabb PRIVATE vne::scene)

target_include_directories(example_04_fit_to_aabb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_04_fit_to_aabb PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

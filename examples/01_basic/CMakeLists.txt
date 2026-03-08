#==============================================================================
# 01_basic Example: minimal scene (camera + lights) with logging
#==============================================================================

add_executable(example_01_basic main.cpp)

target_link_libraries(example_01_basic PRIVATE vne::scene vne::logging)

target_include_directories(example_01_basic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_01_basic PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

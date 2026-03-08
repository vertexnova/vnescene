#==============================================================================
# 06_shadow_settings Example: setShadowSettings / getShadowSettings round-trip
#==============================================================================

add_executable(example_06_shadow_settings main.cpp)

target_link_libraries(example_06_shadow_settings PRIVATE vne::scene)

target_include_directories(example_06_shadow_settings PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_06_shadow_settings PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples"
)

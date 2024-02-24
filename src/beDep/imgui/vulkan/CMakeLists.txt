# compile shaders
add_custom_target(
    imgui_compile_shaders
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_spv.sh
    COMMENT "Compiling imgui shaders..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)

add_dependencies(BigoudiEngine imgui_compile_shaders)
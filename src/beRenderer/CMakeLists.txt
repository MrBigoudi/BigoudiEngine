# include directories for the Bigoudi Engine Renderer library
target_include_directories(BigoudiEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vulkan
        ${CMAKE_CURRENT_SOURCE_DIR}/renderingSubSystems
)

# add the subdirectories
add_subdirectory(vulkan)
add_subdirectory(renderingSubSystems)
add_subdirectory(pbr)
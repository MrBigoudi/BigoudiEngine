# include directories for the Bigoudi Engine Dependencies library
target_include_directories(BigoudiEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/stbImage
        ${CMAKE_CURRENT_SOURCE_DIR}/tinyObjectLoadr
        ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf
)

# add the subdirectories
add_subdirectory(stbImage)
add_subdirectory(tinyObjectLoader)
add_subdirectory(imgui)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(tinygltf)
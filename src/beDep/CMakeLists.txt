# include directories for the Bigoudi Engine Dependencies library
target_include_directories(BigoudiEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/stbImage
        ${CMAKE_CURRENT_SOURCE_DIR}/tinyObjectLoadr
)

# add the subdirectories
add_subdirectory(stbImage)
add_subdirectory(tinyObjectLoader)
add_subdirectory(imgui)
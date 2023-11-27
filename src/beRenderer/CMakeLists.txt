# find the source files
file(GLOB BE_RENDERER_SOURCE_FILES "*.cpp")

# create the library for the renderer of the engine
add_library(BeRenderer ${BE_RENDERER_SOURCE_FILES})

# link to the other libraries
target_link_libraries(BeRenderer PRIVATE BeCore)
target_link_libraries(BeRenderer PRIVATE BeMaths)

# include directories for the Bigoudi Engine Renderer library
target_include_directories(BeRenderer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
)

# use OpenGL
find_package(OpenGL REQUIRED)

# set the cpp standards
target_link_libraries(BeRenderer PUBLIC cflags ${OPENGL_LIBRARIES})

# add the subdirectories
add_subdirectory(shaders)
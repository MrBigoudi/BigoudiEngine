# find the source files in the folder
file(GLOB BE_RENDERER_SHADERS_SOURCE_FILES "*.cpp")

# add the source files to the Bigoudi Engine Renderer library
target_sources(BeRenderer PRIVATE ${BE_RENDERER_SHADERS_SOURCE_FILES})

# include directories for the Bigoudi Engine Renderer library (if needed)
target_include_directories(BeRenderer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
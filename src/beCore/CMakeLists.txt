# find the source files
file(GLOB BE_CORE_SOURCE_FILES "*.cpp")

# create the library for the core of the engine
add_library(BeCore ${BE_CORE_SOURCE_FILES})

# include directories for the Bigoudi Engine Core library
target_include_directories(BeCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/debug
)

# use OpenGL
find_package(OpenGL REQUIRED)

# set the cpp standards
target_link_libraries(BeCore PUBLIC cflags ${OPENGL_LIBRARIES})

# add the subdirectories
add_subdirectory(debug)
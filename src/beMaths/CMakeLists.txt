# find the source files
file(GLOB BE_MATHS_SOURCE_FILES "*.cpp")

# create the library for the maths of the engine
add_library(BeMaths ${BE_MATHS_SOURCE_FILES})

# link to the other libraries
target_link_libraries(BeMaths PRIVATE BeCore)

# include directories for the Bigoudi Engine Maths library
target_include_directories(BeMaths
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix
        ${CMAKE_CURRENT_SOURCE_DIR}/vector
)

# set the cpp standards
target_link_libraries(BeMaths PUBLIC cflags)

#include subdirectories
add_subdirectory(matrix)
add_subdirectory(vector)
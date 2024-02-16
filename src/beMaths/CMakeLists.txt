# include directories for the Bigoudi Engine Maths library
target_include_directories(BigoudiEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix
        ${CMAKE_CURRENT_SOURCE_DIR}/vector
        ${CMAKE_CURRENT_SOURCE_DIR}/other
)

#include subdirectories
add_subdirectory(matrix)
add_subdirectory(vector)
add_subdirectory(other)
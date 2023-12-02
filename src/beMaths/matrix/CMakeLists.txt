# include directories for the Bigoudi Engine Maths library (if needed)
target_include_directories(BeMaths PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# include subdirectories
add_subdirectory(matrix1)
add_subdirectory(matrix2)
add_subdirectory(matrix3)
add_subdirectory(matrix4)
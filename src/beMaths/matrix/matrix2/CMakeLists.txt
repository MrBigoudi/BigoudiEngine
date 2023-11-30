# find the source files in the folder
file(GLOB BE_MATHS_MATRIX2_SOURCE_FILES "*.cpp")

# add the source files to the Bigoudi Engine Maths library
target_sources(BeMaths PRIVATE ${BE_MATHS_MATRIX2_SOURCE_FILES})

# include directories for the Bigoudi Engine Maths library (if needed)
target_include_directories(BeMaths PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
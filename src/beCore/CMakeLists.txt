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
        ${CMAKE_CURRENT_SOURCE_DIR}/time
)

# add the subdirectories
add_subdirectory(debug)
add_subdirectory(time)
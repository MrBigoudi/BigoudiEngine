# find the source files
file(GLOB BE_PHYSICS_SOURCE_FILES "*.cpp")

# create the library for the physics of the engine
add_library(BePhysics ${BE_PHYSICS_SOURCE_FILES})

# link to the other libraries
target_link_libraries(BePhysics PRIVATE BeCore)
target_link_libraries(BePhysics PRIVATE BeMaths)

# include directories for the Bigoudi Engine Physics library
target_include_directories(BePhysics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/collider
)

# set the cpp standards
target_link_libraries(BePhysics PUBLIC cflags)

#include subdirectories
add_subdirectory(collider)
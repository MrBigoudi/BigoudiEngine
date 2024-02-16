# find the source files in the folder
file(GLOB BE_PHYSICS_COLLIDER_SOURCE_FILES "*.cpp")

# add the source files to the Bigoudi Engine Physics library
target_sources(BigoudiEngine PRIVATE ${BE_PHYSICS_COLLIDER_SOURCE_FILES})

# include directories for the Bigoudi Engine Physics library (if needed)
target_include_directories(BigoudiEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
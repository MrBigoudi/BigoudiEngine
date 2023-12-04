# find the source files
file(GLOB BE_SOURCE_FILES "*.cpp")

# create the library for the renderer of the engine
add_library(BigoudiEngine ${BE_SOURCE_FILES})

# link to the other libraries
target_link_libraries(BigoudiEngine PRIVATE BeCore)
target_link_libraries(BigoudiEngine PRIVATE BeMaths)
target_link_libraries(BigoudiEngine PRIVATE BePhysics)
target_link_libraries(BigoudiEngine PRIVATE BeRenderer)

# include directories for the Bigoudi Engine library
target_include_directories(BigoudiEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
)

# set the cpp standards
target_link_libraries(BigoudiEngine PUBLIC cflags)

# add custom libraries
add_subdirectory(beCore)
add_subdirectory(beMaths)
add_subdirectory(bePhysics)
add_subdirectory(beRenderer)
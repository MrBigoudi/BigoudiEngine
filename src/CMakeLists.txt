# include directories for the Bigoudi Engine library
target_include_directories(BigoudiEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
)

# add subdirectories
add_subdirectory(beDep)
add_subdirectory(beCore)
add_subdirectory(beMaths)
add_subdirectory(bePhysics)
add_subdirectory(beRenderer)
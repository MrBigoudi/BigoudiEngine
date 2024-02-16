# include directories for the Bigoudi Engine Core library
target_include_directories(BigoudiEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/debug
        ${CMAKE_CURRENT_SOURCE_DIR}/entityComponentSystem
        ${CMAKE_CURRENT_SOURCE_DIR}/gameplay
        ${CMAKE_CURRENT_SOURCE_DIR}/time
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# add the subdirectories
add_subdirectory(debug)
add_subdirectory(entityComponentSystem)
add_subdirectory(gameplay)
add_subdirectory(time)
add_subdirectory(utils)
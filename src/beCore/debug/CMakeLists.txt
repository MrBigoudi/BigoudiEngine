# find the source files in the folder
file(GLOB BE_CORE_DEBUG_SOURCE_FILES "*.cpp")

# add the source files to the Bigoudi Engine Core library
target_sources(BigoudiEngine PRIVATE ${BE_CORE_DEBUG_SOURCE_FILES})

# include directories for the Bigoudi Engine Core library (if needed)
target_include_directories(BigoudiEngine 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
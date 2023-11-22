# add files
file(GLOB APP_SOURCE_FILES "*.cpp")

# create a library from the source files
add_library(AppLib ${APP_SOURCE_FILES})

# include directories for the AppLib
target_include_directories(AppLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(AppLib PUBLIC cflags)

# add the source files to the App library
target_sources(AppLib PRIVATE ${APP_SOURCE_FILES})
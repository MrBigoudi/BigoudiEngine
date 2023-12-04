cmake_minimum_required(VERSION 3.15)

# project name
project(BigoudiEngineV2 VERSION 0.1)
# pass the variable to the source code as a preprocessor definition
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")
add_compile_definitions(PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_compile_definitions(PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR})

# debug
set(CMAKE_DEBUG_POSTFIX d)

# c++ standard
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(cflags ${PROJECT_NAME}_compiler_flags)
add_library(cflags INTERFACE)
target_compile_features(cflags INTERFACE cxx_std_17)
target_compile_options(cflags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# create compile command
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# main executable
add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# add source files
add_subdirectory(src)
target_link_libraries(${PROJECT_NAME} PUBLIC BigoudiEngine cflags)
target_link_libraries(${PROJECT_NAME} PUBLIC BeCore cflags)
target_link_libraries(${PROJECT_NAME} PUBLIC BeMaths cflags)
target_link_libraries(${PROJECT_NAME} PUBLIC BePhysics cflags)
target_link_libraries(${PROJECT_NAME} PUBLIC BeRenderer cflags)

# add the headers
target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                          )

# testing
enable_testing()
add_test(NAME Runs COMMAND ${PROJECT_NAME})
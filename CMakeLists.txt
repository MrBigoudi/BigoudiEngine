cmake_minimum_required(VERSION 3.15)

# project name
project(mainTest VERSION 0.1)

# pass the variable to the source code as a preprocessor definition
add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")
add_compile_definitions(PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
add_compile_definitions(PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR})

# create compile command
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add source files
add_subdirectory(src)

# main executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE BigoudiEngine)
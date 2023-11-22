# setup project
cmake_minimum_required(VERSION 3.10)
project(BigoudiEngineV2)

# cmake commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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

# main executable
add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# add source files
add_subdirectory(src)
target_link_libraries(${PROJECT_NAME} PUBLIC AppLib cflags)

# add glfw
find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# add glm
find_package(glm REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIRS})

# add vulkan
find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})

# add the headers
target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                          )
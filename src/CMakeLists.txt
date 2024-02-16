# find the source files
file(GLOB BE_SOURCE_FILES "*.cpp")

# create the library for the renderer of the engine
add_library(BigoudiEngine STATIC ${BE_SOURCE_FILES})

# include directories for the Bigoudi Engine library
target_include_directories(BigoudiEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
)

# dependencies
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)

# add subdirectories
add_subdirectory(beDep)
add_subdirectory(beCore)
add_subdirectory(beMaths)
add_subdirectory(bePhysics)
add_subdirectory(beRenderer)

# c++ standard
target_compile_features(BigoudiEngine PUBLIC cxx_std_20)

# compile flags
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(cflags BigoudiEngine_compiler_flags)

add_library(cflags INTERFACE)
target_compile_features(cflags INTERFACE cxx_std_20)
target_compile_options(cflags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# set the cpp standards
target_link_libraries(BigoudiEngine PRIVATE glfw Vulkan::Vulkan cflags)
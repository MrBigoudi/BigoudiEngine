file(GLOB BE_DEP_IMGUI_SOURCE_FILES "*.cpp")

target_sources(BigoudiEngine PRIVATE ${BE_DEP_IMGUI_SOURCE_FILES})

target_include_directories(BigoudiEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(vulkan)
add_subdirectory(backends)
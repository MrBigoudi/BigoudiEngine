file(GLOB BE_DEP_IMGUI_IMPL_SOURCE_FILES "*.cpp")

target_sources(BigoudiEngine PUBLIC ${BE_DEP_IMGUI_IMPL_SOURCE_FILES})

target_include_directories(BigoudiEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
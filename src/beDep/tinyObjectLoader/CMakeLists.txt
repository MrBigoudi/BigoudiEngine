file(GLOB BE_DEP_TINY_OBJECT_LOADER_SOURCE_FILES "*.cpp")

target_sources(BigoudiEngine PRIVATE ${BE_DEP_TINY_OBJECT_LOADER_SOURCE_FILES})

target_include_directories(BigoudiEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
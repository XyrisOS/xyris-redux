project(MemoryLayout)
message(STATUS "    [-] Memory Layout")

add_library(MemoryLayout INTERFACE)

target_include_directories(MemoryLayout
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
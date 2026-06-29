project(Arch)
message(STATUS "    [-] Arch")

# Define an Arch target that must exist, that each individual architecture adds sources to
add_library(Arch OBJECT)
# Define the public CPU architecture API
target_include_directories(Arch
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(amd64)

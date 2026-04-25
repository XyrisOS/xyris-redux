project(Tools)
message(STATUS "  [-] Tools")

# Limine submodule is pre-built so we need to set the library here
add_library(Limine INTERFACE)

target_include_directories(Limine INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/limine")

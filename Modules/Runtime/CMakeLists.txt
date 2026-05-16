project(Runtime)
message(STATUS "    [-] Runtime")

add_library(Runtime OBJECT
    Memory.cpp
    StackProtector.cpp
    HaltAndCatchFire.cpp
)

target_include_directories(Runtime
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

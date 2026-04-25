project(Loader)
message(STATUS "  [-] Loader")

add_library(Loader OBJECT
    Entry.cpp
)

target_link_libraries(Loader PRIVATE Limine)
target_include_directories(Loader
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

install(FILES limine.cfg DESTINATION .)

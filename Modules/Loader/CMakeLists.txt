cmake_minimum_required(VERSION 3.27)
project(Loader)
message(STATUS "\t[-] Loader")

add_library(Loader STATIC
    Entry.cpp
)

target_link_libraries(Loader Limine)
target_include_directories(Loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES limine.cfg DESTINATION ${CMAKE_INSTALL_PREFIX})

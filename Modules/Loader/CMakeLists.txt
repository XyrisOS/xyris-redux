cmake_minimum_required(VERSION 3.27)
project(Loader)
message(STATUS "\t[-] Loader")

add_library(Loader STATIC
    Entry.cpp
)

target_link_libraries(Loader Limine)

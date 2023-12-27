cmake_minimum_required(VERSION 3.27)
project(Tools)
message(STATUS "  [-] Tools")

# Limine submodule is pre-built so we need to set the library here
add_library(Limine INTERFACE
    limine/limine.h
)

target_include_directories(Limine INTERFACE limine)

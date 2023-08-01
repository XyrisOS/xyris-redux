cmake_minimum_required(VERSION 3.27)
message(STATUS "[*] Tools")

add_library(Limine INTERFACE
    limine/limine.h
)

target_include_directories(Limine INTERFACE limine)

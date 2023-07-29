cmake_minimum_required(VERSION 3.27)
message(STATUS "[*] Modules")

set(CMAKE_STATIC_LIBRARY_SUFFIX "")

include(InitializeStandardFlags)
include(InitializeFreestandingFlags)

add_subdirectory(Loader)
add_subdirectory(Kernel)

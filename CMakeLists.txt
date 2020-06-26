cmake_minimum_required(VERSION 3.16)
project(NSG CXX)

set(CMAKE_CXX_STANDARD 11)


add_compile_options(-Wall)

# Output directory for binaries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(NSG
        src/main.cpp
        src/Options.hpp
        src/Options.cpp
        src/json.hpp)

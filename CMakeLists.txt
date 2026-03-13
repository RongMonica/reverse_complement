cmake_minimum_required(VERSION 3.16)

project(reverse_complement_tool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    reverse_complement
    src/application.cpp
    src/csv_writer.cpp
    src/fasta_io.cpp
    src/main.cpp
    src/sequence_processing.cpp
)

target_include_directories(reverse_complement PRIVATE include)

if(MSVC)
    target_compile_options(reverse_complement PRIVATE /W4)
else()
    target_compile_options(reverse_complement PRIVATE -Wall -Wextra -Wpedantic)
endif()

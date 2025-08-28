cmake_minimum_required(VERSION 3.26)
project(
        SimpleSearchEngine
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 labwork11"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)
add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)


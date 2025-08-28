cmake_minimum_required(VERSION 3.26)

project(
    Sandpile
    VERSION 1.0
    DESCRIPTION "C++ Year 2023 Labwork3"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)

add_executable(Sandpile main.cpp
        test.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static")

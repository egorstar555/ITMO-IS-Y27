cmake_minimum_required(VERSION 3.26)
project(PrintFile)

set(CMAKE_CXX_STANDARD 20)

add_executable(PrintFile main.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static")

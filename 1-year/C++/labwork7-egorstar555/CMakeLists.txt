cmake_minimum_required(VERSION 3.12)

project(
    WeatherForecast
    VERSION 1.0
    DESCRIPTION "C++ Year 2024 labwork7"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.11.2)
FetchContent_MakeAvailable(json)

add_subdirectory(lib)
add_subdirectory(bin)

find_library(PDCURSES_LIBRARY
        NAMES pdcurses
        HINTS "pdcurses")


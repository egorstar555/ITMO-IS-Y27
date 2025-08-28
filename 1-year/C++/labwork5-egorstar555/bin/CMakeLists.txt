add_executable(labwork5 main.cpp)

target_link_libraries(labwork5 PRIVATE array3d)
target_include_directories(labwork5 PUBLIC ${PROJECT_SOURCE_DIR})
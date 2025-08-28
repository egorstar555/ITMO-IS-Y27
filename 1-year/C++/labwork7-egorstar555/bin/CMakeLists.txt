add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC forecast)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

set(PDCURSES_DLL ${PROJECT_SOURCE_DIR}/pdcurses/pdcurses.dll)

add_custom_command(TARGET WeatherForecast POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PDCURSES_DLL}
        $<TARGET_FILE_DIR:WeatherForecast>)
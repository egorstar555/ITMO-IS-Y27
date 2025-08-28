add_library(forecast Forecast.cpp Forecast.h
        WMOIcons.h
        CityRepository.h
        CityRepository.cpp)
target_link_libraries(
        forecast
        PUBLIC cpr::cpr
        PUBLIC nlohmann_json::nlohmann_json
        ${PDCURSES_LIBRARY}
)
target_include_directories(forecast PUBLIC ${PROJECT_SOURCE_DIR})
#pragma once

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "pdcurses/curses.h"

#include <iostream>
#include <vector>
#include <optional>
#include <cstdint>

using json = nlohmann::json;

static const int16_t kRowForDate = 9;
static const int16_t kColumnForDate = 125;
static const int16_t kEscape = 27;
static const int16_t kNkey = 110;
static const int16_t kNkeyRu = 1090;
static const int16_t kPkey = 112;
static const int16_t kPkeyRu = 1079;
static const int16_t kPluskey = 43;
static const int16_t kMinuskey = 95;
static const int16_t kMinDays = 1;

namespace Forecast {
    struct ForecastData {
        std::vector<std::string> time;
        std::vector<double> temperature_2m;
        std::vector<int> relativehumidity_2m;
        std::vector<double> apparent_temperature;
        std::vector<int> cloudcover;
        std::vector<double> windspeed_10m;
        std::vector<int> wind_direction_10m;
        std::vector<double> precipitation;
        std::vector<int> precipitation_probability;
        std::vector<int> weathercode;
        std::vector<double> visibility;
    };


    // модель хранит прогноз по городу
    class CityForecast {
    public:
        explicit CityForecast(json& forecast, std::string& city) {
            forecast_.time = forecast["hourly"]["time"].get<std::vector<std::string>>();
            forecast_.temperature_2m = forecast["hourly"]["temperature_2m"].get<std::vector<double>>();
            forecast_.relativehumidity_2m = forecast["hourly"]["relativehumidity_2m"].get<std::vector<int>>();
            forecast_.apparent_temperature = forecast["hourly"]["apparent_temperature"].get<std::vector<double>>();
            forecast_.cloudcover = forecast["hourly"]["cloudcover"].get<std::vector<int>>();
            forecast_.windspeed_10m = forecast["hourly"]["windspeed_10m"].get<std::vector<double>>();
            forecast_.wind_direction_10m = forecast["hourly"]["wind_direction_10m"].get<std::vector<int>>();
            forecast_.precipitation = forecast["hourly"]["precipitation"].get<std::vector<double>>();
            forecast_.precipitation_probability = forecast["hourly"]["precipitation_probability"].get<std::vector<int>>();
            forecast_.weathercode = forecast["hourly"]["weathercode"].get<std::vector<int>>();
            forecast_.visibility = forecast["hourly"]["visibility"].get<std::vector<double>>();
            city_ = city;
        };

        ForecastData GetForecast() {
            return forecast_;
        }

        std::string GetCity() {
            return city_;
        }

    private:
        ForecastData forecast_;
        std::string city_;
    };

    // представление отображает прогноз по городу
    class CityForecastView {
    public:
        virtual void ShowCityForecast(CityForecast&, int16_t) = 0;

        virtual ~CityForecastView() = default;
    };

    // текстовое отображение прогноза
    class TextCityForecastView : public CityForecastView {
    public:
        void ShowCityForecast(CityForecast&, int16_t) override;

        void DrawGrid(int16_t, int16_t, int16_t, int16_t);

        ~TextCityForecastView() override = default;
    };

    // Основной класс
    class WeatherForecast {
    public:
        void Show();

        void GetConfig();

        void GetForecast();

        ~WeatherForecast() {
            for (auto forecast: forecasts_) {
                delete forecast;
            }
        }

    private:
        std::vector<std::string> cities_;
        std::vector<CityForecast*> forecasts_;
        int16_t days_;
        int16_t frequency_;
    };
} // namespace Forecast

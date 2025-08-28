#include "Forecast.h"
#include "CityRepository.h"
#include "lib/WMOIcons.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void Forecast::WeatherForecast::Show() {
    GetConfig();
    GetForecast();

    int16_t tek_city = 0;
    CityForecast* city_forecast = forecasts_[tek_city];
    CityForecastView* city_forecast_view(new TextCityForecastView);

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    halfdelay(frequency_ * 10); // секунды в миллисекунды
    cbreak();

    int16_t win_row, win_col;
    getmaxyx(stdscr, win_row, win_col);
    if (win_row < kRowForDate || win_col < kColumnForDate) {
        printw("Set the minimum window size to 11 rows x 125 columns");
        refresh();
        getch();
        endwin();

        exit(1);
    }

    int16_t max_days = (win_row - 1) / (kRowForDate + 1);
    int16_t current_day = std::min(max_days, days_);
    int16_t ch;

    do {
        city_forecast_view->ShowCityForecast(*city_forecast, current_day);
        refresh();
        ch = getch();
        switch (ch) {
            case ERR: // не дождались, обновляем
                GetForecast();
                break;
            case kNkey: // Переключаться между городами с помощью клавиш "n", "p"
            case kNkeyRu:
                tek_city = (tek_city + 1) % cities_.size();
                city_forecast = forecasts_[tek_city];
                break;
            case kPkey: //  Переключаться между городами с помощью клавиш "n", "p"
            case kPkeyRu:
                if (--tek_city < 0) {
                    tek_city = cities_.size() - 1;
                }
                city_forecast = forecasts_[tek_city];
                break;
            case kPluskey:// Увеличиватьколичество дней прогноза по нажатие клавиш "+"
            case PADPLUS:
                current_day = std::min(max_days, ++current_day);
                break;
            case kMinuskey:// уменьшать количество дней прогноза по нажатие клавиш "-"
            case PADMINUS:
                current_day = std::max(--current_day, kMinDays);
                break;
            default:
                break;
        }
    } while (ch != kEscape);
    endwin();

    delete city_forecast_view;
};

void Forecast::WeatherForecast::GetForecast() {
    CityRepository* city_repository{new OpenMeteoCityRepository};
    forecasts_.clear();
    for (auto& city: cities_) {
        forecasts_.push_back(city_repository->GetByName(city));
    }

    delete city_repository;
}

void Forecast::WeatherForecast::GetConfig() {
    fs::path path = fs::current_path().parent_path();
    std::ifstream config(path / "Config/config.json");
    if (!config.is_open()) {
        std::cerr << "config.json not found" << "\n";

        exit(1);
    }
    json params = json::parse(config);
    if (params.empty()) {
        std::cerr << "config.json can`t be parse";

        exit(1);
    }
    cities_ = params["cities"].get<std::vector<std::string>>();
    days_ = params["days"];
    frequency_ = params["frequency"];
}

void Forecast::TextCityForecastView::DrawGrid(int16_t x, int16_t y, int16_t h, int16_t w) {
    // прямоугольник
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + w, ACS_URCORNER);
    mvaddch(y + h, x, ACS_LLCORNER);
    mvaddch(y + h, x + w, ACS_LRCORNER);
    mvhline(y, x + 1, ACS_HLINE, w - 1);
    mvhline(y + h, x + 1, ACS_HLINE, w - 1);
    mvvline(y + 1, x, ACS_VLINE, h - 1);
    mvvline(y + 1, x + w, ACS_VLINE, h - 1);
    // разделители вертикальные
    mvvline(y + 1, kColumnForDate / 4, ACS_VLINE, h - 1);
    mvvline(y + 1, (kColumnForDate / 4) * 2, ACS_VLINE, h - 1);
    mvvline(y + 1, (kColumnForDate / 4) * 3, ACS_VLINE, h - 1);
    // разделитель горизонтальный
    mvhline(y + 2, x + 1, ACS_HLINE, w - 1);

    mvprintw(y + 1, x + 1 + (30 - std::strlen("Morning")) / 2, "%s", "Morning");
    mvprintw(y + 1, x + 1 + kColumnForDate / 4 + (30 - std::strlen("Afternoon")) / 2, "%s", "Afternoon");
    mvprintw(y + 1, x + 1 + (kColumnForDate / 4) * 2 + (30 - std::strlen("Evening")) / 2, "%s", "Evening");
    mvprintw(y + 1, x + 1 + (kColumnForDate / 4) * 3 + (30 - std::strlen("Night")) / 2, "%s", "Night");
}

void Forecast::TextCityForecastView::ShowCityForecast(CityForecast& city_forecast, int16_t days) {
    clear();

    ForecastData forecast = city_forecast.GetForecast();

    int16_t y = 2;
    int16_t x = 0;
    int16_t w = kColumnForDate;
    int16_t h = kRowForDate - 1;
    int16_t n_forecast;

    mvprintw(y - 2, x, "Weather forecast for city: %s", city_forecast.GetCity().c_str());

    for (auto i = 0; i < days; ++i) {
        mvprintw(y - 1, (kColumnForDate - 10) / 2, "%s", forecast.time[5 + (i * 24)].substr(0, 10).c_str()); // дата

        for (auto j = 0; j < 4; ++j) {
            n_forecast = 5 + j * 6 + i * 24;
            DrawGrid(x, y,  h, w);
            auto wc = kWmoCodes.at(forecast.weathercode[n_forecast]);
            mvprintw(y + 3, x + 16 + (kColumnForDate / 4) * j, "%.*s", 15, wc.first.c_str());
            mvprintw(y + 4, x + 16 + (kColumnForDate / 4) * j, "%-3.f(%3.f) °C", forecast.temperature_2m[n_forecast],
                     forecast.apparent_temperature[n_forecast]);
            mvprintw(y + 5, x + 16 + (kColumnForDate / 4) * j, "%-2.f km/h", forecast.windspeed_10m[n_forecast]);
            mvprintw(y + 6, x + 16 + (kColumnForDate / 4) * j, "%-5.f m", forecast.visibility[n_forecast]);
            mvprintw(y + 7, x + 16 + (kColumnForDate / 4) * j, "%-3.1f mm | %d%%", forecast.precipitation[n_forecast],
                     forecast.precipitation_probability[n_forecast]);
            for (auto k = 0; k < 5; ++k) {
                mvprintw(y + 3 + k, x + 1 + (kColumnForDate / 4) * j, "%s", wc.second[k].c_str());
            }
        }
        y += kRowForDate + 1;
    }
}
#include "Forecast.h"
#include "CityRepository.h"

Forecast::CityForecast* OpenMeteoCityRepository::GetByName(std::string& city_name) {
    std::string API_key = "mRJ3e7vhzGF9id8kxiypAg==6YKMk9f61j8mt3fb";
    std::string url = "https://api.api-ninjas.com/v1/city";
    cpr::Response response = cpr::Get(
            cpr::Url{url},
            cpr::Header{{"X-Api-Key", API_key}},
            cpr::Parameters{{"name", city_name}}
    );

    if (response.status_code != 200) {
        std::cerr << "api.api-ninjas.com is not responding" << "\n";

        exit(1);
    }

    json city_params = json::parse(response.text);
    if (city_params.empty()) {
        std::cerr << city_name << " - city not found";

        exit(1);
    }

    city_params = city_params.get<std::vector<json>>()[0];
    std::string latitude = std::to_string(city_params["latitude"].get<double>());
    std::string longitude = std::to_string(city_params["longitude"].get<double>());
    response = cpr::Get(
            cpr::Url{"https://api.open-meteo.com/v1/forecast"},
            cpr::Parameters{
                    {"latitude",  latitude},
                    {"longitude", longitude},
                    {"timezone",  "GMT"},
                    {"hourly",    "temperature_2m,relativehumidity_2m,apparent_temperature,cloudcover"},
                    {"hourly",    "windspeed_10m,wind_direction_10m,precipitation"},
                    {"hourly",    "precipitation_probability,weathercode,visibility"},
            }
    );

    if (response.status_code != 200) {
        std::cerr << "api.open-meteo.com is not responding" << "\n";

        exit(1);
    }

    city_params = json::parse(response.text);
    Forecast::CityForecast* city{new Forecast::CityForecast(city_params, city_name)};

    return city;
}
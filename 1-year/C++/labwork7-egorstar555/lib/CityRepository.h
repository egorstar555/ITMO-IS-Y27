#pragma once

#include "Forecast.h"

// Определение интерфейса репозитория
class CityRepository {
public:
    virtual Forecast::CityForecast* GetByName(std::string&) = 0;

    virtual ~CityRepository() = default;
};

// Реализация репозитория с использованием Open Meteo
class OpenMeteoCityRepository : public CityRepository {
public:
    Forecast::CityForecast* GetByName(std::string&) override;

    ~OpenMeteoCityRepository() override = default;
};




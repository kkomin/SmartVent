#ifndef WEATHER_PARSE_H
#define WEATHER_PARSE_H

#include "weather_data.h"

void parse_weather_json(const char *json_str, WeatherData *weather_data);

#endif
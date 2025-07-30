#ifndef API_H
#define API_H

#include "weather_data.h"

void getApiKey(char *apiKey, size_t size);
int call_api(double lat, double lon, WeatherData *weather_data);

#endif
#ifndef AIR_PARSE_H
#define AIR_PARSE_H

#include "weather_data.h"

void parse_air_json(const char *json_str, AirData *air_data);

#endif
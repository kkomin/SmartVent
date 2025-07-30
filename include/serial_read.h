#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include "weather_data.h"

int read_serial_data(WeatherData *data);

void serial_write(char status);

#endif
#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include "weather_data.h"

// int read_serial_data(WeatherData *data);

// void serial_write(char status);

int open_serial_port(const char *port_name);

int read_serial_data(int fd, WeatherData *data);

void serial_write(char status);

void close_serial_port(int fd);

#endif
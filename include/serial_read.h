#ifndef SERIAL_READ_H
#define SERIAL_READ_H

#include "weather_data.h"

int open_serial_port(const char *port_name);

int read_serial_data(int fd, WeatherData *data);

void serial_write(char status);

void close_serial_port(int fd);

int read_line(int fd, char *buffer, size_t max_len);

#endif
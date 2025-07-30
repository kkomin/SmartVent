#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

typedef struct {
    float pm25;
    float pm10;
} AirData;

typedef struct {
    float tmp_in;
    int hum_in;
    float tmp_out;
    float hum_out;
    char weather_desc[50];
    char city[50];
    AirData air;
} WeatherData;

#endif
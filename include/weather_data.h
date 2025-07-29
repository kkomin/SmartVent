#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

typedef struct {
    float tmp_in;
    int hum_in;
    float tmp_out;
    float hum_out;
    char weather_desc[50];
    char city[50];
} WeatherData;

typedef struct {
    float pm25;
    float pm10;
} AirData;

#endif
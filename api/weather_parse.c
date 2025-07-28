#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

void parse_weather_json(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if(!root) {
        printf("Weather JSON 파싱 오류\n");
        return;
    }

    // main 객체 온습도 가져오기 (temp, humidity)
    cJSON *main_obj = cJSON_GetObjectItem(root, "main");
    if(main_obj) {
        double tmp_k = cJSON_GetObjectItem(main_obj, "temp") -> valuedouble;
        // 켈빈 -> 섭씨로 변경
        double tmp_c = tmp_k - 273.15;

        int humidity = cJSON_GetObjectItem(main_obj, "humidity") -> valueint;

        printf("현재 온도 : %.2f ℃\n", tmp_c);
        printf("현재 습도 : %d\n", humidity);
    }

    // weather에서 description
    cJSON *weather_arr = cJSON_GetObjectItem(root, "weather");
    if(weather_arr && cJSON_GetArraySize(weather_arr) > 0) {
        cJSON *first_weather = cJSON_GetArrayItem(weather_arr, 0);
        char *description = cJSON_GetObjectItem(first_weather, "description")->valuestring;
        printf("날씨 상태 : %s\n", description);
    }

    // 도시 이름 출력
    char *city = cJSON_GetObjectItem(root, "name")->valuestring;
    printf("도시 : %s\n", city);

    cJSON_Delete(root);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

void parse_weather_json(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if(!root) {
        printf("JSON 파싱 오류\n");
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

    // 도시 이름 출력
}

int main() {
    return 0;
}
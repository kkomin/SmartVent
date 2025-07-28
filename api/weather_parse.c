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


    // weather에서 description

    // 도시 이름 출력
}

int main() {
    return 0;
}
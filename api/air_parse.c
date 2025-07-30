#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "weather_data.h"
#include "air_parse.h"

void parse_air_json(const char *json_str, AirData *air_data) {
    // json 파싱
    cJSON *root = cJSON_Parse(json_str);
    if(!root) {
        printf("Air JSON 파싱 오류\n");
        return;
    }

    // list 배열에서 첫 번째 요소 가져오기
    cJSON *list = cJSON_GetObjectItem(root, "list");
    if(list && cJSON_GetArraySize(list) > 0) {
        // 해당 요소의 components 객체 추출
        cJSON *first_list = cJSON_GetArrayItem(list, 0);
        cJSON *components = cJSON_GetObjectItem(first_list, "components");
        
        if(components) {
            // components 내 pm2_5, pm10 값 읽기
            double pm2_5 = cJSON_GetObjectItem(components, "pm2_5") -> valuedouble;
            double pm10 = cJSON_GetObjectItem(components, "pm10") -> valuedouble;

            // 파싱한 값 구조체 변수에 저장
            air_data->pm25 = (float)pm2_5;
            air_data->pm10 = (float)pm10;

            // 읽은 값 변수에 저장하거나 출력
            printf("초미세먼지 : %.2f\n", pm2_5);
            printf("미세먼지 : %.2f\n", pm10);
        }
    }
    // json 메모리 해제
    cJSON_Delete(root);
}
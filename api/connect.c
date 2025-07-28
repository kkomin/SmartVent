#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"
#include "weather_parse.h"
#include "air_parse.h"

// api 응답 데이터 저장할 구조체 정의
struct Memory {
    char *response;     // 응답 데이터 저장할 문자열
    size_t size;        // 현재 데이터 크기
};

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t totalSize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + totalSize + 1);

    if(ptr == NULL) {
        // 메모리 할당 실패
        printf("메모리 부족\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, totalSize);
    mem->size += totalSize;
    mem->response[mem->size] = '\0';

    return totalSize;
}

int fetch_api_data(const char *url, char **response_out) {
    // curl 핸들 생성 -> 요청 준비 단계
    CURL *curl = curl_easy_init();                    // 요청 객체 생성
    if(!curl) {
        fprintf(stderr, "curl 초기화 실패\n");
        return -1;
    }

    struct Memory chunk;    // 응답 저장 구조체
    chunk.response = malloc(1);
    chunk.size = 0;
    chunk.response[0] = '\0';

    // 요청 옵션 설정 (요청할 url 지정)
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // SSL 인증 일단 무시 ㄱㄱ
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    // 응답 출력 -> 콜백 함수 지정
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    // 콜백 함수에 전달할 데이터를 지정
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // http 요청 실행
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "요청 실패 : %s\n", curl_easy_strerror(res));
        free(chunk.response);
        return -1;
    }

    *response_out = chunk.response; // 호출자에게 메모리 넘김(해제 책임 호출자)
    return 0;
}

int main() {
    // api url 설정
    char api_key[128];
    getApiKey(api_key, sizeof(api_key));
    
    double lat = 36.336728;
    double lon = 127.458998;

    char weather_url[256];
    char air_url[256];

    char *response = NULL;

    snprintf(weather_url, sizeof(weather_url), "https://api.openweathermap.org/data/2.5/weather?lat=%f&lon=%f&appid=%s", lat, lon, api_key);

    snprintf(air_url, sizeof(air_url), "http://api.openweathermap.org/data/2.5/air_pollution?lat=%f&lon=%f&appid=%s", lat, lon, api_key);

    // 날씨 데이터 호출 및 파싱
    if(fetch_api_data(weather_url, &response) == 0) {
        printf("날씨 데이터 응답 : %s\n", response);
        parse_weather_json(response);
        free(response);
    }

    // 미세먼지 데이터 호출 및 파싱
    if(fetch_api_data(air_url, &response) == 0) {
        printf("미세먼지 데이터 응답 : %s\n", response);
        parse_air_json(response);
        free(response);
    }
        
    return 0;
}
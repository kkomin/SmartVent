#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"
#include "weather_parse.h"

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

int main() {
    CURL *curl;             // CURL 핸들
    CURLcode res;           // CURL 요청 결과
    struct Memory chunk;    // 응답 저장 구조체
    chunk.response = malloc(1);
    chunk.size = 0;
    chunk.response[0] = '\0';
    
    // libcurl 초기화
    curl_global_init(CURL_GLOBAL_DEFAULT);      // curl 라이브러리 전체 초기화
    
    // curl 핸들 생성 -> 요청 준비 단계
    curl = curl_easy_init();                    // 요청 객체 생성

    // api url 설정
    char api_key[128];
    memset(api_key, 0, sizeof(api_key));
    
    getApiKey(api_key, sizeof(api_key));
    
    if(curl) {
        const char *api = api_key;
        // 위도, 경도 설정 (대전대 인사관)
        double lat = 36.336728;
        double lon = 127.458998;
        char url[256];

        snprintf(url, sizeof(url), "https://api.openweathermap.org/data/2.5/weather?lat=%f&lon=%f&appid=%s", lat, lon, api);
        printf("요청할 URL: %s\n", url);
        
        // 요청 옵션 설정 (요청할 url 지정)
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // SSL 인증 일단 무시 ㄱㄱ
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // 응답 출력 -> 콜백 함수 지정
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // 콜백 함수에 전달할 데이터를 지정
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // http 요청 실행
        res = curl_easy_perform(curl);

        // 요청 결과 확인
        if(res != CURLE_OK) {
            fprintf(stderr, "요청 실패 : %s\n", curl_easy_strerror(res));
        } else {
            printf("응답 데이터 : %s\n", chunk.response);

            parse_weather_json(chunk.response);
        }

        // 메모리 정리
        free(chunk.response);

        // curl 핸들 정리
        curl_easy_cleanup(curl);
    }

    // libcurl 종료
    curl_global_cleanup();

    return 0;
}
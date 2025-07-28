#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// api 응답 데이터 저장할 구조체 정의
struct Memory {
    char *response;     // 응답 데이터 저장할 문자열
    size_t size;        // 현재 데이터 크기
};

int main() {
    CURL *curl;             // CURL 핸들
    CURLcode res;           // CURL 요청 결과
    struct Memory chunk;    // 응답 저장 구조체

    // 메모리 초기화
    chunk.response = malloc(1);
    chunk.size = 0;

    // libcurl 초기화
    curl_global_init(CURL_GLOBAL_DEFAULT);      // curl 라이브러리 전체 초기화
    curl = curl_easy_init();                    // 요청 객체 생성

    // api url 설정

    // 요청 옵션 설정


    // libcurl 종료

    return 0;
}
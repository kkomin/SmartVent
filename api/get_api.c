#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

void getApiKey(char *apiKey, size_t size) {
    FILE *file = fopen("config.txt", "r");

    if(!file) {
        perror("API 키 파일을 열 수 없습니다.");
        exit(1);
    }

    char buffer[128];
    while(fgets(buffer, sizeof(buffer), file)) {
        // 파일 내용이 API_KEY로 시작하는지 판별
        if(strncmp(buffer, "API_KEY=", 8) == 0) {
            strncpy(apiKey, buffer + 8, size - 1);
            apiKey[strcspn(apiKey, "\n")] = 0;
            break;
        }
    }

    fclose(file);
}
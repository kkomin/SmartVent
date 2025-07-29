#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_db_config(char *host, char *user, char *pass, char *db, unsigned int *port) {
    FILE *file = fopen("db_config.txt", "r");
    if (!file) {
        perror("DB 설정 파일 열기 실패");
        exit(1);
    }

    char key[32], value[128];
    // 전까지 최대 31자까지 읽어서 key에 저장
    // 문자가 있는지 확인하고 넘김
    // 공백 전까지 최대 127자 읽어서 value에 저장
    while (fscanf(file, "%31[^=]=%127s\n", key, value) == 2) {
        if (strcmp(key, "DB_HOST") == 0) {
            strncpy(host, value, 99);
        } else if (strcmp(key, "DB_USER") == 0) {
            strncpy(user, value, 99);
        } else if (strcmp(key, "DB_PASS") == 0) {
            strncpy(pass, value, 99);
        } else if (strcmp(key, "DB_NAME") == 0) {
            strncpy(db, value, 99);
        } else if (strcmp(key, "DB_PORT") == 0) {
            *port = (unsigned int)atoi(value);
        }
    }

    fclose(file);
}

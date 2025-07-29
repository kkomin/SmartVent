#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "db/connect_db.h"
#include "db/get_db_config.h"
#include "include/api.h"
#include "include/timestamp.h"

void main() {
    // DB 연결 -> 성공 여부 확인
    connect_db();

    // 위치 정보 설정 -> 위도, 경도 값 변수 할당 -> 정해진 위도 경도 값 수정
    double lat = 37.5665;  // 예시: 서울의 위도
    double lon = 126.978;  // 예시: 서울의 경도
    call_api(lat, lon);

    // 현재 시간 정보 생성 -> DB에 저장할 거
    char timestamp[20];
    get_current_timestamp(timestamp, sizeof(timestamp));
    
    // 9. DB 저장 -> 파싱한 데이터, 타임스탬프 -> environment_data 테이블에 저장

    // 10. 시스템 로그 저장 -> 저장 성공 여부 및 동작 로그 -> system_logs 테이블에 기록

    // 11. DB 연결 종료 -> 자원 해제
    mysql_close(conn);
    printf("DB 연결 종료\n");

    // 12. 메모리 정리 -> 동적 메모리 해제
    
    return 0;
}
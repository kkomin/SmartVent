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

    // DB 저장 -> 파싱한 데이터, 타임스탬프 -> environment_data 테이블에 저장
    // MYSQL *conn, const char *timestamp, float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10, const char *weather_desc, int vent_status
    
    vent_status = 1; // 예시: 환기 상태 1 (환기 중) -> 추후 조건에 따른 환기 정보로 변수 변경될 예정 (여기서 삭제 예정)
    save_environment_data(conn, timestamp, weather_data.tmp_in, weather_data.hum_in, weather_data.tmp_out, weather_data.hum_out, air_data.pm25, air_data.pm10, weather_data.weather_desc, vent_status);

    // 10. 시스템 로그 저장 -> 저장 성공 여부 및 동작 로그 -> system_logs 테이블에 기록

    // 11. DB 연결 종료 -> 자원 해제
    mysql_close(conn);
    printf("DB 연결 종료\n");

    // 12. 메모리 정리 -> 동적 메모리 해제
    
    return 0;
}
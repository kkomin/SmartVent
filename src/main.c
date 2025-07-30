#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mariadb/mysql.h>
#include "db/connect_db.h"
#include "db/get_db_config.h"
#include "include/api.h"
#include "include/timestamp.h"
#include "weather_data.h"
#include "ventilation.h"

MYSQL *conn;

void main() {
    // DB 연결 -> 성공 여부 확인
    conn = connect_db();

    WeatherData data;

    // 위치 정보 설정 -> 위도, 경도 값 변수 할당 -> 정해진 위도 경도 값 수정
    double lat = 37.5665;  // 예시: 서울의 위도
    double lon = 126.978;  // 예시: 서울의 경도

    while(1) {
        // 현재 시간 정보 생성 -> DB에 저장할 거
        char timestamp[20];
        get_current_timestamp(timestamp, sizeof(timestamp));
        
        // API 호출 -> 날씨, 대기질 정보 가져오기
        call_api(lat, lon);
        
        // DB 저장 -> 파싱한 데이터, 타임스탬프 -> environment_data 테이블에 저장
        // MYSQL *conn, const char *timestamp, float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10, const char *weather_desc, int vent_status
        if(read_serial_data(&data) == 0) {
            int vent_status = auto_vent(data.tmp_in, data.hum_in, data.tmp_out, data.hum_out, data.air.pm25, data.air.pm10);
            
            // 로그 메세지용 변수 선언
            char log_message[256];

            int data_id = save_environment_data(conn, timestamp, weather_data.tmp_in, weather_data.hum_in, weather_data.tmp_out, weather_data.hum_out, air_data.pm25, air_data.pm10, weather_data.weather_desc, vent_status);

            if (data_id > 0) {
                // 성공 시 log 저장
                snprintf(log_message, sizeof(log_message), "데이터 저장 완료 (vent_status : %d)", vent_status);
                // 시스템 로그 저장 -> 저장 성공 여부 및 동작 로그 -> system_logs 테이블에 기록
                save_system_log(conn, timestamp, log_message, "INFO", "environment_data", data_id);
            } else {
                // 실패 시 log 저장
                save_system_log(conn, timestamp, log_message, "ERROR", "environment_data", -1);
            }
                

            // 아두이노에 vent_status 신호 전송 (LED 제어)
            serial_write(vent_status == 1 ? '1' : '0');
        }

        // 5분(300초) 대기
        sleep(300);
    }

    // DB 연결 종료 -> 자원 해제
    mysql_close(conn);
    printf("DB 연결 종료\n");
    
    return 0;
}
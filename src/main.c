#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mariadb/mysql.h>
#include "connect_db.h"
#include "get_db.h"
#include "save_db.h"
#include "api.h"
#include "time_util.h"
#include "weather_data.h"
#include "ventilation.h"
#include "serial_read.h"

extern MYSQL *conn;

int main() {
    // DB 연결 -> 성공 여부 확인
    conn = connect_db();

    WeatherData data;

    // 시리얼 포트 열기 (아두이노 연결 포트)
    int serial_fd = open_serial_port("/dev/ttyACM0");
    if(serial_fd < 0) {
        fprintf(stderr, "시리얼 포트 열기 실패. 프로그램 종료\n");
        mysql_close(conn);
        return 1;
    }

    // 위치 정보 설정 -> 위도, 경도 값 변수 할당 -> 정해진 위도 경도 값 수정
    double lat = 36.336789;  // 대전대학교 인문사회관 경도
    double lon = 127.458976;  // 대전대학교 인문사회관 위도

    while(1) {
        // 현재 시간 정보 생성 -> DB에 저장할 거
        char timestamp[20];
        get_current_timestamp(timestamp, sizeof(timestamp));

        WeatherData apiData;
        
        // API 호출 -> 날씨, 대기질 정보 가져오기
        // call_api(lat, lon);
        if (call_api(lat, lon, &apiData) != 0) {
        fprintf(stderr, "[ERROR] API 호출 실패\n");
        sleep(300);
        continue;
        }

        // 외부 데이터 복사
        data.tmp_out = apiData.tmp_out;
        data.hum_out = apiData.hum_out;
        data.air.pm25 = apiData.air.pm25;
        data.air.pm10 = apiData.air.pm10;
        strcpy(data.weather_desc, apiData.weather_desc);
        
        // DB 저장 -> 파싱한 데이터, 타임스탬프 -> environment_data 테이블에 저장
        // MYSQL *conn, const char *timestamp, float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10, const char *weather_desc, int vent_status
        if(read_serial_data(serial_fd, &data) == 0) {
            int vent_status = auto_vent(data.tmp_in, data.hum_in, data.tmp_out, data.hum_out, data.air.pm25, data.air.pm10);
            
            // 디버깅 출력
            // printf("[DEBUG] 시리얼 데이터 - tmp_in: %.2f, hum_in: %.2f, tmp_out: %.2f, hum_out: %.2f\n",
            // data.tmp_in, data.hum_in, data.tmp_out, data.hum_out);
            
            // 로그 메세지용 변수 선언
            char log_message[256];

            // 데이터 저장 -> DB에 저장
            int data_id = save_environment_data(conn, timestamp, data.tmp_in, data.hum_in, data.tmp_out, data.hum_out, data.air.pm25, data.air.pm10, data.weather_desc, vent_status);
            
            // 저장 쿼리와 결과 확인용 출력
            printf("[DEBUG] save_environment_data 반환값: %d\n", data_id);

            printf("[DATA] 실내 온도 : %.2f, 실내 습도 : %.2f\n", data.tmp_in, data.hum_in);
            printf("[DATA] 실외 온도 : %.2f, 실외 습도 : %.2f\n", data.tmp_out, data.hum_out);
            printf("[DATA] 미세먼지 : %.2f, 초미세먼지 : %.2f\n", data.air.pm10, data.air.pm25);

            if (vent_status == 1) {
                printf("[VENT] 환기 상태: ON (LED 켜짐)\n");
            } else {
                printf("[VENT] 환기 상태: OFF (LED 꺼짐)\n");
            }

            if (data_id > 0) {
                // 성공 시 log 저장
                snprintf(log_message, sizeof(log_message), "%s", vent_status==1 ? "환기 시작" : "환기 정지");
                
                // 시스템 로그 저장 -> 저장 성공 여부 및 동작 로그 -> system_logs 테이블에 기록
                save_system_log(conn, timestamp, log_message, "INFO", "environment_data", data_id);
                printf("[INFO] %s\n", log_message); // 성공 메시지 출력
            } else {
                // 실패 시 log 저장
                save_system_log(conn, timestamp, log_message, "ERROR", "environment_data", -1);
                fprintf(stderr, "[ERROR] %s\n", log_message); // 실패 메시지 출력
            }
                
            // 아두이노에 vent_status 신호 전송 (LED 제어)
            serial_write(vent_status == 1 ? '1' : '0');

        } else {
            fprintf(stderr, "[ERROR] 시리얼 데이터 읽기 실패\n");
        }

        // 5분(300초) 대기
        sleep(300);
    }

    // 시리얼 포트 닫기
    close_serial_port(serial_fd);

    // DB 연결 종료 -> 자원 해제
    mysql_close(conn);
    printf("DB 연결 종료\n");
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termious.h>
#include <unistd.h>
#include "weather_data.h"

#define SERIAL_PROT ttyUSB0     // 아두이노가 연결된 포트 -> 내일 다시 확인 후 수정

int read_serial_data(WeatherData *data) {
    int serial_port;
    struct termios tty;
    char buffer[256];

    // 시리얼 데이터 받아오기 (시리얼 포트 열기)
    serial_port = open(SERIAL_PROT, O_RDWR | Q_NOCTTY );
    if(serial_port < 0) {
        perror("시리얼 포트 열기 실패\n");
        return -1;
    }
    
    // 시리얼 통신 속성 설정
    memset(&tty, 0, sizeof tty);                // 구조체 초기화
    tcgetattr(serial_port, &tty);              // 현재 설정 가져오기
    cfsetispeed(&tty, B9600);                  // 입력 속도 설정
    cfsetospeed(&tty, B9600);                  // 출력 속도 설정

    tty.c_cflag |= (CLOCAL | CREAD);           // 포트 활성화 및 읽기 설정
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                        // 데이터 비트 8비트
    tty.c_cflag &= ~PARENB;                    // 패리티 비트 없음
    tty.c_cflag &= ~CSTOPB;                    // 스톱 비트 1비트
    tty.c_cflag &= ~CRTSCTS;                   // 하드웨어 흐름제어 사용 안함

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 캐논모드/에코/신호 처리 비활성화
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // 소프트웨어 흐름 제어 비활성화
    tty.c_oflag &= ~OPOST;                          // 출력 처리 없음

    tcsetattr(serial_port, TCSANOW, &tty);     // 설정 즉시 적용

    // 데이터 수신
    memset(buffer, 0, sizeof buffer);
    int n = read(serial_port, buffer, sizeof buffer - 1);

    if(n>0) {
        // 문자열 파싱
        float tmp_in, hum_in;
        if(scanf(buffer, "%f, %f", &tmp_in, &hum_in) == 2) {
            // weatherdata.h의 구조체에 tmp_in, hum_in 저장
            data->tmp_in = tmp_in;
            data->hum_in = hum_in;
            // 시리얼 포트 닫기
            close(serial_port);
            return 0;
        }
    }
    
    close(serial_port);
    return -1;
}
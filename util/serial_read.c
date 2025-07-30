#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "weather_data.h"

#define SERIAL_PROT "/dev/ttyACM0"     // 아두이노가 연결된 포트

// 시리얼 포트 열기 및 설정
int open_serial_port(const char *port_name) {
    int fd = open(port_name, O_RDWR | O_NOCTTY);
    if(fd < 0) {
        perror("시리얼 포트 열기 실패");
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if(tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr 실패");
        close(fd);
        return -1;
    }

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cc[VMIN] = 0;      // 최소 0 바이트 읽기 (비차단)
    tty.c_cc[VTIME] = 10;    // 1초(10*0.1s) 타임아웃

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    if(tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr 실패");
        close(fd);
        return -1;
    }

    return fd;
}


// 시리얼 포트에서 온습도 데이터 읽기
int read_serial_data(int fd, WeatherData *data) {
    char buffer[256];
    int total_read = 0;

    while (1) {
        int n = read(fd, buffer + total_read, sizeof(buffer) - 1 - total_read);
        if (n < 0) {
            perror("시리얼 읽기 오류");
            return -1;
        } else if (n == 0) {
            // 읽을 데이터 없음, 타임아웃 등
            break;
        }
        total_read += n;
        buffer[total_read] = '\0';

        // 개행 문자로 데이터 완성 판단 (줄 끝)
        if (strchr(buffer, '\n') != NULL) {
            break;
        }
        // 너무 길면 잘라내기
        if (total_read >= sizeof(buffer) - 1) {
            break;
        }
    }

    // '\r'과 '\n' 제거
    char *ptr = buffer;
    while (*ptr) {
        if (*ptr == '\r' || *ptr == '\n') {
            *ptr = '\0';
            break;
        }
        ptr++;
    }

    float tmp_in, hum_in;
    if (sscanf(buffer, "%f, %f", &tmp_in, &hum_in) == 2) {
        data->tmp_in = tmp_in;
        data->hum_in = hum_in;
        return 0;
    }

    printf("[DEBUG] 시리얼 원본 데이터: %s\n", buffer);

    return -1;
}



// 시리얼 포트에 상태 전송 (환기 상태 ON/OFF)
void serial_write(char status) {
    int serial_port = open(SERIAL_PROT, O_RDWR | O_NOCTTY );
    if (serial_port < 0) {
        perror("시리얼 포트 열기 실패\n");
        return;
    }

    // 시리얼 포트 속성 설정 (read와 동일하게 적용)
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    tcgetattr(serial_port, &tty);
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;
    tcsetattr(serial_port, TCSANOW, &tty);

    // 1바이트 전송
    write(serial_port, &status, 1);
    close(serial_port);
}

// 시리얼 포트 닫기
void close_serial_port(int fd) {
    close(fd);
}

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
    tcsetattr(fd, TCSANOW, &tty);

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

int read_line(int fd, char *buffer, size_t max_len) {
    size_t idx = 0;
    char ch;
    while (idx < max_len - 1) {
        int n = read(fd, &ch, 1);
        if (n == 1) {
            if (ch == '\n') break;
            if (ch != '\r') buffer[idx++] = ch;
        } else if (n == 0) {
            break;  // 타임아웃
        } else {
            perror("read error");
            return -1;
        }
    }
    buffer[idx] = '\0';
    return idx;
}


// 시리얼 포트에서 온습도 데이터 읽기
int read_serial_data(int fd, WeatherData *data) {
    char buffer[256] = {0};
    int len = read_line(fd, buffer, sizeof(buffer));
    if (len <= 0) return -1;

    printf("[DEBUG] 수신 문자열: %s\n", buffer);

    if (sscanf(buffer, "%f, %f", &data->tmp_in, &data->hum_in) == 2 || 
        sscanf(buffer, "%f,%f", &data->tmp_in, &data->hum_in) == 2) {
        return 0;
    }
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

# 컴파일러 및 옵션
CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lmariadb -lcjson -lcurl

# 실행 파일 이름
TARGET = smartvent

# 소스 파일 목록
SRC = \
    src/main.c \
    src/ventilation.c \
    api/air_parse.c \
    api/connect.c \
    api/get_api.c \
    api/weather_parse.c \
    db/connect_db.c \
    db/get_db.c \
    db/save_db.c \
    util/serial_read.c \
    util/time_util.c

# 오브젝트 파일 목록
OBJ = $(SRC:.c=.o)

# 기본 빌드 명령
all: $(TARGET)

# 실행 파일 빌드
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 개별 오브젝트 파일 빌드 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 정리
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

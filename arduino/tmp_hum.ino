#include <DHT.h>

// DHT 센서 핀 번호 및 종류 정의
#define DHTTYPE DHT11
#define DHTPIN 2
#define LEDPIN 13

// 객체 및 변수 초기화
DHT dht(DHTPIN, DHTTYPE);    // DHT 센서 객체 생성

void setup() {
    // Serial 통신 시작
    Serial.begin(9600);
    // DHT 센서 시작
    dht.begin();
    // LED 핀을 출력으로 설정
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);  // 초기 LED OFF
}

void loop() {
    // DHT 센서로부터 온도와 습도 읽기
    float tmp = dht.readTemperature();
    float hum = dht.readHumidity();

    if(isnan(tmp) || isnan(hum)) {
        Serial.println("DHT11 센서 오류 발생");
    } else {
        // 온도와 습도를 Serial로 전송
        Serial.print(tmp, 2);
        Serial.print(", ");
        Serial.print(hum, 2);
    }
    
    // 라즈베리파이에서 auto_vent()함수로 판단된 결과 전송
    // 1이면 환기 상태 ON, 0이면 OFF
    // Serial로 수신 처리 -> 환기 상태 수신 시
    if(Serial.available() > 0) {
        char state = Serial.read();
        if(state == '1') {
            digitalWrite(LEDPIN, HIGH);     //  LED ON
        } else if (state == '0') {
            digitalWrite(LEDPIN, LOW);      // LED OFF
        }
    }
    
    // 일정 시간 (5분마다 측정 및 상태 확인)
    delay(300000);
}
#include <stdio.h>

// 미세먼지 상태 판단
int air_quality(float pm25, float pm10) {
}

// 자동 환기 판단
int auto_vent(float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10) {
    // 환기 권장
    int need_vent = (tmp_in >= 28.0 || hum_in > 70.0);

    // 환기 가능
    int can_vent = (tmp_out < tmp_in) && air_quality(pm25, pm10);

    return 0; // 자동 환기 불필요
}
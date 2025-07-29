#include <stdio.h>

// 초미세먼지 상태 판단
int pm25_level(float pm25) {
    if(pm25 < 10) return 1;         // 좋음
    else if(pm25 < 25) return 2;    // 양호
    else if(pm25 < 50) return 3;    // 보통
    else if(pm25 < 75) return 4;    // 나쁨
    else return 5;                  // 매우 나쁨
}

// 미세먼지 상태 판단
int pm10_level(float pm10) {
    if(pm10 < 20) return 1;         // 좋음
    else if(pm10 < 50) return 2;    // 양호
    else if(pm10 < 100) return 3;   // 보통
    else if(pm10 < 200) return 4;   // 나쁨
    else return 5;                  // 매우 나쁨
}

// 공기질 상태 판단 (초미세먼지, 미세먼지)
int air_quality(float pm25, float pm10) {
    int level25 = pm25_level;
    int level10 = pm10_level;

    // 수치 중 하나라도 나쁨(4) 이상이면 환기 제한
    if(level25 <= 3 && level10 <= 3) {
        return 1;       // 환기 가능
    } else {
        return 0;       // 환기 불가
    }
}

// 자동 환기 판단 (실내 온습도 불쾌 -> need_vent, 실외 온습도/미세먼지 -> can_vent)
int auto_vent(float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10) {
    // 환기 권장
    int need_vent = (tmp_in >= 28.0 || hum_in > 70.0);
    
    // 환기 가능
    int can_vent = (tmp_out < tmp_in) && air_quality(pm25, pm10);
    
    // 두 조건 다 만족 -> return 1 (ON) / 아니면 return 0 (OFF)
    if(need_vent && can_vent) {
        return 1;
    } else {
        return 0; // 자동 환기 불필요
    }
}
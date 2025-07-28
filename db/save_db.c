#include <stdio.h>
#include <mysql/mysql.h>

int save_environment_data(MYSQL *conn, const char *timestamp, float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10, const char *weather_desc, int vent_status) {
    char query[1024];
    snprintf(query, sizeof(query),
    "INSERT INTO environment_data (timestamp, tmp_in, hum_in, tmp_out, hum_out, pm25, pm10, weather_description, vent_status) "
    "VALUES ('%s', %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, '%s', %d)",
    timestamp, tmp_in, hum_in, tmp_out, hum_out, pm25, pm10, weather_desc, vent_status);

    if(mysql_query(conn, query)) {
        fprintf(stderr, "쿼리 실행 실패 : %s\n", mysql_error(conn));
        return -1;
    }
    return 0;
}
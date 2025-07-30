#include <stdio.h>
#include <mariadb/mysql.h>

int save_environment_data(MYSQL *conn, const char *timestamp, float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10, const char *weather_desc, int vent_status) {
    char query[1024];

    printf("실행쿼리 : %s\n", query);

    if(mysql_query(conn, query)) {
        fprintf(stderr, "쿼리 실행 실패 : %s\n", mysql_error(conn));
        return -1;
    }
    // 반환 -> 삽입된 데이터의 id
    return (int)mysql_insert_id(conn);
}

// 어떤 환경 데이터가 저장되었는지 확인할 수 있어야 함
int save_system_log(MYSQL *conn, const char *timestamp, const char *log_message,  const char *log_level, const char *related_table, int related_id) {
    char query[1024];

    if(mysql_query(conn, query)) {
        fprintf(stderr, "쿼리 실행 실패 : %s\n", mysql_error(conn));
        return -1;
    }
    return 0;
}
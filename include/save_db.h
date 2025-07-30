#ifndef SAVE_DB_H
#define SAVE_DB_H

#include <mariadb/mysql.h>

int save_environment_data(MYSQL *conn, const char *timestamp, float tmp_in, float hum_in, float tmp_out, float hum_out, float pm25, float pm10, const char *weather_desc, int vent_status);

int save_system_log(MYSQL *conn, const char *timestamp, const char *log_message,  const char *log_level, const char *related_table, int related_id);

#endif

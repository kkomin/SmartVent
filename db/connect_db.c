#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_db.h"

MYSQL *conn;

MYSQL *connect_db() {
    char host[100], user[100], pass[100], db[100];
    unsigned int port = 0;

    // db 정보 가져오기
    get_db_config(host, user, pass, db, &port);

    conn = mysql_init(NULL);
    if(conn == NULL) {
        fprintf(stderr, "mysql init() 실패\n");
        exit(1);
    }

    if(mysql_real_connect(conn, host, user, pass, db, port, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() 실패 : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    printf("db 연결 성공!\n");
    return conn;
}
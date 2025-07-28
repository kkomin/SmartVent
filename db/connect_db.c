#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

MySQL *conn;

void connect_db() {
    conn = mysql_init(NULL);
    if(conn == NULL) {
        fprintf(stderr, "mysql init() 실패\n");
        exit(1);
    }

    if(mysql_real_connect(conn, "localhost", "root", "비밀번호", "SmartVent", 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() 실패 : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    printf("연결 성공!\n");
}
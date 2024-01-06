#include "mysql/mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql_config.h"

const char *host = "49.234.15.205";
const char *username = "root";
const char *password = "Lird2002";
const char *database = "test";
const char *port = "3308";
const char *testSql =
    "INSERT INTO orders (customer_name, total_amount) values ('lird', 10)";

int main(int argc, char *argv[]) {
    if (mysql_library_init(0, NULL, NULL)) {
        fprintf(stderr, "Can not init mysql client lib\n");
        exit(1);
    }
    printf("ConnectionPool\n");
    MYSQL mysql, *connection;
    MYSQL_RES *result;
    mysql_init(&mysql);

    char *portEndPtr;
    long portNum = strtol(port, &portEndPtr, 10);
    if (*portEndPtr != '\0') {
        fprintf(stderr, "Port Err\n");
        exit(1);
    }
    connection = mysql_real_connect(&mysql, host, username, password, database,
                                    portNum, NULL, 0);
    if (connection == NULL) {
        fprintf(stderr, "Failed to connect to database: Error: %s\n",
                mysql_error(&mysql));
        exit(1);
    }

    if(mysql_real_query(&mysql, testSql, strlen(testSql))){
        fprintf(stderr, "Failed to query: Error: %s\n",
                mysql_error(&mysql));
        mysql_close(connection);
        exit(1);
    } else {
        result = mysql_store_result(&mysql);
        
        if (result) {
            
        }
    }


    mysql_free_result(result);
    mysql_close(connection);
    mysql_library_end();
    return EXIT_SUCCESS;
}
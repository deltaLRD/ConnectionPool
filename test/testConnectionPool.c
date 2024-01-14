#include "connection_pool.h"
#include "mysql_config.h"
#include <mysql/mysql.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

MySQLConfig config;

void *worker_thread(void *arg) {
    ConnectionPool *pool = (ConnectionPool *)arg;
    MYSQL *connection = get_connection(pool);
    if (connection != NULL) {
        if (mysql_query(connection, "SELECT * FROM orders")) {
            fprintf(stderr, "Error exec query: %s\n", mysql_error(connection));
            exit(1);
        } else {
            MYSQL_RES *result = mysql_store_result(connection);
            if (result != NULL) {
                int num_fields = mysql_num_fields(result);
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(result))) {
                    for(int i = 0; i < num_fields; i++) {
                        printf("%s ", row[i] ? row[i] : "NULL");
                    }
                    printf("\n");
                }
                mysql_free_result(result);
            }
        }
        release_connection(pool, connection);
    }
    pthread_exit(NULL);
}

int main() {
    config.host = "49.234.15.205";
    config.username = "root";
    config.password = "Lird2002";
    config.database = "test";
    config.port = 3308;
    int max_connections = 10;
    ConnectionPool pool;
    init_pool(&pool, max_connections, config);
    pthread_t threads[max_connections];
    for (int i = 0 ; i < max_connections; i++) {
        pthread_create(&threads[i], NULL, worker_thread, (void *)&pool);
    }
    for (int i=0; i<max_connections; i++) {
        pthread_join(threads[i], NULL);
    }
    destroy_pool(&pool);
    return 0;
}
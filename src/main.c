#include "connection_pool.h"
#include "csv_reader.h"
#include "mysql/mysql.h"
#include "mysql_config.h"
#include <bits/pthreadtypes.h>
#include <mysql/field_types.h>
#include <mysql/mysql_time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

const char *host = "49.234.15.205";
const char *username = "root";
const char *password = "Lird2002";
const char *database = "test";
const char *port = "3308";
const char *testSql =
    "INSERT INTO orders (customer_name, total_amount) values ('lird', 10)";

MySQLConfig config;

typedef struct {
    CSVRow *rows;
    ConnectionPool *pool;

    // [start,end)
    int start;
    int end;
} WorkerArg;

MYSQL_TIME convert_to_MYSQL_TIME(struct tm t) {
    struct tm *timeinfo = &t;
    MYSQL_TIME sqlTime;

    sqlTime.year =
        timeinfo->tm_year + 1900; // struct tm stores year as years since 1900
    sqlTime.month = timeinfo->tm_mon +
                    1; // struct tm stores month as months since January (0-11)
    sqlTime.day = timeinfo->tm_mday;
    sqlTime.hour = timeinfo->tm_hour;
    sqlTime.minute = timeinfo->tm_min;
    sqlTime.second = timeinfo->tm_sec;

    return sqlTime;
}

void *worker_thread(void *arg) {
    WorkerArg *args = (WorkerArg *)arg;
    ConnectionPool *pool = args->pool;
    CSVRow *rows = args->rows;
    int i;
    for (i = (args->start); i < (args->end); i++) {
        MYSQL *connection = get_connection(pool);
        if (connection == NULL) {
            fprintf(stderr, "Error get connection wrong: %s\n",
                    mysql_error(connection));
            pthread_exit(NULL);
        }
        MYSQL_STMT *stmt = mysql_stmt_init(connection);
        const char *query = "INSERT INTO orders (customer_name, total_amount) values ('%s', %s)";
        char sql[256];
        sprintf(sql, query, rows[i].fields[1], rows[i].fields[3]);
        if (mysql_query(connection, sql)!=0) {
            fprintf(stderr,"insert data error: %s\n", mysql_error(connection));
            fprintf(stderr, "%s\n", sql);
            exit(1);
        }
        release_connection(pool, connection);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // 初始化libmysqlclient
    if (mysql_library_init(0, NULL, NULL)) {
        fprintf(stderr, "Can not init mysql client lib\n");
        exit(1);
    }

    // 初始化mysql参数
    config.host = "49.234.15.205";
    config.username = "root";
    config.password = "Lird2002";
    config.database = "test";
    config.port = 3308;

    int row_num = 0;
    CSVRow *csv = parse_csv("../test/test.csv", &row_num);

    int max_connections = 10;
    ConnectionPool pool;
    init_pool(&pool, max_connections, config);

    int data_per_thread = row_num / max_connections; // 每个线程处理的数据量
    int extra_data = row_num % max_connections; // 余数部分的数据量

    pthread_t threads[max_connections];
    WorkerArg *arg[max_connections];
    for (int i = 0; i < max_connections; i++) {
        int start_index = i * data_per_thread;         // 起始索引
        int end_index = (i + 1) * data_per_thread; // 结束索引
        if (end_index>=row_num)end_index = row_num;
        arg[i] = (WorkerArg *)malloc(sizeof(WorkerArg));
        arg[i]->start = start_index;
        arg[i]->end = end_index;
        arg[i]->pool = &pool;
        arg[i]->rows = csv;
    }
    for (int i = 0; i < max_connections; i++) {
        pthread_create(&threads[i], NULL, worker_thread, (void *)arg[i]);
    }
    for (int i = 0; i < max_connections; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < max_connections; i++) {
        free(arg[i]);
    }
    printf("Insert CSV Done\n");

    destroy_pool(&pool);

    mysql_library_end();
    return EXIT_SUCCESS;
}
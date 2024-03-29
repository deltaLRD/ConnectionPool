#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include "mysql/mysql.h"
#include "mysql_config.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

typedef struct {
    MYSQL *connection;
    int in_use;
} DBConnection;

typedef struct {
    DBConnection *connections;     // 连接数组
    int max_connections;     // 最大连接数
    MySQLConfig config;
    pthread_mutex_t lock; // 互斥锁
    pthread_cond_t cond;  // 条件变量
} ConnectionPool;

MYSQL *get_connection(ConnectionPool *pool);
void init_pool(ConnectionPool *pool, int max_connections, MySQLConfig config);
// MYSQL *create_connection(const ConnectionPool *pool);
void release_connection(ConnectionPool *pool, MYSQL *connection);
void destroy_pool(ConnectionPool *pool);
#endif
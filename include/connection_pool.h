#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include "mysql/mysql.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
typedef struct {
    MYSQL **connections;   // 连接数组
    int max_connections;   // 最大连接数
    int current_connections;   // 当前连接数
    pthread_mutex_t lock;   // 互斥锁
    pthread_cond_t cond;   // 条件变量
} ConnectionPool;

MYSQL* getConnection(ConnectionPool *pool);
void init_pool(ConnectionPool *pool, int max_connections);
MYSQL *create_connection();
void release_connection(ConnectionPool *pool, MYSQL *connection);
void destroy_pool(ConnectionPool *pool);
#endif
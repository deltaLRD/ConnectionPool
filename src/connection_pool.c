#include "connection_pool.h"
#include "mysql_config.h"
#include <mysql/mysql.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void init_pool(ConnectionPool *pool, int max_connections, MySQLConfig config) {
    pool->max_connections = max_connections;
    pool->config = config;
    pool->connections =
        (DBConnection *)malloc(sizeof(DBConnection) * max_connections);
    pthread_mutex_init(&pool->lock, NULL); // init lock
    pthread_cond_init(&pool->cond, NULL);  // init condition var
    for (int i = 0; i < max_connections; i++) {
        pool->connections[i].connection = mysql_init(NULL);
        if (mysql_real_connect(pool->connections[i].connection, config.host,
                               config.username, config.password,
                               config.database, config.port, NULL, 0) == NULL) {
            fprintf(stderr, "Error init MySQL connection\n");
            exit(1);
        }
        pool->connections[i].in_use = 0;
    }
}

// MYSQL *create_connection(const ConnectionPool *pool) {
//     MYSQL *connection = mysql_init(NULL);
//     if (connection == NULL) {
//         fprintf(stderr, "Error init MySQL connection\n");
//         return NULL;
//     }
//     if (mysql_real_connect(connection, pool->config.host,
//     pool->config.username,
//                            pool->config.password, pool->config.database,
//                            pool->config.port, NULL, 0) == NULL) {
//         fprintf(stderr, "Error connecting to MySQL database: %s\n",
//                 mysql_error(connection));
//         mysql_close(connection);
//         return NULL;
//     }
//     return connection;
// }

MYSQL *get_connection(ConnectionPool *pool) {
    pthread_mutex_lock(&pool->lock);
    MYSQL *connection = NULL;
    int i;
    while (connection == NULL) {
        for (i = 0; i < pool->max_connections; i++) {
            if (!pool->connections[i].in_use) {
                pool->connections[i].in_use = 1;
                connection = pool->connections[i].connection;
                break;
            }
        }
        if (connection == NULL) {
            pthread_cond_wait(&(pool->cond), &(pool->lock));
        }
    }
    pthread_mutex_unlock(&pool->lock);
    return connection;
}

void release_connection(ConnectionPool *pool, MYSQL *connection) {
    pthread_mutex_lock(&pool->lock);
    int i;
    for (i = 0; i < pool->max_connections; i++) {
        if (pool->connections[i].connection == connection) {
            pool->connections[i].in_use = 0;
            mysql_close(pool->connections[i].connection);
            pool->connections[i].connection = mysql_init(NULL);
            if (mysql_real_connect(pool->connections[i].connection,
                                   pool->config.host, pool->config.username,
                                   pool->config.password, pool->config.database,
                                   pool->config.port, NULL, 0) == NULL) {
                fprintf(stderr, "Error init MySQL connection\n");
                exit(1);
            }
            break;
        }
    }
    pthread_mutex_unlock(&pool->lock);
    pthread_cond_signal(&pool->cond);
}

void destroy_pool(ConnectionPool *pool) {
    int i;
    for (i = 0; i < pool->max_connections; i++) {
        mysql_close(pool->connections[i].connection);
    }
    free(pool->connections);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
}
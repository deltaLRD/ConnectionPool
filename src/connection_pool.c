#include "connection_pool.h"
#include "mysql_config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void init_pool(ConnectionPool *pool, int max_connections, MySQLConfig config) {
    pool->max_connections = max_connections;
    pool->current_connections = 0;
    pool->config = config;
    pool->connections = (MYSQL **)malloc(sizeof(MYSQL *) * max_connections);
    pthread_mutex_init(&pool->lock, NULL); // init lock
    pthread_cond_init(&pool->cond, NULL);  // init condition var
    for (int i = 0; i < max_connections; i++) {
        pool->connections[i] = NULL;
    }
}

MYSQL *create_connection(const ConnectionPool *pool) {
    MYSQL *connection = mysql_init(NULL);
    if (connection == NULL) {
        fprintf(stderr, "Error init MySQL connection\n");
        return NULL;
    }
    if (mysql_real_connect(connection, pool->config.host, pool->config.username,
                           pool->config.password, pool->config.database,
                           pool->config.port, NULL, 0) == NULL) {
        fprintf(stderr, "Error connecting to MySQL database: %s\n",
                mysql_error(connection));
        mysql_close(connection);
        return NULL;
    }
    return connection;
}

MYSQL *get_connection(ConnectionPool *pool) {
    MYSQL *connection = NULL;
    pthread_mutex_lock(&pool->lock);
    while (pool->current_connections == pool->max_connections) {
        pthread_cond_wait(&pool->cond, &pool->lock);
    }
    for (int i = 0; i < pool->max_connections; i++) {
        if (pool->connections[i] == NULL) {
            connection = create_connection(pool);
            if (connection != NULL) {
                pool->connections[i] = connection;
                pool->current_connections++;
            }
            break;
        }
    }
    pthread_mutex_unlock(&pool->lock);
    return connection;
}

void release_connection(ConnectionPool *pool, MYSQL *connection) {
    pthread_mutex_lock(&pool->lock);
    int i;
    for (i = 0; i < pool->max_connections; i++) {
        if (pool->connections[i] == connection) {
            mysql_close(connection);
            pool->connections[i] = NULL;
            pool->current_connections--;
            pthread_cond_signal(&pool->cond);
            break;
        }
    }
    pthread_mutex_unlock(&pool->lock);
}

void destroy_pool(ConnectionPool *pool) {
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    int i;
    for (i = 0; i < pool->max_connections; i++) {
        if (pool->connections[i] != NULL) {
            mysql_close(pool->connections[i]);
        }
    }
    free(pool->connections);
}
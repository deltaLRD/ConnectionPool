#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include "mysql/mysql.h"
typedef struct {

} ConnectionPool;
MYSQL* getConnection(ConnectionPool *pool);
ConnectionPool* initConnectionPool(int nums);
#endif
#ifndef MYSQL_CONFIG_H
#define MYSQL_CONFIG_H

typedef struct {
    char *host;
    char *username;
    char *password;
    char *database;
    int   port;
} MySQLConfig;

#endif
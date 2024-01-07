#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>
#include <pthread.h>

// 用字节作为最小单位实现通用的线程安全的vector
typedef struct {
    char* data;
    size_t len;
    size_t cap;
    size_t ele_size;
    pthread_mutex_t lock;
} Vector;

void initVector(Vector* vector, size_t ele_size);
void pushBack(Vector* vector, char* value);
void freeVector(Vector* vector);
void* getVectorElement(Vector* vector, int pos);
#endif
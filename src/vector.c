#include "vector.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

void initVector(Vector *vector, size_t ele_size) {
    vector->data = NULL;
    vector->len = 0;
    vector->cap = 0;
    vector->ele_size = ele_size;
    pthread_mutex_init(&vector->lock, NULL);
}

void pushBack(Vector *vector, char *value) {
    pthread_mutex_lock(&vector->lock);
    if (vector->len * vector->ele_size >= vector->cap * 0.8) {
        vector->cap = (vector->cap == 0) ? 1 : vector->cap * 2;
        vector->data = realloc(vector->data, vector->cap);
    }
    size_t start_offest = vector->len * vector->ele_size;
    for (size_t i = 0; i < vector->ele_size; i++) {
        vector->data[start_offest + i] = *(value + i);
    }
    vector->len++;
    pthread_mutex_unlock(&vector->lock);
}

// 不考虑递归释放的情况
void freeVector(Vector *vector) {
    pthread_mutex_lock(&vector->lock);
    free(vector->data);
    vector->data = NULL;
    vector->len = 0;
    vector->cap = 0;
    pthread_mutex_unlock(&vector->lock);
    pthread_mutex_destroy(&vector->lock);
}

void *getVectorElement(Vector *vector, int pos) {
    if (pos > vector->len) {
        return NULL;
    }
    return (void *)(vector->data + (pos)*vector->ele_size);
}
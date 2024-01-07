#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    initVector(vector, sizeof(int));
    int ele = 32;
    pushBack(vector, (char*)(&ele));
    ele = 10;
    pushBack(vector, (char*)(&ele));
    ele = 11;
    pushBack(vector, (char*)(&ele));

    int result = 0;
    result = *(int*)getVectorElement(vector, 2);
    if(result != 11){
        fprintf(stderr, "%d\n", result);
        exit(1);
    }

    freeVector(vector);
    exit(0);
}
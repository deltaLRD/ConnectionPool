#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include <time.h>

int main() {
    struct timespec start, end;
    double execution_time;
    // 记录开始时间
    clock_gettime(CLOCK_MONOTONIC, &start);

    char* file_buffer = read_csv("./test.csv");
    if (file_buffer == NULL) {
        perror("read file error\n");
        exit(1);
    }
    if (count_lines(file_buffer, strlen(file_buffer)) != 4) {
        perror("count lines error\n");
        exit(1);
    }
    // 记录结束时间
    clock_gettime(CLOCK_MONOTONIC, &end);

    // 计算执行时间（以微秒为单位）
    execution_time = (double)(end.tv_sec - start.tv_sec) * 1e6 + (double)(end.tv_nsec - start.tv_nsec) / 1000;

    // 输出执行时间
    fprintf(stderr, "Execution Time: %.2f microseconds\n", execution_time);
    
    exit(0);
}
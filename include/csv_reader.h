#ifndef CSV_READER_H
#define CSV_READER_H
#include <stdint.h>
// 定义一行数据
typedef struct {
    char **fields;
    int fields_cnt;
} CSVRow;

// 解析CSV
CSVRow *parse_csv(const char *filename, int *row_nums);

// 获取csv行数
int count_lines(const char *buffer, int buffer_size);

char *read_csv(const char *filename);
#endif
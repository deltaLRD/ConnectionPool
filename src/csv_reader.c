#include "csv_reader.h"
#include "vector.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 获取csv行数
int count_lines(const char *buffer, int buffer_size) {
    int newline_count = 0;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer[i] == '\n') {
            newline_count++;
        }
    }
    return newline_count;
}

char *read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File open error\n");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content_buffer = (char *)malloc(file_size + 1);

    int read_size = fread(content_buffer, 1, file_size, file);
    content_buffer[file_size] = '\0';
    fclose(file);
    return content_buffer;
}

CSVRow *parse_csv(const char *filename, int *row_nums) {
    char *file_buffer = read_csv(filename);
    int file_size = strlen(file_buffer);

    int lines_num = count_lines(file_buffer, file_size);
    *row_nums = lines_num;

    // 申请空间
    char **lines = malloc(lines_num * sizeof(char *));
    int tmp_cnt = 0;
    int pos = 0;
    for (int i = 0; i < file_size; i++) {
        if (file_buffer[i] == '\n') {
            lines[tmp_cnt] = malloc(i - pos + 2);
            memmove(lines[tmp_cnt++], file_buffer + pos, i - pos + 1);
            pos = i + 1;
        }
    }

    // 将每一行数据的最后改成,方便处理
    for (int i = 0; i < lines_num; i++) {
        int tmp_len = strlen(lines[i]);
        lines[i][tmp_len - 1] = ',';
    }

    CSVRow *csv_data = (CSVRow *)malloc(lines_num * sizeof(CSVRow));
    int field_cnt = 0;
    for (int i = 0; i < strlen(lines[0]); i++) {
        if (lines[0][i] == ',') {
            field_cnt++;
        }
    }

    for (int i = 0; i < lines_num; i++) {
        csv_data[i].fields = malloc(field_cnt * sizeof(char *));
        int tmp_field_cnt = 0;
        char *tmp_field = strtok(lines[i], ",");
        while (tmp_field != NULL) {
            csv_data[i].fields[tmp_field_cnt++] = tmp_field;
            tmp_field = strtok(NULL, ",");
        }
        csv_data[i].fields_cnt = tmp_field_cnt;
    }

    return csv_data;
}
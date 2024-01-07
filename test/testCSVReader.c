#include "csv_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    CSVRow* csv;
    int nums;
    csv = parse_csv("./test.csv", &nums);
    if (csv[0].fields_cnt != 4) {
        perror("field cnt wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if (csv[1].fields_cnt != 4) {
        perror("field cnt wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if (csv[2].fields_cnt != 4) {
        perror("field cnt wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if (csv[3].fields_cnt != 4) {
        perror("field cnt wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }

    if ( strcmp(csv[0].fields[0], "100") != 0 ) {
        perror("field 0 wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[0].fields[1], "lird") != 0 ) {
        perror("field 1 wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[0].fields[2], "2024-01-07") != 0 ) {
        perror("field 2 wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[0].fields[3], "11") != 0 ) {
        perror("field 3 wrong");
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }

    if ( strcmp(csv[1].fields[0], "101") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[1].fields[1], "lird") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[1].fields[2], "2024-01-07") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[1].fields[3], "10") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }

    if ( strcmp(csv[2].fields[0], "102") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[2].fields[1], "lird") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[2].fields[2], "2024-01-07") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[2].fields[3], "12") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }

    if ( strcmp(csv[3].fields[0], "103") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[3].fields[1], "lird") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[3].fields[2], "2024-01-07") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    if ( strcmp(csv[3].fields[3], "13") != 0 ) {
        fprintf(stderr, "%d\n", __LINE__);
        exit(1);
    }
    return 0;
}
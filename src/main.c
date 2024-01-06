#include <stdio.h>
#include "mysql/mysql.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(mysql_library_init(0, NULL, NULL)){
        fprintf(stdout, "Can not init mysql client lib\n");
        exit(1);
    }
    printf("hello world\n");
    
    mysql_library_end();
    return EXIT_SUCCESS;
}
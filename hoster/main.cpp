
#include <stdio.h>

#include "file_infect.h"
#include "file_recover.h"

void main(void) {
    char md5_buffer[16]={0};
    file_infect("F:\\test.txt",md5_buffer);

    printf("OK,Get Next Step");
    getchar();

    file_recover("F:\\test.txt.exe",md5_buffer);
}

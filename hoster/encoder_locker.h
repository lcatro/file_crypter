
#ifndef _LOCKER_H__
#define _LOCKER_H__

typedef struct {
    unsigned char* output_buffer;
    unsigned int   output_buffer_length;
    unsigned char  lock_md5[16];
} locker_output;

bool encoder_locker(unsigned char* input_buffer,unsigned int input_buffer_length,locker_output* output);

#endif

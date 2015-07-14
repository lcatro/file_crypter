
#ifndef _UNLOCKER_H__
#define _UNLOCKER_H__

char* encoder_unlocker(unsigned char* input_buffer,unsigned int input_buffer_length,unsigned char lock_md5[16],unsigned long* output_length);

#endif

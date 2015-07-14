
#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "encoder_base64.h"
#include "encoder_dymaic.h"
#include "encoder_md5.h"
#include "encoder_unlocker.h"
#include "encoder_xor.h"

char* encoder_unlocker(unsigned char* input_buffer,unsigned int input_buffer_length,unsigned char lock_md5[16],unsigned long* output_length) {
    if (NULL==input_buffer || NULL==input_buffer_length) return NULL;
    encoder_xor(input_buffer,input_buffer_length,lock_md5,16);
    dymaic_decode((char*)input_buffer,input_buffer_length);
    char* decoder_base64=base64_decode((const char*)input_buffer,input_buffer_length,(unsigned long*)output_length);
    return decoder_base64;
}

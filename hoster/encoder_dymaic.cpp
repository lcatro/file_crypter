
#include <malloc.h>
#include <memory.h>

#include "encoder_dymaic.h"

unsigned int dymaic_encode(char* encode_string,unsigned int encode_string_length_) {
    unsigned int encode_buffer_length=!(encode_string_length_%2)?encode_string_length_:encode_string_length_+1;
    char* encode_buffer=(char*)malloc(encode_buffer_length);
    char* encode_point=encode_buffer;
    memset(encode_buffer,0,encode_buffer_length);
    memcpy(encode_buffer,encode_string,encode_string_length_);

    for (unsigned int index=0;index<encode_buffer_length;index+=2,encode_point+=2) {
        unsigned char key_num1=(*encode_point&0xF),key_num2=(*encode_point&0xF0)>>4,key_num3=(*(encode_point+1)&0xF),key_num4=(*(encode_point+1)&0xF0)>>4;
        *encode_point=key_num2+(key_num3<<4);
        *(encode_point+1)=key_num4+(key_num1<<4);
        *encode_point^=encode_buffer_length;
        *(encode_point+1)^=encode_buffer_length;
    }
    memcpy(encode_string,encode_buffer,encode_buffer_length);
    free(encode_buffer);
    return encode_buffer_length;
}

void dymaic_decode(char* decode_string,unsigned int decode_string_length_) {
    unsigned int decode_buffer_length=!(decode_string_length_%2)?decode_string_length_:decode_string_length_+1;
    char* decode_buffer=(char*)malloc(decode_buffer_length);
    char* decode_point=decode_buffer;
    memset(decode_point,0,decode_buffer_length);
    memcpy(decode_buffer,decode_string,decode_string_length_);

    for (unsigned int index=0;index<decode_buffer_length;index+=2,decode_point+=2) {
        *decode_point^=decode_buffer_length;
        *(decode_point+1)^=decode_buffer_length;
        unsigned char key_num1=(*(decode_point+1)&0xF0)>>4,key_num2=(*decode_point&0xF),key_num3=(*(decode_point)&0xF0)>>4,key_num4=(*(decode_point+1)&0xF);
        *decode_point=key_num1+(key_num2<<4);
        *(decode_point+1)=key_num3+(key_num4<<4);
    }
    memcpy(decode_string,decode_buffer,decode_buffer_length);
    free(decode_buffer);
}


#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "encoder_base64.h"
#include "encoder_dymaic.h"
#include "encoder_locker.h"
#include "encoder_md5.h"
#include "encoder_xor.h"

bool encoder_locker(unsigned char* input_buffer,unsigned int input_buffer_length,locker_output* output) {
    if (NULL==output) return false;

    char* encoder_base64=base64_encode((const char*)input_buffer,(int)input_buffer_length);
    if (NULL!=encoder_base64) {
        unsigned int encoder_base64_length=strlen(encoder_base64);
        unsigned int encoder_dymaic_length=dymaic_encode(encoder_base64,encoder_base64_length);
        char* md5=calcu_md5(encoder_base64,encoder_dymaic_length);
        encoder_xor((unsigned char*)encoder_base64,encoder_dymaic_length,(unsigned char*)md5,16);
        memcpy(&output->lock_md5,md5,16);
        output->output_buffer_length=encoder_dymaic_length;
        output->output_buffer=(unsigned char*)encoder_base64;
        return true;
    }
    return false;
}


#include "encoder_xor.h"

void encoder_xor(unsigned char* input_buffer,unsigned int input_buffer_length,unsigned char xor_torrent) {
    for (unsigned int encoder_index=0;encoder_index<input_buffer_length;++encoder_index)
        *(input_buffer+encoder_index)^=xor_torrent;
}

void encoder_xor(unsigned char* input_buffer,unsigned int input_buffer_length,unsigned char* xor_torrent,unsigned int xor_torrent_length) {
    for (unsigned int encoder_index=0,torrent_index=0;encoder_index<input_buffer_length;++encoder_index) {
        *(input_buffer+encoder_index)^=*(xor_torrent+torrent_index);
        if (torrent_index>=xor_torrent_length)
            torrent_index=0;
        else
            ++torrent_index;
    }
}

#include <stdio.h>
#include "ecg_blt.h"
#include "ecg.h"

unsigned int packet_type;
unsigned char ecg_buffer[4096];

int ecg_decode(unsigned char *buff, unsigned int buff_size)
{
    int ret = 0;

    if((ret = ecg_blt_decode(buff, buff_size)) > 0) {
        print_broadcast_packet_info(&(decoded_blt_pkt->body.broadcast));
    }

    return ret;
}


#include <stdio.h>
#include "ecg_blt.h"
#include "ecg.h"

unsigned int packet_type;
unsigned char ecg_buffer[4096];

int ecg_decode(unsigned char *buff, int buff_size)
{
    int ret = 0;

    if((ret = blt_ecg_decode(buff, buff_size)) > 0) {
    }

    return ret;
}


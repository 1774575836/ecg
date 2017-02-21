#ifndef _ECG_H
#define _ECG_H

extern unsigned char ecg_buffer[4096];
int ecg_decode(unsigned char *buff, int buff_size);

#endif

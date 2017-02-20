#include <stdio.h>
#include <string.h>
#include "ecg_blt.h"
#include "ecg.h"

packet_t *decoded_blt_pkt;

void print_broadcast_packet_info(broadcast_packet_t *broad)
{
	printf("Dump broadcast packet info:\n");
	printf("Software version %d.%d\n", broad->config.MAJOR, broad->config.MINOR);
	if(broad->config.ICG) {
		printf("with ICG\n");
	}
	if(broad->config.RESP) {
		printf("with RESP\n");
	}
	if(broad->config.PULSE) {
		printf("with PULSE\n");
	}
	if(broad->config.SPO2) {
		printf("with SPO2\n");
	}
	if(broad->config.NIBP) {
		printf("with NIBP\n");
	}
	if(broad->config.ECG12) {
		printf("with ECG12\n");
	}
	if(broad->config.ECG5) {
		printf("with ECG5\n");
	}
	if(broad->config.ECG3) {
		printf("with ECG3\n");
	}
	if(broad->config.IBP1){
		printf("with IBP1\n");
	}
	if(broad->config.IBP2){
		printf("with IBP2\n");
	}
	if(broad->config.IBP3){
		printf("with IBP3\n");
	}
	if(broad->config.IBP4){
		printf("with IBP4\n");
	}
	if(broad->config.IBP5){
		printf("with IBP5\n");
	}
	if(broad->config.IBP6){
		printf("with IBP6\n");
	}
	if(broad->config.IBP7){
		printf("with IBP7\n");
	}
	if(broad->config.IBP8){
		printf("with IBP8\n");
	}
	if(broad->config.TEMP1){
		printf("with TEMP1\n");
	}
	if(broad->config.TEMP2){
		printf("with TEMP2\n");
	}
	if(broad->config.TEMP3){
		printf("with TEMP3\n");
	}
	if(broad->config.TEMP4){
		printf("with TEMP4\n");
	}
	if(broad->config.TEMP5){
		printf("with TEMP5\n");
	}
	if(broad->config.TEMP6){
		printf("with TEMP6\n");
	}
	if(broad->config.TEMP7){
		printf("with TEMP7\n");
	}
	if(broad->config.TEMP8){
		printf("with TEMP8\n");
	}
	if(broad->config.CO2){
		printf("with CO2\n");
	}
	if(broad->config.GAS){
		printf("with GAS\n");
	}
	if(broad->config.CO){
		printf("with CO\n");
	}
}

// a broadcast packet:
// 	B0	B1	B2	B3	B4	B5	B6	B7	B8	B9	B10	B11
// 	0xFF	0xD0	0xXX	0xXX	0x00 	[Configuration of Modules	]	status1	status2
//always return TRUE
int check_broadcast(packet_t *pkt)
{
	printf("Broadcast packet decoded\n");
	decoded_blt_pkt = pkt;
	//print_broadcast_packet_info(&(pkt->body.broadcast));
	return 1;
}

void generate_broadcast_packet()
{
}

int ecg_blt_decode(unsigned char *buff, unsigned int buff_size)
{
	packet_t *pkt;
	int i, decode_size;

	printf(" decode ECG buffer: %p, size %d\n ", buff, buff_size);
#if 1
	for(i = 0; i < buff_size; i++) {
		printf("%02x ", buff[i]);
	}
#endif
	printf("\n");
	memcpy(ecg_buffer, buff, buff_size);

	pkt = (packet_t *)ecg_buffer;
	decode_size = 5;
	if(buff_size < decode_size)
		return 0;

	if(pkt->header.B0.sync == PACKET_SYNC_BYTE) {
		switch(pkt->header.B1.type) {
			case PACKET_TYPE_BROADCAST:
				decode_size = 12;
				 if(decode_size > buff_size)
				 	return 0;
				if(check_broadcast(pkt)) {
				    packet_type = 1;
					return 12;
				}else{
					return 0;
				}
			break;
			case PACKET_TYPE_DATA:
			break;
			case PACKET_TYPE_ARRHY_INFO:
			break;
			case PACKET_TYPE_SLAVE_TO_MASTER_SETTINGS:
			break;
			case PACKET_TYPE_MASTER_TO_SLAVE_SETTINGS:
			break;
			case PACKET_TYPE_COMMAND_RESPONSE:
			break;
			default:
			break;
		}
	}

	return 0;
}
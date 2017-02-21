#include <stdio.h>
#include <string.h>
#include "ecg_blt.h"
#include "ecg.h"

packet_t *decoded_blt_pkt;

void print_broadcast_packet_info(broadcast_packet_t *broad)
{
	printf("Dump broadcast packet info:\n");
	printf(" packet size: 7\n");
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
int blt_decode_broadcast(unsigned char *buffer, unsigned int size, broadcast_packet_t *broadcast, int *consumed_bytes)
{
	if(size >= 7) {//broadcast payload size is 7B
		*consumed_bytes = 7;
		broadcast = (broadcast_packet_t *)buffer;
		printf("Broadcast packet decoded\n");
		print_broadcast_packet_info(broadcast);
		return 0;
	}else{
		printf("broadcast packet is not complete!\n");
		return -1;
	}
}

void generate_broadcast_packet()
{
}

void print_patiant_config_packet_info(PATIANT_config_t *patiant)
{
	printf("Dump patiant config packet info:\n");
	printf(" packet size: %d\n", patiant->length_h<<8|patiant->length_l);
	printf(" patiant age:%d\n", patiant->age);
	printf(" patiant gender:%s\n", patiant->gender?"Female":"Male");
	printf(" patiant weight:%d(%s)\n", patiant->weight_4<<8|patiant->weight_8, patiant->weight_unit?"Lb":"Kg");
	printf(" patiant height:%d(%s)\n", patiant->height, patiant->height_unit?"cm":"ft");
	printf(" patiant type:%x\n", patiant->patiant_type);
	printf(" patiant %s qiboqi\n", patiant->with_qiboqi?"with":"without");
}

void print_temp_config_packet_info(TEMP_config_t *temp)
{
	printf("Dump temp config packet info:\n");
	printf(" packet size: %d\n", temp->length_h<<8|temp->length_l);
	printf(" temp number:%d\n", temp->type-CONFIG_TYPE_TEMP1+1);
	printf(" unit %s\n", (temp->unit==1)?"farenheit":"celsius");
	printf(" alarm %s\n", temp->alarm_onoff?"on":"off");
	printf(" alarm level %d\n", temp->alarm_level);
	printf(" alarm high level %d\n", temp->alarm_high_level_1 << 8 | temp->alarm_high_level_8);
	printf(" alarm low level %d\n", temp->alarm_low_level_1 << 8 | temp->alarm_low_level_8);
}

void print_monitor_config_packet_info(MONITOR_config_t *monitor)
{
	printf("Dump monitor config packet info:\n");
}

void print_resp_config_packet_info(RESP_config_t *resp)
{
	printf("Dump resp config packet info:\n");
}

void print_pulse_config_packet_info(PULSE_config_t *pulse)
{
	printf("Dump pulse config packet info:\n");
}

void print_spo2_config_packet_info(SPO2_config_t *spo2)
{
	printf("Dump spo2 config packet info:\n");
}

void print_nibp_config_packet_info(NIBP_config_t *nibp)
{
	printf("Dump nibp config packet info:\n");
}

void print_ecg_config_packet_info(ECG_config_t *ecg)
{
	printf("Dump ecg config packet info:\n");
	printf(" packet size: %d\n", ecg->length_h<<8|ecg->length_l);
	printf(" drift filter %s\n", (ecg->filter_drift==1)?"on 1":(ecg->filter_drift==2?"on 2":"off"));
	printf(" AC filter %d\n", ecg->filter_ac);
	printf(" Muscle filter %s\n", ecg->filter_muscle?"on":"off");
	printf(" work mode %d\n", ecg->mode);
	printf(" channel %x\n", ecg->channel);
	if(ecg->type == CONFIG_TYPE_ECG5) {
		printf("%s\n", ecg->ecg3_5?" ECG 3":" ECG 5");
	}
	printf(" qibofenxi %s\n", ecg->qibofenxi?"on":"off");
	printf(" alarm %s\n", ecg->alarm_onoff?"on":"off");
	printf(" alarm high level %d\n", ecg->alarm_high_level_4<<8|ecg->alarm_high_level_8);
	printf(" alarm low level %d\n", ecg->alarm_low_level_4<<8|ecg->alarm_low_level_8);
}
	
// a configration packet:
// 0xff, 0xd4, 0x01, 0xf1, 0x00, 0x00, 0x56, 0xce, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x04, 0x00, 0xc5, 0x86, 0x68, 0xfb, 0x00, 0xfd, 0x00, 0x06, 0x0d, 0x02, 0x04, 0x78, 0x32, 0x00, 0xfa, 0x00, 0x03, 0x04, 0x64, 0x5a, 0xf9, 0x00, 0x04, 0x0c, 0x78, 0x32, 0x00, 0xf8, 0x00, 0x06, 0x21, 0x80, 0x14, 0x04, 0x1e, 0x08, 0xc1, 0x00, 0x04, 0x00, 0xc5, 0x86, 0x68, 0xfb, 0x00, 0x0c, 0x20, 0x00, 0x04, 0xa0, 0x5a, 0x00, 0x5a, 0x32, 0x00, 0x6e, 0x3c, 0x00
int blt_decode_config(unsigned char *buffer, unsigned int size, config_packet_t *config, int *consumed_bytes)
{
	ECG_config_t *ecg;
	SPO2_config_t *spo2;
	PULSE_config_t *pulse;
	RESP_config_t *resp;
	TEMP_config_t *temp;
	NIBP_config_t *nibp;
	PATIANT_config_t *patiant;
	MONITOR_config_t *monitor;
	
	int decode_size, length;
	config = (config_packet_t *)buffer;
	decode_size = 2; // add the length field size;
	length = (config->length_h<<8 | config->length_l);
	if(decode_size + length > size){
		return -1; //the packet size is larger than received data
	}
	
	//the data integration check is done, no need to check more
	while(decode_size < size) {
		//printf("\ntype:%d,%d\n", buffer[decode_size],decode_size);
		switch(buffer[decode_size]) {
		case CONFIG_TYPE_PATIANT: //0xCE
			patiant = (PATIANT_config_t *)&buffer[decode_size];
			length = patiant->length_h << 8 | patiant->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("PATIANT config packet decoded, size %d\n", length);
			print_patiant_config_packet_info(patiant);
		break;
		
		case CONFIG_TYPE_TEMP1: //0xC1
		case CONFIG_TYPE_TEMP2: //0xC2
		case CONFIG_TYPE_TEMP3: //0xC3
		case CONFIG_TYPE_TEMP4: //0xC4
		case CONFIG_TYPE_TEMP5: //0xC5
		case CONFIG_TYPE_TEMP6: //0xC6
		case CONFIG_TYPE_TEMP7: //0xC7
		case CONFIG_TYPE_TEMP8: //0xC8
			temp = (TEMP_config_t *)&buffer[decode_size];
			length = temp->length_h << 8 | temp->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("TEMPEARTURE config packet decoded, size %d\n", length);
			print_temp_config_packet_info(temp);
		break;
		
		case CONFIG_TYPE_MONITOR: //0xF3
			monitor = (MONITOR_config_t *)&buffer[decode_size];
			length = monitor->length_h << 8 | monitor->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("MONITOR config packet decoded, size %d\n", length);
			print_monitor_config_packet_info(monitor);
		break;
		
		case CONFIG_TYPE_RESP: //0xF8
			resp = (RESP_config_t *)&buffer[decode_size];
			length = resp->length_h << 8 | resp->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("RESP config packet decoded, size %d\n", length);
			print_resp_config_packet_info(resp);
		break;
		
		case CONFIG_TYPE_PULSE: //0xF9
			pulse = (PULSE_config_t *)&buffer[decode_size];
			length = pulse->length_h << 8 | pulse->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("PULSE config packet decoded, size %d\n", length);
			print_pulse_config_packet_info(pulse);
		break;
		
		case CONFIG_TYPE_SPO2: //0xFA
			spo2 = (SPO2_config_t *)&buffer[decode_size];
			length = spo2->length_h << 8 | spo2->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("SPO2 config packet decoded, size %d\n", length);
			print_spo2_config_packet_info(spo2);
		break;
		
		case CONFIG_TYPE_NIBP: //0xFB
			nibp = (NIBP_config_t *)&buffer[decode_size];
			length = nibp->length_h << 8 | nibp->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("NIBP config packet decoded, size %d\n", length);
			print_nibp_config_packet_info(nibp);
		break;
		
		case CONFIG_TYPE_ECG3: //0xFE
		case CONFIG_TYPE_ECG5: //0xFD
		case CONFIG_TYPE_ECG12: //0xFC
			ecg = (ECG_config_t *)&buffer[decode_size];
			length = ecg->length_h << 8 | ecg->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("ECG config packet decoded, size %d\n", length);
			print_ecg_config_packet_info(ecg);
		break;
		
		default:
		break;
		}
	}
	return 1;
}

int blt_ecg_decode(unsigned char *buff, unsigned int buff_size)
{
	packet_t *pkt;
	broadcast_packet_t *broadcast;
	config_packet_t *config;
	int i, decode_size, consumed;
	int length;

	printf(" decode ECG buffer: %p, size %d\n ", buff, buff_size);
#if 1
	for(i = 0; i < buff_size; i++) {
		printf("%02x ", buff[i]);
	}
#endif
	printf("\n");
	memcpy(ecg_buffer, buff, buff_size);

	pkt = (packet_t *)ecg_buffer;
	decode_size = PKT_HEADER_SIZE;
	if(buff_size < decode_size)
		return 0;

	if(pkt->header.B0.sync == PACKET_SYNC_BYTE) {
		switch(pkt->header.B1.type) {
			case PACKET_TYPE_BROADCAST:
				if(blt_decode_broadcast(
					&ecg_buffer[decode_size],
					buff_size-decode_size,
					broadcast,
					&consumed) == 0) { // skip header
					return (decode_size+consumed);
				}else{
					return 0;
				}
			break;
			case PACKET_TYPE_DATA:
			break;
			case PACKET_TYPE_ARRHY_INFO:
			break;
			case PACKET_TYPE_SLAVE_TO_MASTER_SETTINGS:
				//process with config decode
				if(blt_decode_config(
					&ecg_buffer[decode_size],
					buff_size-decode_size,
					config,
					&consumed) == 0) {
					return (decode_size+consumed);
				}else{
					return 0;
				}
				
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
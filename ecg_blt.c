#include <stdio.h>
#include <string.h>
#include "ecg_blt.h"
#include "ecg.h"

packet_t *decoded_blt_pkt;

/********arrhythmia packet(type 0xD3) decode *************/
void print_arrhy_packet_info(arrhy_packet_t *arrhy)
{
	printf("Dump arrhy packet info:\n");
	switch(arrhy->code) {
		case ARRHY_NORMAL: printf(" NORMAL");
		break;
		case ARRHY_ASYSTOLE: printf(" ARRHY_ASYSTOLE");
		break;
		case ARRHY_VENT_FIB: printf(" ARRHY_VENT_FIB");
		break;
		case ARRHY_VENT_TACHY: printf(" ARRHY_VENT_TACHY");
		break;
		case ARRHY_MULTIPLE_PVCS: printf(" ARRHY_MULTIPLE_PVCS");
		break;
		case ARRHY_COUPLET: printf(" ARRHY_COUPLET");
		break;
		case ARRHY_PVCS_BIGEMINY: printf(" ARRHY_PVCS_BIGEMINY");
		break;
		case ARRHY_PVCS_TRIGEMINY: printf(" ARRHY_PVCS_TRIGEMINY");
		break;
		case ARRHY_R_ON_T: printf(" ARRHY_R_ON_T");
		break;
		case ARRHY_VPB: printf(" ARRHY_VPB");
		break;
		case ARRHY_TACHY: printf(" ARRHY_TACHY");
		break;
		case ARRHY_BRADY: printf(" ARRHY_BRADY");
		break;
		case ARRHY_MISSED_BEATS: printf(" ARRHY_MISSED_BEATS");
		break;
		case ARRHY_ST_EPRESS: printf(" ARRHY_ST_EPRESS");
		break;
		case ARRHY_ST_ELEVATE: printf(" ARRHY_ST_ELEVATE");
		break;
		case ARRHY_PNP: printf(" ARRHY_PNP");
		break;
		case ARRHY_PNC: printf(" ARRHY_PNC");
		break;
		default:
		break;
	}
	printf("\n");
}

int blt_decode_arrhy(unsigned char *buffer, int size, arrhy_packet_t *arrhy, int *consumed_bytes)
{
    int decode_size, length, cmd;
	arrhy = (arrhy_packet_t *)buffer;
	decode_size = 3; // add the length field and code
	if(decode_size > size){
		return -1; //the packet size is larger than received data
	}

	printf("decode arrhy packet\n");
	print_arrhy_packet_info(arrhy);
    return -1;
}
/********End of arrhythmia packet(type 0xD3) decode *************/



/********broadcast packet(type 0xD0) decode *************/
void print_broadcast_packet_info(broadcast_packet_t *broad)
{
	printf("Dump broadcast packet info:\n");
	printf(" packet size: 7\n");
	printf(" Software version %d.%d\n", broad->config.MAJOR, broad->config.MINOR);
	if(broad->config.ICG) {
		printf(" with ICG\n");
	}
	if(broad->config.RESP) {
		printf(" with RESP\n");
	}
	if(broad->config.PULSE) {
		printf(" with PULSE\n");
	}
	if(broad->config.SPO2) {
		printf(" with SPO2\n");
	}
	if(broad->config.NIBP) {
		printf(" with NIBP\n");
	}
	if(broad->config.ECG12) {
		printf(" with ECG12\n");
	}
	if(broad->config.ECG5) {
		printf(" with ECG5\n");
	}
	if(broad->config.ECG3) {
		printf(" with ECG3\n");
	}
	if(broad->config.IBP1){
		printf(" with IBP1\n");
	}
	if(broad->config.IBP2){
		printf(" with IBP2\n");
	}
	if(broad->config.IBP3){
		printf(" with IBP3\n");
	}
	if(broad->config.IBP4){
		printf(" with IBP4\n");
	}
	if(broad->config.IBP5){
		printf(" with IBP5\n");
	}
	if(broad->config.IBP6){
		printf(" with IBP6\n");
	}
	if(broad->config.IBP7){
		printf(" with IBP7\n");
	}
	if(broad->config.IBP8){
		printf(" with IBP8\n");
	}
	if(broad->config.TEMP1){
		printf(" with TEMP1\n");
	}
	if(broad->config.TEMP2){
		printf(" with TEMP2\n");
	}
	if(broad->config.TEMP3){
		printf(" with TEMP3\n");
	}
	if(broad->config.TEMP4){
		printf(" with TEMP4\n");
	}
	if(broad->config.TEMP5){
		printf(" with TEMP5\n");
	}
	if(broad->config.TEMP6){
		printf(" with TEMP6\n");
	}
	if(broad->config.TEMP7){
		printf(" with TEMP7\n");
	}
	if(broad->config.TEMP8){
		printf(" with TEMP8\n");
	}
	if(broad->config.CO2){
		printf(" with CO2\n");
	}
	if(broad->config.GAS){
		printf(" with GAS\n");
	}
	if(broad->config.CO){
		printf(" with CO\n");
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

/********End of broadcast packet(type 0xD0) decode *************/


/********config packet(type 0xD4) decode *************/
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
	printf(" temp number:%d\n", temp->module-MODULE_TEMP1+1);
	printf(" unit %s\n", (temp->unit==1)?"farenheit":"celsius");
	printf(" alarm %s\n", temp->alarm_onoff?"on":"off");
	printf(" alarm level %d\n", temp->alarm_level);
	printf(" alarm high level %d\n", temp->alarm_high_level_1 << 8 | temp->alarm_high_level_8);
	printf(" alarm low level %d\n", temp->alarm_low_level_1 << 8 | temp->alarm_low_level_8);
}

void print_monitor_config_packet_info(MONITOR_config_t *monitor)
{
	printf("Dump monitor config packet info:\n");
	printf(" packet size: %d\n", monitor->length_h<<8|monitor->length_l);
	printf(" heart beat sound level %d\n", monitor->heart_beat_sound_level);
	printf(" alarm sound level %d\n", monitor->alarm_sound_level);
}

void print_resp_config_packet_info(RESP_config_t *resp)
{
	printf("Dump resp config packet info:\n");
	printf(" packet size: %d\n", resp->length_h<<8|resp->length_l);
	printf(" source %s\n", resp->source==0?"AUTO":(resp->source==1?"ECG":"CO2"));
	printf(" amplify %d\n", resp->amplify);
	printf(" suffocate time %d\n", resp->suff_time);
	printf(" suffocate %s\n",resp->suff_onoff?"ON":"OFF");
	printf(" breath sensitive value: %d\n", resp->sense_value);
	printf(" alarm level %d\n", resp->alarm_level);
	printf(" alarm high level %d\n", resp->alarm_high_level_8);
	printf(" alarm low level %d\n", resp->alarm_low_level_8);
}

void print_pulse_config_packet_info(PULSE_config_t *pulse)
{
	printf("Dump pulse config packet info:\n");
	printf(" packet size: %d\n", pulse->length_h<<8|pulse->length_l);
	printf(" alarm %s\n", pulse->alarm_onoff?"on":"off");
	printf(" alarm level %d\n", pulse->alarm_level);
	printf(" source %d\n", pulse->source);
	printf(" alarm high level %d\n", pulse->alarm_high_level_4 << 4 | pulse->alarm_high_level_8);
	printf(" alarm low level %d\n", pulse->alarm_low_level_4 << 4 | pulse->alarm_low_level_8);

}

void print_spo2_config_packet_info(SPO2_config_t *spo2)
{
	printf("Dump spo2 config packet info:\n");
	printf(" packet size: %d\n", spo2->length_h<<8|spo2->length_l);
	printf(" alarm %s\n", spo2->alarm_onoff?"on":"off");
	printf(" alarm level %d\n", spo2->alarm_level);
	printf(" alarm high level %d\n", spo2->alarm_high_level_8);
	printf(" alarm low level %d\n", spo2->alarm_low_level_8);
}

void print_nibp_config_packet_info(NIBP_config_t *nibp)
{
	printf("Dump nibp config packet info:\n");
	printf(" packet size: %d\n", nibp->length_h<<8|nibp->length_l);
	printf(" mode %s\n", nibp->mode==0?"adult":(nibp->mode==1?"child":"baby"));
	if(nibp->mode == 0) {
		if(nibp->initial_pressure == 0)
			printf(" Initial pressure 150mmHg\n");
		else if(nibp->initial_pressure == 1)
			printf(" Initial pressure 160mmHg\n");
		else if(nibp->initial_pressure == 2)
			printf(" Initial pressure 180mmHg\n");
		else if(nibp->initial_pressure == 3)
			printf(" Initial pressure 195mmHg\n");
	}else if(nibp->mode == 1) {
		if(nibp->initial_pressure == 0)
			printf(" Initial pressure 130mmHg\n");
		else if(nibp->initial_pressure == 1)
			printf(" Initial pressure 150mmHg\n");
		else if(nibp->initial_pressure == 2)
			printf(" Initial pressure 160mmHg\n");
		else if(nibp->initial_pressure == 3)
			printf(" Initial pressure unknown\n");
		
	}else if(nibp->mode == 2) {
		if(nibp->initial_pressure == 0)
			printf(" Initial pressure 70mmHg\n");
		else if(nibp->initial_pressure == 1)
			printf(" Initial pressure 100mmHg\n");
		else if(nibp->initial_pressure == 2)
			printf(" Initial pressure 120mmHg\n");
		else if(nibp->initial_pressure == 3)
			printf(" Initial pressure unknown\n");
	}
	printf(" work mode %d\n", nibp->work_mode);
	printf(" measure interval %d\n", nibp->measure_interval);
	printf(" alarm %s\n", nibp->alarm_onoff?"on":"off");
	printf(" alarm level %d\n", nibp->alarm_level);
	printf(" alarm level unit %s\n", nibp->alarm_unit?"Kpa":"mmHg");
	printf(" Sys alarm high level %d\n", nibp->sys_alarm_high_level_8 << 8 | nibp->sys_alarm_high_level_4);
	printf(" Dia alarm high level %d\n", nibp->dia_alarm_high_level_8 << 8 | nibp->dia_alarm_high_level_4);
	printf(" Mean alarm high level %d\n", nibp->mean_alarm_high_level_8 << 8 | nibp->mean_alarm_high_level_4);
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
	if(ecg->module == MODULE_ECG5) {
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
	
	int decode_size, length, flag;
	config = (config_packet_t *)buffer;
	decode_size = 2; // add the length field size;
	length = (config->length_h<<8 | config->length_l);
	*consumed_bytes = 0;
	if(decode_size + length > size){
		return -1; //the packet size is larger than received data
	}
	*consumed_bytes = decode_size + (config->length_h<<8 | config->length_l);
	
	//the data integration check is done, no need to check more
	flag = 0;
	while(decode_size < size) {
		//printf("\ntype:%d,%d\n", buffer[decode_size],decode_size);
		switch(buffer[decode_size]) {
		case MODULE_PATIANT: //0xCE
			patiant = (PATIANT_config_t *)&buffer[decode_size];
			length = patiant->length_h << 8 | patiant->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("PATIANT config packet decoded, size %d\n", length);
			print_patiant_config_packet_info(patiant);
			flag = 1;
		break;
		
		case MODULE_TEMP1: //0xC1
		case MODULE_TEMP2: //0xC2
		case MODULE_TEMP3: //0xC3
		case MODULE_TEMP4: //0xC4
		case MODULE_TEMP5: //0xC5
		case MODULE_TEMP6: //0xC6
		case MODULE_TEMP7: //0xC7
		case MODULE_TEMP8: //0xC8
			temp = (TEMP_config_t *)&buffer[decode_size];
			length = temp->length_h << 8 | temp->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("TEMPEARTURE config packet decoded, size %d\n", length);
			print_temp_config_packet_info(temp);
			flag = 1;
		break;
		
		case MODULE_SETTINGS: //0xF3
			monitor = (MONITOR_config_t *)&buffer[decode_size];
			length = monitor->length_h << 8 | monitor->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("MONITOR config packet decoded, size %d\n", length);
			print_monitor_config_packet_info(monitor);
			flag = 1;
		break;
		
		case MODULE_RESP: //0xF8
			resp = (RESP_config_t *)&buffer[decode_size];
			length = resp->length_h << 8 | resp->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("RESP config packet decoded, size %d\n", length);
			print_resp_config_packet_info(resp);
			flag = 1;
		break;
		
		case MODULE_PULSE: //0xF9
			pulse = (PULSE_config_t *)&buffer[decode_size];
			length = pulse->length_h << 8 | pulse->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("PULSE config packet decoded, size %d\n", length);
			print_pulse_config_packet_info(pulse);
			flag = 1;
		break;
		
		case MODULE_SPO2: //0xFA
			spo2 = (SPO2_config_t *)&buffer[decode_size];
			length = spo2->length_h << 8 | spo2->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("SPO2 config packet decoded, size %d\n", length);
			print_spo2_config_packet_info(spo2);
			flag = 1;
		break;
		
		case MODULE_NIBP: //0xFB
			nibp = (NIBP_config_t *)&buffer[decode_size];
			length = nibp->length_h << 8 | nibp->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("NIBP config packet decoded, size %d\n", length);
			print_nibp_config_packet_info(nibp);
			flag = 1;
		break;
		
		case MODULE_ECG3: //0xFE
		case MODULE_ECG5: //0xFD
		case MODULE_ECG12: //0xFC
			ecg = (ECG_config_t *)&buffer[decode_size];
			length = ecg->length_h << 8 | ecg->length_l;
			decode_size += 3; // type and length field
			decode_size += length;
			printf("ECG config packet decoded, size %d\n", length);
			print_ecg_config_packet_info(ecg);
			flag = 1;
		break;
		
		default:
			printf("Unsupported Module(=%d) Config!\n", buffer[decode_size]);
		break;
		}
		if(flag == 0) {
			printf("No valid config packet decoded, break while loop\n");
		}
	}
	return 0;
}
/********End of config packet(type 0xD4) decode *************/


/********command packet(type 0xDA) decode *************/
int blt_decode_command_request_data(unsigned char *buffer, unsigned int size,
	command_master_request_data_t **request_data,
	int *consumed_bytes)
{
	command_master_request_data_t *p;
	
    if(size >= 2) {
        p = (command_master_request_data_t *)buffer;
        printf("command master request data, port %d\n", p->port_h<<8|p->port_l);
        *consumed_bytes = 2;
		*request_data = p;
        return 0;
    }else{
        printf("Incomplete master request command packet!\n");
    }
    return -1;
}

int blt_decode_command_response_request(unsigned char *buffer, unsigned int size,
	command_slave_response_request_t **response_request,
	int *consumed_bytes)
{
	command_slave_response_request_t *p;
    if(size >= 1) {
        p = (command_slave_response_request_t *)buffer;
        printf("command respomse request data %d\n", p->ack);
        *consumed_bytes = 1;
		*response_request = p;
        return 0;
    }else{
        printf("Incomplete response request command packet!\n");
    }
    return -1;
}

int blt_decode_command_master_request_data_resend(unsigned char *buffer, unsigned int size,
	command_master_request_data_resend_t **requst_data_resend,
	int *consumed_bytes)
{
	command_master_request_data_resend_t *p;
    if(size >= 1) {
        p = (command_master_request_data_resend_t *)buffer;
        printf("command master request data, packet %d\n", p->packet_number);
		*requst_data_resend = p;
        *consumed_bytes = 1;
        return 0;
    }else{
        printf("Incomplete master request command packet!\n");
    }
    return -1;
}

int blt_decode_command_slave_response_data_not_exist(unsigned char *buffer, unsigned int size, 
	command_slave_response_data_not_exist_t **response_data_not_exist,
	int *consumed_bytes)
{
	command_slave_response_data_not_exist_t *p;
    if(size >= 1) {
        p = (command_slave_response_data_not_exist_t *)buffer;
        printf("command slave response data not exist, packet %d\n", p->packet_number);
		*response_data_not_exist = p;
        *consumed_bytes = 1;
        return 0;
    }else{
        printf("Incomplete master request data packet!\n");
    }
    return -1;
}

int blt_decode_command(unsigned char *buffer, int size, command_packet_t *command, int *consumed_bytes)
{
	int ret = -1;
	int decode_size, length, cmd;
	command_master_request_data_t *request_data;
	command_slave_response_request_t *response_request;
	command_master_request_data_resend_t *requst_data_resend;
	command_slave_response_data_not_exist_t *response_data_not_exist;

	command = (command_packet_t *)buffer;
	decode_size = 3; // add the length field command direction, command high/low
	*consumed_bytes = 0;
	if(decode_size > size){
		return ret; //the packet size is larger than received data
	}
	cmd = command->command_h<<8|command->command_l;
	switch(cmd) {
		case COMMAND_MASTER_REQUEST_DATA:
		ret = blt_decode_command_request_data(buffer+decode_size, size-decode_size, &request_data, consumed_bytes);
		break;
		case COMMAND_SLAVE_RESPONSE_REQUEST:
		ret = blt_decode_command_response_request(buffer+decode_size, size-decode_size, &response_request, consumed_bytes);
		break;
		case COMMAND_MASTER_REQUEST_DATA_RESEND:
		ret = blt_decode_command_master_request_data_resend(buffer+decode_size, size-decode_size, &requst_data_resend, consumed_bytes);
		break;
		case COMMAND_SLAVE_RESPONSE_REQUEST_NOT_EXIST:
		ret = blt_decode_command_slave_response_data_not_exist(buffer+decode_size, size-decode_size, &response_data_not_exist, consumed_bytes);
		break;
		case COMMAND_MASTER_REQUEST_STOP:
		ret = 0;
		printf("master request stop command\n");
		break;
		case COMMAND_MASTER_REQUEST_SETTINGS:
		ret = 0;
		printf("master request setting command\n");
		break;
		case COMMAND_MASTER_REQUEST_NIBP:
		ret = 0;
		printf("master request NIBP command\n");
		break;
		case COMMAND_MASTER_REQUEST_NIBP_STOP:
		ret = 0;
		printf("master request NIBP stop command\n");
		break;
	}
	if(ret == 0) {
		*consumed_bytes += 3;
	}else{
		*consumed_bytes = 0;
	}
	return ret;
}
/********End of command packet(type 0xDA) decode *************/


/********data packet(type 0xD1) decode *************/
void print_data_packet_temp_info(TEMP_data_t *temp)
{
	printf("Dump TEMP data packet info:\n");
	printf(" packet length:%d\n", temp->length_h<<8|temp->length_l);
	printf(" 1st amp %s\n", temp->amplifer1?"wrong":"ok");
	printf(" 2nd amp %s\n", temp->amplifer2?"wrong":"ok");
	printf(" Detector %s\n", temp->dropped?"dropped":"ok");
	printf("%s out of low range\n", temp->out_low_range?"":" No");
	printf("%s out of high range\n", temp->out_high_range?"":" No");
	printf("%s alarm at low\n", temp->alarm_at_low?"":" No");
	printf("%s alarm at high\n", temp->alarm_at_high?"":" No");
	printf(" temperature %s\n", temp->invalid?"invalid":"valid");
	printf(" value %d\n", temp->temp_b4<<8|temp->temp_b8);
	printf(" alarm level %d\n", temp->alarm_level);
	printf("\n");
}

int blt_decode_data_temp(unsigned char *buffer, int size, TEMP_data_t **temp, int *consumed_bytes)
{
	int length;
	TEMP_data_t *p;
	
	p = (TEMP_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	//temp packet is fix length, no need to decode indeed
	*consumed_bytes = length+3;
	*temp = p;
	return 0;
}

void print_data_packet_nibp_info(NIBP_data_t *nibp)
{
	printf("Dump NIBP data packet info:\n");
	printf(" packet length:%d\n", nibp->length_h<<8|nibp->length_l);
	printf(" walarm level %d\n", nibp->alarm_level);
	printf(" work mode %d\n", nibp->work_mode);
	printf(" measure mode %d\n", nibp->measure_mode);
	printf(" %s working\n", nibp->working?"is":"not");
	printf(" %s result\n", nibp->resulting?"has":"no");
	if(nibp->resulting) {
		NIBP_payload_t *payload = (NIBP_payload_t *)nibp->data;
		printf(" measure %s\n", payload->error?"error":"ok");
		printf(" SYS value:%d\n", payload->sys_b1<<payload->sys_b8);
		printf(" DIA value:%d\n", payload->dia_b1<<payload->dia_b8);
		printf(" MAP value:%d\n", payload->map_b1<<payload->map_b8);
		printf(" PR value:%d\n", payload->pr_b1<<payload->pr_b8);
		printf(" SYS low %s\n", payload->sys_low_alarm?"alarming":"not alarm");
		printf(" SYS high %s\n", payload->sys_up_alarm?"alarming":"not alarm");
		printf(" DIA low %s\n", payload->dia_low_alarm?"alarming":"not alarm");
		printf(" DIA high %s\n", payload->dia_up_alarm?"alarming":"not alarm");
		printf(" MAP low %s\n", payload->map_low_alarm?"alarming":"not alarm");
		printf(" MAP high %s\n", payload->map_up_alarm?"alarming":"not alarm");
		printf(" PR low %s\n", payload->pr_low_alarm?"alarming":"not alarm");
		printf(" PR high %s\n", payload->pr_up_alarm?"alarming":"not alarm");
	}
	printf("\n");
}

int blt_decode_data_nibp(unsigned char *buffer, int size, NIBP_data_t **nibp, int *consumed_bytes)
{
	int length;
	NIBP_data_t *p;
	
	p = (NIBP_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	//nibp packet is 2Bytes + more 7B if resulting bit is set
	if(p->resulting && length < 9) { // contains more 7Bytes data
		return -1;
	}
	*nibp = p;
	*consumed_bytes = length+3;
	return 0;
}

void print_data_packet_resp_info(RESP_data_t *resp)
{
	printf("Dump RESP data packet info:\n");
	printf(" packet length:%d\n", resp->length_h<<8|resp->length_l);
	printf(" breath rate %d\n", resp->rate);
	printf(" breath rate %s\n", resp->invalid?"invalid":"valid");
	printf(" breath rate %s\n", resp->dropped?"dropped":"not dropped");
	printf("%s alarm at low\n", resp->alarm_at_low?"":" No");
	printf("%s alarm at high\n", resp->alarm_at_high?"":" No");
	printf("%s alarm at suffocate\n", resp->alarm_at_high?"":" No");
	printf(" alarm level %d\n", resp->alarm_level);
	printf(" breath rate source %d\n", resp->source);
	printf(" amplifier %d\n", resp->amplifer);
	printf("\n");
}

int blt_decode_data_resp(unsigned char *buffer, int size, RESP_data_t **resp, int *consumed_bytes)
{
	RESP_data_t *p;
	RESP_payload_t *payload;
	int length;
	
	p = (RESP_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	payload = (RESP_payload_t *)p->data;
	*resp = p;
	*consumed_bytes = length+3;
	return 0;
}

void print_data_packet_pulse_info(PULSE_data_t *pulse)
{
	printf("Dump PULSE data packet info:\n");
	printf(" packet length:%d\n", pulse->length_h<<8|pulse->length_l);
	printf(" pulse value %d\n", pulse->pulse_b1<<8|pulse->pulse_b8);
	printf("%s alarm at low\n", pulse->alarm_at_low?"":" No");
	printf("%s alarm at high\n", pulse->alarm_at_high?"":" No");
	printf(" pulse %s\n", pulse->invalid?"invalid":"valid");
	printf(" Detector %s\n", pulse->dropped?"dropped":"ok");
	printf(" alarm level %d\n", pulse->alarm_level);
	printf(" pulse source %d\n", pulse->source);

	printf("\n");
}

int blt_decode_data_pulse(unsigned char *buffer, int size, PULSE_data_t **pulse, int *consumed_bytes)
{
	PULSE_data_t *p;
	int length;
	
	p = (PULSE_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	//pulse packet is fix length, no need to decode indeed
	*consumed_bytes = length+3;
	*pulse = p;
	return 0;
}

void print_data_packet_ecg3_info(ECG3_data_t *ecg3)
{
	printf("Dump ECG3 data packet info:\n");
	printf(" packet length:%d\n", ecg3->length_h<<8|ecg3->length_l);
	printf(" heart beat rate:%d\n", ecg3->heat_rate_b9<<8|ecg3->heart_rate_b8);
	printf(" amplifier:%d\n", ecg3->amplifer);
	printf("%s alarm at low\n", ecg3->alarm_at_low?"":" No");
	printf("%s alarm at high\n", ecg3->alarm_at_high?"":" No");
	printf(" alarm level:%d\n", ecg3->alarm_level);
	printf(" channel:%d\n", ecg3->channel);
	printf(" ST value:%s%d(unit:0.01mv)\n", ecg3->st_sign?"-":"", ecg3->st_value_b2<<8|ecg3->st_value_b8);
	printf(" arrhythmia:%d\n", ecg3->arrhythmia);
	printf(" LA %s dropped\n",ecg3->LA_dropped?"":"not");
	printf(" LL %s dropped\n",ecg3->LL_dropped?"":"not");
	printf(" RA %s dropped\n",ecg3->RA_dropped?"":"not");
	printf(" II wave %s\n", ecg3->II_alarm?"alarm":"no alarm");
	printf(" heart beat rate %s\n", ecg3->heart_beat_invalid?"invalid":"valid");
	printf("\n");
}

int blt_decode_data_ecg3(unsigned char *buffer, int size, ECG3_data_t **ecg3, int *consumed_bytes)
{
	ECG3_data_t *p;
	ECG3_payload_t *payload;
	int length;
	
	p = (ECG3_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	payload = (ECG3_payload_t *)p->data;
	*consumed_bytes = length+3;
	*ecg3 = p;
	return 0;
}

void print_data_packet_ecg5_info(ECG5_data_t *ecg5)
{
	printf("Dump ECG5 data packet info\n");
	printf(" packet length:%d\n", ecg5->length_h<<8|ecg5->length_l);
	printf(" heart beat rate:%d\n", ecg5->heat_rate_b9<<8|ecg5->heart_rate_b8);
	printf(" amplifier:%d\n", ecg5->amplifer);
	printf("%s alarm at low\n", ecg5->alarm_at_low?"":" No");
	printf("%s alarm at high\n", ecg5->alarm_at_high?"":" No");
	printf(" alarm level:%d\n", ecg5->alarm_level);
	printf(" channel:%d\n", ecg5->channel);
	printf(" ECG %d\n", ecg5->ECG_3_5?3:5);
	printf(" ST value:%s%d(unit:0.01mv)\n", ecg5->st_sign?"-":"", ecg5->st_value_b2<<8|ecg5->st_value_b8);
	printf(" arrhythmia:%d\n", ecg5->arrhythmia);
	printf(" LA %s dropped\n",ecg5->LA_dropped?"":"not");
	printf(" LL %s dropped\n",ecg5->LL_dropped?"":"not");
	printf(" RA %s dropped\n",ecg5->RA_dropped?"":"not");
	printf(" II wave %s\n", ecg5->II_alarm?"alarm":"no alarm");
	printf(" heart beat rate %s\n", ecg5->heart_beat_invalid?"invalid":"valid");
	printf("\n");
}

int blt_decode_data_ecg5(unsigned char *buffer, int size, ECG5_data_t **ecg5, int *consumed_bytes)
{
	ECG5_data_t *p;
	ECG5_payload_t *payload;
	int length;
	
	p = (ECG5_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	printf("size=%d,length=%d\n", size, length);
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	payload = (ECG5_payload_t *)p->data;
	*consumed_bytes = length+3;
	*ecg5 = p;
	return 0;
}

void print_data_packet_ecg12_info(ECG12_data_t *ecg12)
{
	printf("Dump ECG12 data packet info:\n");
	printf(" packet length:%d\n", ecg12->length_h<<8|ecg12->length_l);
	printf(" heart beat rate:%d\n", ecg12->heat_rate_b9<<8|ecg12->heart_rate_b8);
	printf(" amplifier:%d\n", ecg12->amplifer);
	printf("%s alarm at low\n", ecg12->alarm_at_low?"":" No");
	printf("%s alarm at high\n", ecg12->alarm_at_high?"":" No");
	printf(" alarm level:%d\n", ecg12->alarm_level);
	printf(" channel:%d\n", ecg12->channel);
	printf(" ECG %d\n", ecg12->ECG_3_12?3:12);
	printf(" ST value:%s%d(unit:0.01mv)\n", ecg12->st_sign?"-":"", ecg12->st_value_b2<<8|ecg12->st_value_b8);
	printf(" arrhythmia:%d\n", ecg12->arrhythmia);
	printf(" LA %s dropped\n",ecg12->LA_dropped?"":"not");
	printf(" LL %s dropped\n",ecg12->LL_dropped?"":"not");
	printf(" RA %s dropped\n",ecg12->RA_dropped?"":"not");
	printf(" II wave %s\n", ecg12->II_alarm?"alarm":"no alarm");
	printf(" heart beat rate %s\n", ecg12->heart_beat_invalid?"invalid":"valid");
	printf("\n");
}


int blt_decode_data_ecg12(unsigned char *buffer, int size, ECG12_data_t **ecg12, int *consumed_bytes)
{
	ECG12_data_t *p;
	ECG12_payload_t *payload;
	int length;
	
	p = (ECG12_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	payload = (ECG12_payload_t *)p->data;
	*consumed_bytes = length+3;
	*ecg12 = p;
	return 0;
}

void print_data_packet_spo2_info(SPO2_data_t *spo2)
{
	printf("Dump SPO2 data packet info:\n");
	printf(" packet length:%d\n", spo2->length_h<<8|spo2->length_l);

	printf("\n");
}

int blt_decode_data_spo2(unsigned char *buffer, int size, SPO2_data_t **spo2, int *consumed_bytes)
{
	SPO2_data_t *p;
	SPO2_payload_t *payload;
	int length;
	
	p = (SPO2_data_t *)buffer;
	length = p->length_h<<8|p->length_l;
	*consumed_bytes = 0;
	if(length > size-3) {// exclude the ID/length 3 bytes
		return -1;
	}
	payload = (SPO2_payload_t *)p->data;
	*spo2 = p;
	*consumed_bytes = length+3;
	return 0;
}

int blt_decode_data(unsigned char *buffer, int size, packet_data_t *data, int *consumed_bytes)
{
	TEMP_data_t *temp;
	NIBP_data_t *nibp;
	PULSE_data_t *pulse;
	RESP_data_t *resp;
	ECG3_data_t *ecg3;
	ECG5_data_t *ecg5;
	ECG12_data_t *ecg12;
	SPO2_data_t *spo2;
	
    int decode_size, length, cmd, flag;
	data = (packet_data_t *)buffer;
	decode_size = 2; // add the length field
	length = (data->length_h<<8 | data->length_l);
	*consumed_bytes = 0;	
	//printf("decode data:%d, size:%d, length:%d\n", decode_size, size, length);	
	printf("data packet number:%d\n", buffer[decode_size]);
	if(decode_size + length > size){
		return -1; //the packet size is larger than received data
	
	}	
	//skip packet number
	decode_size = 3;
	//the data integration check is done, no need to check more
	flag = 0; // incase the data is garbage, need to break the while loop
	while(decode_size < size) {
		printf("module=%x, size=%d\n", buffer[decode_size], decode_size);
		switch(buffer[decode_size]) {
			case MODULE_TEMP1:
			case MODULE_TEMP2:
			case MODULE_TEMP3:
			case MODULE_TEMP4:
			case MODULE_TEMP5:
			case MODULE_TEMP6:
			case MODULE_TEMP7:
			case MODULE_TEMP8:
			if(blt_decode_data_temp(&buffer[decode_size], size-decode_size, &temp, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_temp_info(temp);
				flag = 1;
			}
			break;
			case MODULE_RESP:
			if(blt_decode_data_resp(&buffer[decode_size], size-decode_size, &resp, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_resp_info(resp);
				flag = 1;
			}
			break;
			case MODULE_PULSE:
			if(blt_decode_data_pulse(&buffer[decode_size], size-decode_size, &pulse, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_pulse_info(pulse);
				flag = 1;
			}
			break;
			case MODULE_SPO2:
			if(blt_decode_data_spo2(&buffer[decode_size], size-decode_size, &spo2, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_spo2_info(spo2);
				flag = 1;
			}
			break;
			case MODULE_NIBP:
			if(blt_decode_data_nibp(&buffer[decode_size], size-decode_size, &nibp, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_nibp_info(nibp);
				flag = 1;
			}
			break;
			case MODULE_ECG12:
			if(blt_decode_data_ecg12(&buffer[decode_size], size-decode_size, &ecg12, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_ecg12_info(ecg12);
				flag = 1;
			}
			break;
			case MODULE_ECG5:
			if(blt_decode_data_ecg5(&buffer[decode_size], size-decode_size, &ecg5, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_ecg5_info(ecg5);
				flag = 1;
			}
			break;
			case MODULE_ECG3:
			if(blt_decode_data_ecg3(&buffer[decode_size], size-decode_size, &ecg3, consumed_bytes) == 0) {
				decode_size += *consumed_bytes;
				print_data_packet_ecg3_info(ecg3);
				flag = 1;
			}
			break;
			//Undefined modules
			case MODULE_MONITOR_CONFIG:
			case MODULE_PATIANT:
			case MODULE_CONFIG:
			case MODULE_ICG:
			case MODULE_IBP1:
			case MODULE_IBP2:
			case MODULE_IBP3:
			case MODULE_IBP4:
			case MODULE_IBP5:
			case MODULE_IBP6:
			case MODULE_IBP7:
			case MODULE_IBP8:
			case MODULE_SETTINGS:
			case MODULE_CO2_1:
			case MODULE_CO:
			case MODULE_GAS:
			case MODULE_CO2_2:
			default:
			printf("Unsupported Module(=%d) Data!\n", data->data[0]);
			break;
		}
		if(flag == 0) {
			printf("No valid data packet decoded, break while loop\n");
		}
	}
	//ignore sub-packet decode consumed length result
	*consumed_bytes = decode_size;
    return 0;
}
/********End of data packet(type 0xD1) decode *************/

int blt_ecg_decode(unsigned char *buff, int buff_size)
{
	packet_t *pkt;
	broadcast_packet_t *broadcast;
	config_packet_t *config;
	command_packet_t *command;
	arrhy_packet_t *arrhy;
	packet_data_t *data;

	int i, decode_size, consumed;
	int length;

	printf("decode buffer: %p, size %d\n ", buff, buff_size);
#if 1
	for(i = 0; i < buff_size; i++) {
		printf("%02x ", buff[i]);
	}
#endif
	printf("\n");
	memcpy(ecg_buffer, buff, buff_size);

	pkt = (packet_t *)ecg_buffer;
	decode_size = PKT_HEADER_SIZE;
	if(buff_size < decode_size) {
		return 0;
	}

	if(pkt->header.sync == PACKET_SYNC_BYTE) {
		switch(pkt->header.type) {
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
			
			case PACKET_TYPE_ARRHY_INFO:
				if(blt_decode_arrhy(
					&ecg_buffer[decode_size],
					buff_size-decode_size,
					arrhy,
					&consumed) == 0) { // skip header
					return (decode_size+consumed);
				}else{
					return 0;
				}
			break;
			
			case PACKET_TYPE_DATA:
                if(blt_decode_data(
					&ecg_buffer[decode_size],
					buff_size-decode_size,
					data,
					&consumed) == 0) { // skip header
					return (decode_size+consumed);
				}else{
					return 0;
				}
			break;
			
			case PACKET_TYPE_SLAVE_TO_MASTER_SETTINGS:
			case PACKET_TYPE_MASTER_TO_SLAVE_SETTINGS:
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
			
			case PACKET_TYPE_COMMAND_RESPONSE:
				//process with command packet decode
				if(blt_decode_command(
					&ecg_buffer[decode_size],
					buff_size-decode_size,
					command,
					&consumed) == 0) {
					return (decode_size+consumed);
				}else{
					return 0;
				}
			break;
			default:
			break;
		}
	}

	return 0;
}
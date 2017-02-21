#ifndef ECG_BLT_H
#define ECG_BLT_H

#define PACKET_SYNC_BYTE (0xFF)

enum{
MACHINE_TYPE_ERROR = -1,
MACHINE_TYPE_SLAVE,
MACHINE_TYPE_MASTER,
MACHINE_TYPE_BROADCAST,
};

enum{
PACKET_TYPE_BROADCAST = 0xD0,
PACKET_TYPE_DATA = 0xD1,
PACKET_TYPE_ARRHY_INFO = 0xD3,
PACKET_TYPE_SLAVE_TO_MASTER_SETTINGS = 0xD4,
PACKET_TYPE_MASTER_TO_SLAVE_SETTINGS = 0xD5,
PACKET_TYPE_COMMAND_RESPONSE = 0xDA,
};

#define PKT_HEADER_SIZE (5)
typedef struct{
    union{
        unsigned char B0;
        unsigned char sync; // always == 0xFF
    }B0;
    union{
        unsigned char B1;
        unsigned char type;
    }B1;
    union{
        unsigned char B2;
        unsigned char source;
    }B2;
    union{
        unsigned char B3;
        unsigned char dest;
    }B3;
    union{
        unsigned char B4;
        unsigned char _reserved;
    }B4;
}packet_header_t;

enum{
COMMAND_DIRECTION_NULL,
COMMAND_DIRECTION_SLAVE_TO_MASTER,
COMMAND_DIRECTION_MASTER_TO_SLAVE,
COMMAND_DIRECTION_LAST
};

enum{
COMMAND_MASTER_REQUEST_DATA = 0x01,
COMMAND_SLAVE_RESPONSE_REQUEST = 0x02,
COMMAND_MASTER_REQUEST_STOP = 0x04,
COMMAND_MASTER_REQUEST_DATA_RESEND = 0x05,
COMMAND_SLAVE_RESPONSE_REQUEST_NOT_EXIST = 0x06,
COMMAND_MASTER_REQUEST_SETTINGS = 0x07,
COMMAND_MASTER_REQUEST_NIBP = 0x08,
COMMAND_MASTER_REQUEST_NIBP_STOP = 0x09,
};

typedef{
	unsigned char port_h;
	unsigned char port_l;
}command_request_data_t;

typedef struct{
    unsigned char module;
    unsigned char length;
    unsigned char data[0];
}command_data_t;

//PACKET_TYPE_COMMAND_RESPONSE
typedef struct{
    unsigned char B0;
    unsigned char direction;
    unsigned char command_h;
    unsigned char command_l;
    command_data_t data;
}command_packet_t;

typedef struct{
//Byte5
    unsigned int ECG3       :1;
    unsigned int ECG5       :1;
    unsigned int ECG12      :1;
    unsigned int NIBP       :1;
    unsigned int SPO2       :1;
    unsigned int PULSE      :1;
    unsigned int RESP       :1;
    unsigned int ICG        :1;

//Byte6
    unsigned int IBP1       :1;
    unsigned int IBP2       :1;
    unsigned int IBP3       :1;
    unsigned int IBP4       :1;
    unsigned int IBP5       :1;
    unsigned int IBP6       :1;
    unsigned int IBP7       :1;
    unsigned int IBP8       :1;

//Byte7
    unsigned int TEMP1      :1;
    unsigned int TEMP2      :1;
    unsigned int TEMP3      :1;
    unsigned int TEMP4      :1;
    unsigned int TEMP5      :1;
    unsigned int TEMP6      :1;
    unsigned int TEMP7      :1;
    unsigned int TEMP8      :1;

//Byte8
    unsigned int CO2        :1;
    unsigned int GAS        :1;
    unsigned int CO         :1;
    unsigned int            :5;

//Byte9
    unsigned int MINOR      :4;
    unsigned int MAJOR      :4;
}machine_config_t;

//PACKET_TYPE_BROADCAST
typedef struct{
    machine_config_t    config;
    unsigned char       status1;
    unsigned char       status2;
}broadcast_packet_t;

typedef struct{
    unsigned char   module; // ==0xFE
    unsigned char   length_h;
    unsigned char   length_l;
    unsigned char   heart_rate_b_0_7;
    unsigned int    alarm_level:2;
    unsigned int    alarm_at_high:1;
    unsigned int    alarm_at_low:1;
    unsigned int    amplify:3;
    unsigned int    heat_rate_b8:1;
}ECG3_data_t;

//PACKET_TYPE_DATA == 0xD1
typedef struct{
    unsigned char length_h;
    unsigned char length_l;
    unsigned char number;
    union{
        ECG3_data_t ecg3;
    }data;
}uplink_data_t;


enum{
	CONFIG_TYPE_ECG3 = 0xFE,
	CONFIG_TYPE_ECG5 = 0xFD,
	CONFIG_TYPE_ECG12 = 0xFC,
	CONFIG_TYPE_NIBP = 0xFB,
	CONFIG_TYPE_SPO2 = 0xFA,
	CONFIG_TYPE_PULSE = 0xF9,
	CONFIG_TYPE_RESP = 0xF8,
	CONFIG_TYPE_MONITOR = 0xF3,
	CONFIG_TYPE_TEMP1 = 0xC1,
	CONFIG_TYPE_TEMP2 = 0xC2,
	CONFIG_TYPE_TEMP3 = 0xC3,
	CONFIG_TYPE_TEMP4 = 0xC4,
	CONFIG_TYPE_TEMP5 = 0xC5,
	CONFIG_TYPE_TEMP6 = 0xC6,
	CONFIG_TYPE_TEMP7 = 0xC7,
	CONFIG_TYPE_TEMP8 = 0xC8,
	CONFIG_TYPE_PATIANT = 0xCE,
	
};


typedef struct{
	unsigned char type; //==0xFD or 0xFE or 0xFC
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int filter_drift:2;
	unsigned int filter_ac:1;
	unsigned int filter_muscle:1;
	unsigned int mode:2;
	unsigned int :2;
	//B4
	unsigned int channel:4;
	unsigned int ecg3_5:1; // only valid when type == 0xFD
	unsigned int :2;
	unsigned int qibofenxi:1;
	//B5
	unsigned int alarm_onoff:1;
	unsigned int alarm_level:2;
	unsigned int :5;
	//B6
	unsigned char alarm_high_level_8;
	//B7
	unsigned char alarm_low_level_8;
	//B8
	unsigned int alarm_high_level_4:4;
	unsigned int alarm_low_level_4:4;
	
}ECG_config_t;

typedef struct{
	unsigned char type; //==0xFA
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int alarm_onoff:1;
	unsigned int alarm_level:2;
	unsigned int :5;
	//B4
	unsigned char alarm_high_level_8;
	//B5
	unsigned char alarm_low_level_8;
}SPO2_config_t;

enum{
	PULSE_FROM_AUTO = 0,
	PULSE_FROM_SPO2,
	PULSE_FROM_IBP1,
	PULSE_FROM_IBP2,
	PULSE_FROM_IBP3,
	PULSE_FROM_IBP4,
	PULSE_FROM_IBP5,
	PULSE_FROM_IBP6,
	PULSE_FROM_IBP7,
	PULSE_FROM_IBP8,
	PULSE_FROM_UNKNOWN
};

typedef struct{
	unsigned char type; //==0xF9
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int alarm_onoff:1;
	unsigned int alarm_level:2;
	unsigned int source:4;
	unsigned int :1;
	//B4
	unsigned char alarm_high_level_8;
	//B5
	unsigned char alarm_low_level_8;
	//B6
	unsigned int alarm_high_level_4:4;
	unsigned int alarm_low_level_4:4;
}PULSE_config_t;


enum{
	RESP_FROM_AUTO = 0,
	RESP_FROM_ECG,
	RESP_FROM_CO2,
	RESP_FROM_UNKNOWN,
};

typedef struct{
	unsigned char type; //==0xF8
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int source:2;
	unsigned int :3;
	unsigned int amplify:3;
	//B4
	unsigned int suff_time:7;
	unsigned int suff_onoff:1;
	//B5
	unsigned int sense_value:7;
	unsigned int :1;
	//B6
	unsigned int alarm_onoff:1;
	unsigned int alarm_level:2;
	unsigned int :5;
	//B7
	unsigned char alarm_high_level_8;
	//B8
	unsigned char alarm_low_level_8;
}RESP_config_t;

typedef struct{
	unsigned char type; //==0xC1-0xC8
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int unit:2;
	unsigned int :6;
	//B4
	unsigned int alarm_onoff:1;
	unsigned int alarm_level:2;
	unsigned int :3;
	unsigned int alarm_high_level_1:1;
	unsigned int alarm_low_level_1:1;
	//B5
	unsigned char alarm_high_level_8;
	//B6
	unsigned char alarm_low_level_8;
}TEMP_config_t;

enum{
	NIBP_MODE_ADULT,
	NIBP_MODE_CHILD,
	NIBP_MODE_BABY,
	NIBP_MODE_UNKNOWN,
};

typedef struct{
	unsigned char type; //==0xFB
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int mode:2;
	unsigned int initial_pressure:2;
	unsigned int work_mode:3;
	unsigned int :1;
	//B4
	unsigned char measure_interval;
	//B5
	unsigned int alarm_onoff:1;
	unsigned int alarm_level:2;
	unsigned int alarm_unit:2;
	unsigned int :3;
	//B6
	unsigned char sys_alarm_high_level_8;
	//B7
	unsigned char sys_alarm_low_level_8;
	//B8
	unsigned char sys_alarm_high_level_4:4;
	unsigned char sys_alarm_low_level_4:4;
	//B9
	unsigned char dia_alarm_high_level_8;
	//B10
	unsigned char dia_alarm_low_level_8;
	//B11
	unsigned char dia_alarm_high_level_4:4;
	unsigned char dia_alarm_low_level_4:4;
	//B12
	unsigned char mean_alarm_high_level_8;
	//B13
	unsigned char mean_alarm_low_level_8;
	//B14
	unsigned char mean_alarm_high_level_4:4;
	unsigned char mean_alarm_low_level_4:4;
}NIBP_config_t;

typedef struct{
	unsigned char type; //==0xCE
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int age:7;
	unsigned int gender:1;
	//B4
	unsigned int weight_unit:1;
	unsigned int height_unit:1;
	unsigned int :2;
	unsigned int weight_4:4;
	//B5
	unsigned char weight_8;
	//B6
	unsigned char height;
	//B7-B12
	unsigned char room[6];
	//B13-B18
	unsigned char bed[6];
	//B19
	unsigned int patiant_type:2;
	unsigned int with_qiboqi:1;
	unsigned int :5;
	//B20
	unsigned char name[0];
}PATIANT_config_t;

typedef struct{
	unsigned char type; //==0xF3
	unsigned char length_h;
	unsigned char length_l;
	//B3
	unsigned int heart_beat_sound_level:3;
	unsigned int alarm_sound_level:3;
	unsigned int :2;
}MONITOR_config_t;

typedef struct{
	unsigned char length_h;
	unsigned char length_l;
	unsigned char data[0];
}config_packet_t;

typedef struct{
    packet_header_t     header;
    union{
        broadcast_packet_t  broadcast;
        command_packet_t    command;
		config_packet_t		 config;
    }body;
} __attribute__ ((packed)) packet_t;

inline unsigned char machine_typea(unsigned char type) {
    if(type >= 1 && type <= 66)
        return MACHINE_TYPE_SLAVE;
    if(type >= 241 && type <= 254)
            return MACHINE_TYPE_MASTER;
    if(type == 255)
            return MACHINE_TYPE_BROADCAST;
			
	return MACHINE_TYPE_ERROR;
}

extern packet_t *decoded_blt_pkt;
extern unsigned int packet_type;

int blt_ecg_decode(unsigned char *buff, unsigned int buff_size);
#endif
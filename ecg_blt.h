#ifndef ECG_BLT_H
#define ECG_BLT_H

#define PACKET_SYNC_BYTE (0xFF)

enum{
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
PACKET_TYPE_COMMAND_RESPONSE = 0xDa,
};

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
COMMAND_SLAVE_RESPONSE_DATA_REQUEST = 0x02,
COMMAND_MASTER_REQUEST_STOP = 0x04,
COMMAND_MASTER_REQUEST_DATA_RESEND = 0x05,
COMMAND_SLAVE_RESPONSE_DATA_REQUEST_NOT_EXIST = 0x06,
COMMAND_MASTER_REQUEST_SETTINGS = 0x07,
COMMAND_MASTER_REQUEST_NIBP = 0x08,
COMMAND_MASTER_REQUEST_NIBP_STOP = 0x09,
};

typedef struct{
    unsigned char module;
    unsigned char length;
    unsigned char data[0];
}command_data_t;

//PACKET_TYPE_COMMAND_RESPONSE
typedef struct{
    union{
        unsigned char B0;
        unsigned char direction;
    }B0;

    union{
        unsigned char B1;
        unsigned char command_h;
    }B1;

    union{
        unsigned char B2;
        unsigned char command_l;
    }B2;
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

typedef struct{
    packet_header_t     header;
    union{
        broadcast_packet_t  broadcast;
        command_packet_t    command;
    }body;
} __attribute__ ((packed)) packet_t;

inline unsigned char machine_typea(unsigned char type) {
    if(type >= 1 && type <= 66)
        return MACHINE_TYPE_SLAVE;
    if(type >= 241 && type <= 254)
            return MACHINE_TYPE_MASTER;
    if(type == 255)
            return MACHINE_TYPE_BROADCAST;
}

extern packet_t *decoded_blt_pkt;
extern unsigned int packet_type;
void print_broadcast_packet_info(broadcast_packet_t *broad);
int ecg_blt_decode(unsigned char *buff, unsigned int buff_size);
#endif
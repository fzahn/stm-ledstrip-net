#ifndef __ARTNET_PACKETS_H__
#define __ARTNET_PACKETS_H__

#include "stdint.h"

typedef enum artnet_OpCode_e {
    ARTNET_OP_POLL		= 0x2000,
    ARTNET_OP_POLL_REPLY	= 0x2100,
    ARTNET_OP_DIAG_DATA		= 0x2300,
    ARTNET_OP_COMMAND		= 0x2400,
    ARTNET_OP_DMX		= 0x5000,
    ARTNET_OP_NZS		= 0x5100,
    ARTNET_OP_ADDRESS		= 0x6000,
    ARTNET_OP_INPUT		= 0x7000,
    ARTNET_OP_TOD_REQUEST	= 0x8000,
    ARTNET_OP_TOD_DATA		= 0x8100,
    ARTNET_OP_TOD_CONTROL	= 0x8200,
    ARTNET_OP_RDM		= 0x8300,
    ARTNET_OP_RDM_SUB		= 0x8400,
    ARTNET_OP_VIDEO_SETUP	= 0xa010,
    ARTNET_OP_VIDEO_PALETTE	= 0xa020,
    ARTNET_OP_VIDEO_DATA	= 0xa040,
    ARTNET_OP_MAC_MASTER	= 0xf000,
    ARTNET_OP_MAC_SLAVE		= 0xf100,
    ARTNET_OP_FIRMWARE_MASTER	= 0xf200,
    ARTNET_OP_FIRMWARE_REPLY	= 0xf300,
    ARTNET_OP_FILE_TN_MASTER	= 0xf400,
    ARTNET_OP_FILE_FN_MASTER	= 0xf500,
    ARTNET_OP_FILE_FN_REPLY	= 0xf600,
    ARTNET_OP_IP_PROG		= 0xf800,
    ARTNET_OP_IP_PROG_REPLY	= 0xf900,
    ARTNET_OP_MEDIA		= 0x9000,
    ARTNET_OP_MEDIA_PATCH	= 0x9100,
    ARTNET_OP_MEDIA_CONTROL	= 0x9200,
    ARTNET_OP_MEDIA_CONTRL_REPLY= 0x9300,
    ARTNET_OP_TIME_CODE		= 0x9700,
    ARTNET_OP_TIME_SYNC		= 0x9800,
    ARTNET_OP_TRIGGER		= 0x9900,
    ARTNET_OP_DIRECTORY		= 0x9a00,
    ARTNET_OP_DIRECTORY_REPLY	= 0x9b00
} artnet_OpCode_t;

typedef enum artnet_StyleCode_e {
    ARTNET_ST_NODE       = 0x00,
    ARTNET_ST_CONTROLLER = 0x01,
    ARTNET_ST_MEDIA      = 0x02,
    ARTNET_ST_ROUTE      = 0x03,
    ARTNET_ST_BACKUP     = 0x04,
    ARTNET_ST_CONFIG     = 0x05,
    ARTNET_ST_VISULA     = 0x06    
} artnet_StyleCode_t;

typedef struct artnet_status1_s {
    uint8_t indicator_state                    :2;
    uint8_t port_address_programming_authority :2;
    uint8_t tobenamed                           :1;
    uint8_t normal_firmware_boot               :1;
    uint8_t rdm_en                             :1;
    uint8_t ubea                               :1;    
} artnet_status1_t; 


struct artnet_poll_s {
    uint8_t version_hi;
    uint8_t version_lo;
    uint8_t talk_to_me;
    uint8_t priority;
} __attribute__((packed));

typedef struct artnet_poll_s artnet_poll_t;

struct artnet_poll_reply_s {
    uint8_t ip_addr[4];
    uint16_t port;
    uint8_t version_hi;
    uint8_t version_lo;
    uint8_t net_switch;
    uint8_t sub_switch;
    uint8_t oem_hi;
    uint8_t oem_lo;
    uint8_t ubea_version;
    artnet_status1_t status1;
    char esta_man[2];
    char short_name[18];
    char long_name[64];
    char node_report[64];
    uint8_t num_ports_hi;
    uint8_t num_ports_lo;
    

} __attribute__((packed));

typedef struct artnet_poll_reply_s artnet_poll_reply_t;


typedef struct {
    uint8_t id[8];
    uint16_t op_code;
    union {
        artnet_poll_t poll;
        artnet_poll_reply_t poll_reply;
    } data;
} artnet_packet_t;


#endif /* __ARTNET_PACKETS_H__ */

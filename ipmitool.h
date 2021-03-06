//
// Created by keti on 2017. 10. 12..
//

#ifndef IPMI_IPMITOOL_H
#define IPMI_IPMITOOL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/stropts.h>

#include <inttypes.h>
//NETFN

#define IPMI_CHASSIS_NETFN      0x00
#define IPMI_BRIDGE_NETFN       0x02
#define IPMI_SENSOR_EVENT_NETFN     0x04
#define IPMI_APP_NETFN          0x06
#define IPMI_FIRMWARE_NETFN     0x08
#define IPMI_STORAGE_NETFN      0x0a
#define IPMI_TRANSPORT_NETFN        0x0c
#define IPMI_GROUP_EXTENSION_NETFN  0x2c
#define IPMI_OEM_GROUP_NETFN        0x2e

// CHASSIS COMMAND

#define IPMI_CHASSIS_CTL_POWER_DOWN     0x0
#define IPMI_CHASSIS_CTL_POWER_UP       0x1
#define IPMI_CHASSIS_CTL_POWER_CYCLE    0x2
#define IPMI_CHASSIS_CTL_HARD_RESET     0x3
#define IPMI_CHASSIS_CTL_PULSE_DIAG     0x4
#define IPMI_CHASSIS_CTL_ACPI_SOFT      0x5

#define IPMI_CHASSIS_POLICY_NO_CHANGE   0x3
#define IPMI_CHASSIS_POLICY_ALWAYS_ON   0x2
#define IPMI_CHASSIS_POLICY_PREVIOUS    0x1
#define IPMI_CHASSIS_POLICY_ALWAYS_OFF  0x0

#define IPMI_CHASSIS_POWER_STATUS       0x01
#define IPMI_CHASSIS_POWER_CTL          0x02

#define IPMI_BUF_SIZE 1024

//USER COMMAND

#define IPMI_SET_USER_ACCESS           0x43
#define IPMI_GET_USER_ACCESS           0x44
#define IPMI_SET_USER_NAME             0x45
#define IPMI_GET_USER_NAME             0x46
#define IPMI_SET_USER_PASSWORD         0x47
// LAN COMMAND

#define IPMI_LAN_SET_CONFIG	0x01
#define IPMI_LAN_GET_CONFIG	0x02
#define IPMI_LAN_SUSPEND_ARP	0x03
# define IPMI_LAN_SUSPEND_ARP_RESP (2)
# define IPMI_LAN_SUSPEND_ARP_GRAT (1)
#define IPMI_LAN_GET_STAT	0x04

#define IPMI_CHANNEL_NUMBER_MAX	0xe

#define IPMI_LANP_TIMEOUT		3
#define IPMI_LANP_RETRIES		10
#define IPMI_LANP_WRITE_UNLOCK		0
#define IPMI_LANP_WRITE_LOCK		1
#define IPMI_LANP_WRITE_COMMIT		2
#define IPMI_SESSION_AUTHTYPE_NONE      0x0
#define IPMI_SESSION_AUTHTYPE_MD2       0x1
#define IPMI_SESSION_AUTHTYPE_MD5   	0x2
#define IPMI_SESSION_AUTHTYPE_KEY	0x4
#define IPMI_SESSION_AUTHTYPE_PASSWORD	IPMI_SESSION_AUTHTYPE_KEY
#define IPMI_SESSION_AUTHTYPE_OEM       0x5
#define IPMI_SESSION_AUTHTYPE_RMCP_PLUS 0x6

//ETC
#define IPMI_SESSION_PRIV_UNSPECIFIED   0x0
#define IPMI_SESSION_PRIV_CALLBACK  0x1
#define IPMI_SESSION_PRIV_USER      0x2
#define IPMI_SESSION_PRIV_OPERATOR  0x3
#define IPMI_SESSION_PRIV_ADMIN     0x4
#define IPMI_SESSION_PRIV_OEM       0x5
#define SEND_MAX_PAYLOAD_SIZE       34  /* MAX payload */
#define RECV_MAX_PAYLOAD_SIZE       33  /* MAX payload */

#define IPMI_DEFAULT_PAYLOAD_SIZE   25

#define DEMO 1
# define IPMI_UID_MAX 10

#define dprintf(fmt, args...) fprintf(stderr, "\x1b[33m""[%s:%d:%s()]: " "\x1b[0m" fmt, \
		__FILE__, __LINE__, __func__, ##args)

int fd;

struct ipmi_rq {

	struct {
		uint8_t netfn:6;
		uint8_t lun:2;
		uint8_t cmd;
		uint8_t target_cmd;
		uint16_t data_len;
		uint8_t *data;
	} msg;

	//DEMO
	/*
	   struct {
	   unsigned char netfn;
	   unsigned char lun;
	   unsigned char cmd;              //byte 2
	   unsigned char data[];
	   }msg;
	 */
};

struct ipmi_rs {
	uint8_t ccode;
	uint8_t data[IPMI_BUF_SIZE];

	/*
	 * Looks like this is the length of the entire packet, including the RMCP
	 * stuff, then modified to be the length of the extra IPMI message data
	 */
	int data_len;

	struct {
		uint8_t netfn;
		uint8_t cmd;
		uint8_t seq;
		uint8_t lun;
	} msg;

	// DEMO
	/*
	   struct {
	   unsigned char netfn;
	   unsigned char lun;
	   unsigned char cmd;
	   unsigned char cc;
	   unsigned char data[];
	   }msg;
	 */
	struct {
		uint8_t authtype;
		uint32_t seq;
		uint32_t id;
		uint8_t bEncrypted; /* IPMI v2 only */
		uint8_t bAuthenticated; /* IPMI v2 only */
		uint8_t payloadtype;    /* IPMI v2 only */
		/* This is the total length of the payload or
		   IPMI message.  IPMI v2.0 requires this to
		   be 2 bytes.  Not really used for much. */
		uint16_t msglen;
	} session;

	/*
	 * A union of the different possible payload meta-data
	 */
	union {
		struct {
			uint8_t rq_addr;
			uint8_t netfn;
			uint8_t rq_lun;
			uint8_t rs_addr;
			uint8_t rq_seq;
			uint8_t rs_lun;
			uint8_t cmd;
		} ipmi_response;
		struct {
			uint8_t message_tag;
			uint8_t rakp_return_code;
			uint8_t max_priv_level;
			uint32_t console_id;
			uint32_t bmc_id;
			uint8_t auth_alg;
			uint8_t integrity_alg;
			uint8_t crypt_alg;
		} open_session_response;
		struct {
			uint8_t message_tag;
			uint8_t rakp_return_code;
			uint32_t console_id;
			uint8_t bmc_rand[16];   /* Random number generated by the BMC */
			uint8_t bmc_guid[16];
			uint8_t key_exchange_auth_code[20];
		} rakp2_message;
		struct {
			uint8_t message_tag;
			uint8_t rakp_return_code;
			uint32_t console_id;
			uint8_t integrity_check_value[20];
		} rakp4_message;
		struct {
			uint8_t packet_sequence_number;
			uint8_t acked_packet_number;
			uint8_t accepted_character_count;
			uint8_t is_nack;    /* bool */
			uint8_t transfer_unavailable;   /* bool */
			uint8_t sol_inactive;   /* bool */
			uint8_t transmit_overrun;   /* bool */
			uint8_t break_detected; /* bool */
		} sol_packet;

	} payload;
};


/* Setion 9.2 KCS Request Message Format */
typedef struct
{
	unsigned char netfn;  //byte 1 : netfn [8:2], lun[1:0]
	unsigned char lun;
	unsigned char cmd;      //byte 2
	unsigned char data[SEND_MAX_PAYLOAD_SIZE];       //byte 3:N
} ipmi_req_t;

// IPMI response Structure (IPMI/Section 9.3)
typedef struct
{
	unsigned char netfn;
	unsigned char lun;
	unsigned char cmd;
	unsigned char cc;
	unsigned char data[RECV_MAX_PAYLOAD_SIZE];
} ipmi_res_t;

typedef struct bmc_req {
	uint8_t fn;         /* netFn for command */
	uint8_t lun;            /* logical unit on responder */
	uint8_t cmd;            /* command */
	uint8_t datalength;     /* length of following data */
	uint8_t data[SEND_MAX_PAYLOAD_SIZE]; /* request data */
} bmc_req_t;

typedef struct bmc_rsp {
	uint8_t fn;         /* netFn for command */
	uint8_t lun;            /* logical unit on responder */
	uint8_t cmd;            /* command */
	uint8_t ccode;          /* completion code */
	uint8_t datalength;     /* Length */
	uint8_t data[RECV_MAX_PAYLOAD_SIZE]; /* response */
} bmc_rsp_t;

/*
 * the data structure for synchronous operation via ioctl (DEPRECATED)
 */
typedef struct bmc_reqrsp {
	bmc_req_t   req;            /* request half */
	bmc_rsp_t   rsp;            /* response half */
} bmc_reqrsp_t;


/* (22.27) Get and (22.26) Set User Access */
struct user_access_t {
	uint8_t callin_callback;
	uint8_t channel;
	uint8_t enabled_user_ids;
	uint8_t enable_status;
	uint8_t fixed_user_ids;
	uint8_t ipmi_messaging;
	uint8_t link_auth;
	uint8_t max_user_ids;
	uint8_t privilege_limit;
	uint8_t session_limit;
	uint8_t user_id;
};

/* (22.29) Get User Name */
struct user_name_t {
	uint8_t user_id;
	uint8_t user_name[17];
};

typedef enum IPMI_OEM {
	IPMI_OEM_UNKNOWN    = 0,
	IPMI_OEM_HP         = 11,
	IPMI_OEM_SUN        = 42,
	IPMI_OEM_NOKIA      = 94,
	IPMI_OEM_BULL       = 107,
	IPMI_OEM_HITACHI_116 = 116,
	IPMI_OEM_NEC        = 119,
	IPMI_OEM_TOSHIBA    = 186,
	IPMI_OEM_ERICSSON   = 193,
	IPMI_OEM_INTEL      = 343,
	IPMI_OEM_TATUNG     = 373,
	IPMI_OEM_HITACHI_399 = 399,
	IPMI_OEM_DELL       = 674,
	IPMI_OEM_LMC        = 2168,
	IPMI_OEM_RADISYS    = 4337,
	IPMI_OEM_BROADCOM   = 4413,
	IPMI_OEM_MAGNUM     = 5593,
	IPMI_OEM_TYAN       = 6653,
	IPMI_OEM_QUANTA     = 7244,
	IPMI_OEM_NEWISYS    = 9237,
	IPMI_OEM_ADVANTECH  = 10297,
	IPMI_OEM_FUJITSU_SIEMENS = 10368,
	IPMI_OEM_AVOCENT    = 10418,
	IPMI_OEM_PEPPERCON  = 10437,
	IPMI_OEM_SUPERMICRO = 10876,
	IPMI_OEM_OSA        = 11102,
	IPMI_OEM_GOOGLE     = 11129,
	IPMI_OEM_PICMG      = 12634,
	IPMI_OEM_RARITAN    = 13742,
	IPMI_OEM_KONTRON    = 15000,
	IPMI_OEM_PPS        = 16394,
	IPMI_OEM_AMI        = 20974,
	IPMI_OEM_NOKIA_SIEMENS_NETWORKS = 28458,
	IPMI_OEM_SUPERMICRO_47488 = 47488
} IPMI_OEM;

struct valstr {
	uint16_t val;
	const char * str;
};

struct ipmi_intf {
	char name[16];
	char desc[128];
	char *devfile;
	int fd;
	int opened;
	int abort;
	int noanswer;
	int picmg_avail;
	int vita_avail;
	IPMI_OEM manufacturer_id;

	//	struct ipmi_session_params ssn_params;
	//	struct ipmi_session * session;
	struct ipmi_oem_handle * oem;
	struct ipmi_cmd * cmdlist;
	uint8_t target_ipmb_addr;
	uint32_t my_addr;
	uint32_t target_addr;
	uint8_t target_lun;
	uint8_t target_channel;
	uint32_t transit_addr;
	uint8_t transit_channel;
	uint16_t max_request_data_size;
	uint16_t max_response_data_size;

	uint8_t devnum;

	int (*setup)(struct ipmi_intf * intf);
	int (*open)(struct ipmi_intf * intf);
	void (*close)(struct ipmi_intf * intf);
	struct ipmi_rs *(*sendrecv)(struct ipmi_intf * intf, struct ipmi_rq * req);
	int (*sendrsp)(struct ipmi_intf * intf, struct ipmi_rs * rsp);
	//	struct ipmi_rs *(*recv_sol)(struct ipmi_intf * intf);
	//	struct ipmi_rs *(*send_sol)(struct ipmi_intf * intf, struct ipmi_v2_payload * payload);
	int (*keepalive)(struct ipmi_intf * intf);
	int (*set_my_addr)(struct ipmi_intf * intf, uint8_t addr);
	void (*set_max_request_data_size)(struct ipmi_intf * intf, uint16_t size);
	void (*set_max_response_data_size)(struct ipmi_intf * intf, uint16_t size);
};

enum {
	IPMI_LANP_SET_IN_PROGRESS,
	IPMI_LANP_AUTH_TYPE,
	IPMI_LANP_AUTH_TYPE_ENABLE,
	IPMI_LANP_IP_ADDR,
	IPMI_LANP_IP_ADDR_SRC,
	IPMI_LANP_MAC_ADDR,            /* 5 */
	IPMI_LANP_SUBNET_MASK,
	IPMI_LANP_IP_HEADER,
	IPMI_LANP_PRI_RMCP_PORT,
	IPMI_LANP_SEC_RMCP_PORT,
	IPMI_LANP_BMC_ARP,             /* 10 */
	IPMI_LANP_GRAT_ARP,
	IPMI_LANP_DEF_GATEWAY_IP,
	IPMI_LANP_DEF_GATEWAY_MAC,
	IPMI_LANP_BAK_GATEWAY_IP,
	IPMI_LANP_BAK_GATEWAY_MAC,     /* 15 */
	IPMI_LANP_SNMP_STRING,
	IPMI_LANP_NUM_DEST,
	IPMI_LANP_DEST_TYPE,
	IPMI_LANP_DEST_ADDR,
	IPMI_LANP_VLAN_ID,             /* 20 */
	IPMI_LANP_VLAN_PRIORITY,
	IPMI_LANP_RMCP_CIPHER_SUPPORT,
	IPMI_LANP_RMCP_CIPHERS,
	IPMI_LANP_RMCP_PRIV_LEVELS,
	IPMI_LANP_VLAN_TAGS,
	IPMI_LANP_BAD_PASS_THRESH,
	IPMI_LANP_OEM_ALERT_STRING=96,
	IPMI_LANP_ALERT_RETRY=97,
	IPMI_LANP_UTC_OFFSET=98,
	IPMI_LANP_DHCP_SERVER_IP=192,
	IPMI_LANP_DHCP_SERVER_MAC=193,
	IPMI_LANP_DHCP_ENABLE=194,
	IPMI_LANP_CHAN_ACCESS_MODE=201,
};

static struct lan_param {
	int cmd;
	int size;
	char desc[24];
	uint8_t * data;
	int data_len;
} ipmi_lan_params[] __attribute__((unused)) = {
	{ IPMI_LANP_SET_IN_PROGRESS,	1,	"Set in Progress", NULL, 0 },
	{ IPMI_LANP_AUTH_TYPE,		1,	"Auth Type Support", NULL, 0 },
	{ IPMI_LANP_AUTH_TYPE_ENABLE,	5,	"Auth Type Enable", NULL, 0	},
	{ IPMI_LANP_IP_ADDR,		4,	"IP Address", NULL, 0 },
	{ IPMI_LANP_IP_ADDR_SRC,	1,	"IP Address Source", NULL, 0 },
	{ IPMI_LANP_MAC_ADDR,		6,	"MAC Address", NULL, 0 }, /* 5 */
	{ IPMI_LANP_SUBNET_MASK,	4,	"Subnet Mask", NULL, 0 },
	{ IPMI_LANP_IP_HEADER,		3,	"IP Header", NULL, 0 },
	{ IPMI_LANP_PRI_RMCP_PORT,	2,	"Primary RMCP Port", NULL, 0 },
	{ IPMI_LANP_SEC_RMCP_PORT,	2,	"Secondary RMCP Port", NULL, 0 },
	{ IPMI_LANP_BMC_ARP,		1,	"BMC ARP Control", NULL, 0}, /* 10 */
	{ IPMI_LANP_GRAT_ARP,		1,	"Gratituous ARP Intrvl", NULL, 0 },
	{ IPMI_LANP_DEF_GATEWAY_IP,	4,	"Default Gateway IP", NULL, 0 },
	{ IPMI_LANP_DEF_GATEWAY_MAC,	6,	"Default Gateway MAC", NULL, 0 },
	{ IPMI_LANP_BAK_GATEWAY_IP,	4,	"Backup Gateway IP", NULL, 0 },
	{ IPMI_LANP_BAK_GATEWAY_MAC,	6,	"Backup Gateway MAC", NULL, 0 }, /* 15 */
	{ IPMI_LANP_SNMP_STRING,	18,	"SNMP Community String", NULL, 0 },
	{ IPMI_LANP_NUM_DEST,		1,	"Number of Destinations", NULL, 0 },
	{ IPMI_LANP_DEST_TYPE,		4,	"Destination Type", NULL, 0 },
	{ IPMI_LANP_DEST_ADDR,		13,	"Destination Addresses", NULL, 0 },
	{ IPMI_LANP_VLAN_ID,		2,	"802.1q VLAN ID", NULL, 0 }, /* 20 */
	{ IPMI_LANP_VLAN_PRIORITY,	1,	"802.1q VLAN Priority", NULL, 0 },
	{ IPMI_LANP_RMCP_CIPHER_SUPPORT,1,	"RMCP+ Cipher Suite Count", NULL, 0 },
	{ IPMI_LANP_RMCP_CIPHERS,	16,	"RMCP+ Cipher Suites", NULL, 0 },
	{ IPMI_LANP_RMCP_PRIV_LEVELS,	9,	"Cipher Suite Priv Max", NULL, 0 },
	{ IPMI_LANP_BAD_PASS_THRESH,	6,	"Bad Password Threshold", NULL, 0 },
	{ IPMI_LANP_OEM_ALERT_STRING,	28,	"OEM Alert String", NULL, 0 }, /* 25 */
	{ IPMI_LANP_ALERT_RETRY,	1,	"Alert Retry Algorithm", NULL, 0 },
	{ IPMI_LANP_UTC_OFFSET,		3,	"UTC Offset", NULL, 0 },
	{ IPMI_LANP_DHCP_SERVER_IP,	4,	"DHCP Server IP", NULL, 0 },
	{ IPMI_LANP_DHCP_SERVER_MAC,	6,	"DHDP Server MAC", NULL, 0},
	{ IPMI_LANP_DHCP_ENABLE,	1,	"DHCP Enable", NULL, 0 }, /* 30 */
	{ IPMI_LANP_CHAN_ACCESS_MODE,	2,	"Channel Access Mode", NULL, 0 },
	{ -1 }
};

const struct valstr completion_code_vals[] = { 
	{ 0x00, "Command completed normally" },
	{ 0xc0, "Node busy" },
	{ 0xc1, "Invalid command" },
	{ 0xc2, "Invalid command on LUN" },
	{ 0xc3, "Timeout" },  
	{ 0xc4, "Out of space" },
	{ 0xc5, "Reservation cancelled or invalid" },
	{ 0xc6, "Request data truncated" },
	{ 0xc7, "Request data length invalid" },
	{ 0xc8, "Request data field length limit exceeded" },
	{ 0xc9, "Parameter out of range" },
	{ 0xca, "Cannot return number of requested data bytes" },
	{ 0xcb, "Requested sensor, data, or record not found" },
	{ 0xcc, "Invalid data field in request" },
	{ 0xcd, "Command illegal for specified sensor or record type" },
	{ 0xce, "Command response could not be provided" },
	{ 0xcf, "Cannot execute duplicated request" },
	{ 0xd0, "SDR Repository in update mode" },
	{ 0xd1, "Device firmeware in update mode" },
	{ 0xd2, "BMC initialization in progress" },
	{ 0xd3, "Destination unavailable" },
	{ 0xd4, "Insufficient privilege level" },
	{ 0xd5, "Command not supported in present state" },
	{ 0xd6, "Cannot execute command, command disabled" },
	{ 0xff, "Unspecified error" },
	{ 0x00, NULL }
}; 


const struct valstr entity_device_type_vals[] = {
	{ 0x00, "Reserved" },
	{ 0x01, "Reserved" },
	{ 0x02, "DS1624 temperature sensor" },
	{ 0x03, "DS1621 temperature sensor" },
	{ 0x04, "LM75 Temperature Sensor" },
	{ 0x05, "Heceta ASIC" },
	{ 0x06, "Reserved" },
	{ 0x07, "Reserved" },
	{ 0x08, "EEPROM, 24C01" },
	{ 0x09, "EEPROM, 24C02" },
	{ 0x0a, "EEPROM, 24C04" },
	{ 0x0b, "EEPROM, 24C08" },
	{ 0x0c, "EEPROM, 24C16" },
	{ 0x0d, "EEPROM, 24C17" },
	{ 0x0e, "EEPROM, 24C32" },
	{ 0x0f, "EEPROM, 24C64" },
	{ 0x1000, "IPMI FRU Inventory" },
	{ 0x1001, "DIMM Memory ID" },
	{ 0x1002, "IPMI FRU Inventory" },
	{ 0x1003, "System Processor Cartridge FRU" },
	{ 0x11, "Reserved" },
	{ 0x12, "Reserved" },
	{ 0x13, "Reserved" },
	{ 0x14, "PCF 8570 256 byte RAM" },
	{ 0x15, "PCF 8573 clock/calendar" },
	{ 0x16, "PCF 8574A I/O Port" },
	{ 0x17, "PCF 8583 clock/calendar" }, 
	{ 0x18, "PCF 8593 clock/calendar" },
	{ 0x19, "Clock calendar" },
	{ 0x1a, "PCF 8591 A/D, D/A Converter" },
	{ 0x1b, "I/O Port" },
	{ 0x1c, "A/D Converter" },
	{ 0x1d, "D/A Converter" },
	{ 0x1e, "A/D, D/A Converter" },
	{ 0x1f, "LCD Controller/Driver" },
	{ 0x20, "Core Logic (Chip set) Device" },
	{ 0x21, "LMC6874 Intelligent Battery controller" },
	{ 0x22, "Intelligent Batter controller" },
	{ 0x23, "Combo Management ASIC" },
	{ 0x24, "Maxim 1617 Temperature Sensor" },
	{ 0xbf, "Other/Unspecified" },
	{ 0x00, NULL },
};

struct oemvalstr {
	uint32_t oem;
	uint16_t val;
	const char * str;
}; 
const struct oemvalstr ipmi_oem_sdr_type_vals[] = {
	/* Keep OEM grouped together */
	{ IPMI_OEM_KONTRON , 0xC0 , "OEM Firmware Info" },
	{ IPMI_OEM_KONTRON , 0xC2 , "OEM Init Agent" },
	{ IPMI_OEM_KONTRON , 0xC3 , "OEM IPMBL Link State" },
	{ IPMI_OEM_KONTRON , 0xC4 , "OEM Board Reset" },
	{ IPMI_OEM_KONTRON , 0xC5 , "OEM FRU Information Agent" },
	{ IPMI_OEM_KONTRON , 0xC6 , "OEM POST Value Sensor" },
	{ IPMI_OEM_KONTRON , 0xC7 , "OEM FWUM Status" },
	{ IPMI_OEM_KONTRON , 0xC8 , "OEM Switch Mngt Software Status" },
	{ IPMI_OEM_KONTRON , 0xC9 , "OEM OEM Diagnostic Status" },
	{ IPMI_OEM_KONTRON , 0xCA , "OEM Component Firmware Upgrade" },
	{ IPMI_OEM_KONTRON , 0xCB , "OEM FRU Over Current" },
	{ IPMI_OEM_KONTRON , 0xCC , "OEM FRU Sensor Error" },
	{ IPMI_OEM_KONTRON , 0xCD , "OEM FRU Power Denied" },
	{ IPMI_OEM_KONTRON , 0xCE , "OEM Reserved" },
	{ IPMI_OEM_KONTRON , 0xCF , "OEM Board Reset" },
	{ IPMI_OEM_KONTRON , 0xD0 , "OEM Clock Resource Control" },
	{ IPMI_OEM_KONTRON , 0xD1 , "OEM Power State" },
	{ IPMI_OEM_KONTRON , 0xD2 , "OEM FRU Mngt Power Failure" },
	{ IPMI_OEM_KONTRON , 0xD3 , "OEM Jumper Status" },
	{ IPMI_OEM_KONTRON , 0xF2 , "OEM RTM Module Hotswap" },         

	{ IPMI_OEM_PICMG   , 0xF0 , "PICMG FRU Hotswap" },
	{ IPMI_OEM_PICMG   , 0xF1 , "PICMG IPMB0 Link State" },
	{ IPMI_OEM_PICMG   , 0xF2 , "PICMG Module Hotswap" },

	{ 0xffffff,            0x00,  NULL }
};

struct ipm_devid_rsp {
	uint8_t device_id;
	uint8_t device_revision;
	uint8_t fw_rev1;
	uint8_t fw_rev2;
	uint8_t ipmi_version;
	uint8_t adtl_device_support;
	uint8_t manufacturer_id[3];
	uint8_t product_id[2];
	uint8_t aux_fw_rev[4];
};


#ifdef ORG
static void dump_request(bmc_req_t *request)
{
	int i;

	printf("BMC req.fn         : 0x%x\n", request->fn);
	printf("BMC req.lun        : 0x%x\n", request->lun);
	printf("BMC req.cmd        : 0x%x\n", request->cmd);
	printf("BMC req.datalength : 0x%x\n", request->datalength);
	printf("BMC req.data       : ");

	if (request->datalength > 0) {
		for (i = 0; i < request->datalength; i++)
			printf("0x%x ", request->data[i]);
	} else {
		printf("<NONE>");
	}
	printf("\n");
}

static void dump_response(bmc_rsp_t *response)
{
	int i;

	printf("BMC rsp.fn         : 0x%x\n", response->fn);
	printf("BMC rsp.lun        : 0x%x\n", response->lun);
	printf("BMC rsp.cmd        : 0x%x\n", response->cmd);
	printf("BMC rsp.ccode      : 0x%x\n", response->ccode);
	printf("BMC rsp.datalength : 0x%x\n", response->datalength);
	printf("BMC rsp.data       : ");

	if (response->datalength > 0) {
		for (i = 0; i < response->datalength; i++)
			printf("0x%x ", response->data[i]);
	} else {
		printf("<NONE>");
	}
	printf("\n");
}
#elif DEMO
static void dump_request(ipmi_req_t *request)
{
	int i = 0;
	printf("\n");
	printf("req.fn         : 0x%x\n", request->netfn);
	printf("req.lun         : 0x%x\n", request->lun);
	printf("req.cmd        : 0x%x\n", request->cmd);
	printf("req.data       : ");

	for(i = 0 ; i < sizeof(request->data) ; i ++){
		printf("0x%x ", request->data[i++]);
	}
	printf("\n");
}

static void dump_response(ipmi_res_t *response)
{
	int i = 0;
	printf("\n");
	printf("rsp.msgfn      : 0x%x\n", response->netfn);
	printf("rsp.lun         : 0x%x\n", response->lun);
	printf("rsp.cmd        : 0x%x\n", response->cmd);
	printf("rsp.ccode      : 0x%x\n", response->cc);
	printf("rsp.data       : ");

	for(i = 0 ; i < sizeof(response->data) ; i++){
		printf("0x%x ", response->data[i++]);
	}
	printf("\n");
}
#endif


static struct ipmi_rs * ipmi_bmc_send_cmd_ioctl(struct ipmi_intf *intf, struct ipmi_rq *req);
struct ipmi_intf ipmi_bmc_intf = {
name:       "bmc",
			desc:       "IPMI v2.0 BMC interface",
			open:       NULL,
			close:      NULL,
			sendrecv:   ipmi_bmc_send_cmd_ioctl};



const struct valstr ipmi_privlvl_vals[] = {
	{ IPMI_SESSION_PRIV_CALLBACK,   "CALLBACK" },
	{ IPMI_SESSION_PRIV_USER,       "USER" },
	{ IPMI_SESSION_PRIV_OPERATOR,   "OPERATOR" },
	{ IPMI_SESSION_PRIV_ADMIN,      "ADMINISTRATOR" },
	{ IPMI_SESSION_PRIV_OEM,        "OEM" },
	{ 0xF,                          "NO ACCESS" },
	{ 0xFF,                         NULL },
};

const char * val2str(uint16_t val, const struct valstr *vs)
{
	static char un_str[32];
	int i;

	for (i = 0; vs[i].str != NULL; i++) {
		if (vs[i].val == val)
			return vs[i].str;
	}

	memset(un_str, 0, 32);
	snprintf(un_str, 32, "Unknown (0x%02X)", val);

	return un_str;
}

const char * oemval2str(uint32_t oem, uint16_t val, const struct oemvalstr *vs)
{
	static char un_str[32];
	int i;

	for (i = 0; vs[i].oem != 0xffffff &&  vs[i].str != NULL; i++) {
		/* FIXME: for now on we assume PICMG capability on all IANAs */
		if ( (vs[i].oem == oem || vs[i].oem == IPMI_OEM_PICMG) &&
				vs[i].val == val ) { 
			return vs[i].str;
		}   
	}   

	memset(un_str, 0, 32);
	snprintf(un_str, 32, "Unknown (0x%X)", val);

	return un_str;
}


void printbuf(const uint8_t * buf, int len, const char * desc)
{
	int i;

	if (len <= 0)
		return;
	/*   
		 if (verbose < 1)
		 return;
	 */
	fprintf(stderr, "%s (%d bytes)\n", desc, len);
	for (i=0; i<len; i++) {
		if (((i%16) == 0) && (i != 0))
			fprintf(stderr, "\n");
		fprintf(stderr, " %2.2x", buf[i]);
	}    
	fprintf(stderr, "\n");
}


static struct ipmi_rs * ipmi_bmc_send_cmd_ioctl(struct ipmi_intf *intf, struct ipmi_rq *req)
{
	struct strioctl istr;
	static struct bmc_reqrsp reqrsp;
	static struct ipmi_rs rsp;
	int res_len, req_len;
	int fdread, fdwrite;

#ifdef ORG
	memset(&reqrsp, 0, sizeof (reqrsp));
	reqrsp.req.fn = req->msg.netfn;
	reqrsp.req.lun = 0;
	reqrsp.req.cmd = req->msg.cmd;
	reqrsp.req.datalength = req->msg.data_len;
	memcpy(reqrsp.req.data, req->msg.data, req->msg.data_len);
	reqrsp.rsp.datalength = RECV_MAX_PAYLOAD_SIZE;

	istr.ic_cmd = IOCTL_IPMI_KCS_ACTION;
	istr.ic_timout = 0;
	istr.ic_dp = (char *)&reqrsp;
	istr.ic_len = sizeof (struct bmc_reqrsp);

	//printf("--\n");
	//dump_request(&reqrsp.req);
	//printf("--\n");


	if (ioctl(fd, I_STR, &istr) < 0) {
		perror("BMC IOCTL: I_STR");
		return (NULL);
	}
	//req_len = write(fd, &reqrsp.req, sizeof(reqrsp.req));
	//res_len = read(fd, &reqrsp.rsp, sizeof(reqrsp.rsp));
	if((req_len = write(fd, req, sizeof(struct ipmi_rq))) < 0) {
		perror("write error : ");
	}
	if((res_len = read(fd, &rsp, sizeof(struct ipmi_rs))) < 0) {
		perror("read error : ");
	}
	//dump_response(&reqrsp.rsp);
#elif DEMO
	static ipmi_req_t rq;
	static ipmi_res_t rs;

	//dprintf("DEMO START!\n");
	if( (fdwrite = open("/etc/ipmi_write", O_WRONLY | O_TRUNC)) < 0) {
		perror("fdwrite error : ");
	}

	//dprintf("MEMSET\n");
	memset(&rq, 0, sizeof(ipmi_req_t));
	rq.netfn = req->msg.netfn;
	rq.lun = req->msg.lun;
	rq.cmd = req->msg.cmd;
	///dprintf("MEMCPY\n");
	memcpy(rq.data, req->msg.data, req->msg.data_len);
	dump_request(&rq);

	//dprintf("WRITE START!\n");
	if((req_len = write(fdwrite, &rq, sizeof(ipmi_req_t))) < 0) {
		perror("write error : ");
	}
	close(fdwrite);
	system("rm -r /etc/ipmi_write");
	system("touch /etc/ipmi_write");
	sleep(2);
	//dprintf("READ START!\n");
	if( (fdread = open("/etc/kcs_write", O_RDONLY)) < 0) {
		perror("fdread error : ");
	}
	if((res_len = read(fdread, &rs, sizeof(ipmi_res_t))) < 0) {
		perror("read error : ");
	}
	dump_response(&rs);
	close(fdread);

#endif

	memset(&rsp, 0, sizeof (struct ipmi_rs));
#ifdef ORG
	rsp.ccode = reqrsp.rsp.ccode;
	rsp.data_len = reqrsp.rsp.datalength;

	/* Decrement for sizeof lun, cmd and ccode */
	rsp.data_len -= 3;

	if (!rsp.ccode && (rsp.data_len > 0))
		memcpy(rsp.data, reqrsp.rsp.data, rsp.data_len);
#elif DEMO
	rsp.ccode = rs.cc;
	rsp.data_len = res_len;

	/* Decrement for sizeof lun, cmd and ccode */
	rsp.data_len -= 3;

	if (!rsp.ccode && (rsp.data_len > 0))
		memcpy(rsp.data, rs.data, rsp.data_len);
#endif
	return (&rsp);
}

/* print_valstr_2col  -  print value string list in two columns to log or stdout
 *
 * @vs:     value string list to print
 * @title:  name of this value string list
 * @loglevel:   what log level to print, -1 for stdout
 */
	void
print_valstr_2col(const struct valstr * vs, const char * title, int loglevel)
{
	int i;               

	if (vs == NULL)
		return;

	if (title != NULL) {
		if (loglevel < 0)
			printf("\n%s:\n\n", title);
		else 
			printf("\n%s:\n", title); 
	}    

	for (i = 0; vs[i].str != NULL; i++) {
		if (vs[i+1].str == NULL) {
			/* last one */
			if (loglevel < 0) {
				printf("  %4d  %-32s\n", vs[i].val, vs[i].str);
			} else {
				printf("  %4d  %-32s\n", vs[i].val, vs[i].str);
			}
		}
		else {
			if (loglevel < 0) {
				printf("  %4d  %-32s    %4d  %-32s\n",
						vs[i].val, vs[i].str, vs[i+1].val, vs[i+1].str);
			} else {
				printf("  %4d  %-32s    %4d  %-32s\n",
						vs[i].val, vs[i].str, vs[i+1].val, vs[i+1].str);
			}
			i++;
		}
	}    

	if (loglevel < 0)
		printf("\n");
}

/* str2ulong - safely convert string to uint64_t
 *
 * @str: source string to convert from
 * @ulng_ptr: pointer where to store result
 *
 * returns zero on success
 * returns (-1) if one of args is NULL, (-2) invalid input, (-3) for *flow
 */
int str2ulong(const char * str, uint64_t * ulng_ptr)
{
	char * end_ptr = 0;
	if (!str || !ulng_ptr)
		return (-1);

	*ulng_ptr = 0;
	errno = 0;
	*ulng_ptr = strtoul(str, &end_ptr, 0); 

	if (*end_ptr != '\0')
		return (-2);

	if (errno != 0)
		return (-3);

	return 0;
} /* str2ulong(...) */

/* str2uchar - safely convert string to uint8
 *
 * @str: source string to convert from
 * @uchr_ptr: pointer where to store result
 *
 * returns zero on success
 * returns (-1) if one of args is NULL, (-2) or (-3) if conversion fails
 */
int str2uchar(const char * str, uint8_t * uchr_ptr)
{
	int rc = (-3);
	uint64_t arg_ulong = 0;
	if (!str || !uchr_ptr)
		return (-1);

	if ( (rc = str2ulong(str, &arg_ulong)) != 0 ) {
		*uchr_ptr = 0;
		return rc;
	}

	if (arg_ulong > UINT8_MAX)
		return (-3);

	*uchr_ptr = (uint8_t)arg_ulong;
	return 0;
} /* str2uchar(...) */

#ifndef __maxlen
# define __maxlen(a, b) ({ int x=strlen(a); int y=strlen(b); (x > y) ? x : y;})
#endif


	uint16_t
ipmi_intf_get_max_response_data_size(struct ipmi_intf * intf)
{
	int16_t size;

	size = intf->max_response_data_size;

	/* check if response size is not specified */
	if (!size) {
		/*
		 * The IPMB standard overall message length for <91>non -bridging<92>
		 * messages is specified as 32 bytes, maximum, including slave
		 * address. This sets the upper limit for typical IPMI messages.
		 * With the exception of messages used for bridging messages to
		 * other busses or interfaces (e.g. Master Write-Read and Send Message)
		 * IPMI messages should be designed to fit within this 32-byte maximum.
		 * In order to support bridging, the Master Write -Read and Send Message
		 * commands are allowed to exceed the 32-byte maximum transaction on IPMB
		 */

		size = IPMI_DEFAULT_PAYLOAD_SIZE; /* response length with subtracted header and checksum byte */

		/* check if message is forwarded */
		if (intf->target_addr && intf->target_addr != intf->my_addr) {
			/* add Send Message header size */
			size += 7;
		}
	}       

	/* check if message is forwarded */
	if (intf->target_addr && intf->target_addr != intf->my_addr) {
		/*  
		 * Some IPMI controllers like PICMG AMC Carriers embed responses
		 * to the forwarded messages into the Send Message response.
		 * In order to be sure that the response is not truncated,
		 * subtract the internal message header size.
		 */
		size -= 8;

		/*  
		 * Check that forwarded response is not greater
		 * than the default payload size.
		 */
		if (size > IPMI_DEFAULT_PAYLOAD_SIZE) {
			size = IPMI_DEFAULT_PAYLOAD_SIZE;
		}

		/* check for double bridging */
		if (intf->transit_addr && intf->transit_addr != intf->target_addr) {
			/* subtract inner send message header size */
			size -= 8;
		}
	}

	/* check for underflow */
	if (size < 0) {
		return 0;
	}

	return size;
}

#endif //IPMI_IPMITOOL_H

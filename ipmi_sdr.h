/*
   Created by keti on 2017. 10. 23..
 */

#ifndef IPMI_IPMI_SDR_H
#define IPMI_IPMI_SDR_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ipmitool.h"
/* ========================= ipmi_sdr.c ================================= */

#define BMC_GET_DEVICE_ID   0x01
#define BMC_COLD_RESET      0x02
#define BMC_WARM_RESET      0x03
#define BMC_GET_SELF_TEST   0x04
#define BMC_RESET_WATCHDOG_TIMER    0x22
#define BMC_SET_WATCHDOG_TIMER  0x24
#define BMC_GET_WATCHDOG_TIMER  0x25
#define BMC_SET_GLOBAL_ENABLES  0x2e
#define BMC_GET_GLOBAL_ENABLES  0x2f
#define BMC_GET_GUID        0x37

#define GET_DEVICE_SDR_INFO      0x20
#define GET_DEVICE_SDR           0x21
#define GET_SENSOR_FACTORS      0x23
#define GET_SENSOR_FACTORS      0x23
#define SET_SENSOR_HYSTERESIS   0x24
#define GET_SENSOR_HYSTERESIS   0x25
#define SET_SENSOR_THRESHOLDS   0x26
#define GET_SENSOR_THRESHOLDS   0x27
#define SET_SENSOR_EVENT_ENABLE 0x28
#define GET_SENSOR_EVENT_ENABLE 0x29
#define GET_SENSOR_EVENT_STATUS 0x2b
#define GET_SENSOR_READING  0x2d
#define GET_SENSOR_TYPE     0x2f

#define IPMI_NETFN_CHASSIS      0x0
#define IPMI_NETFN_BRIDGE       0x2
#define IPMI_NETFN_SE           0x4
#define IPMI_NETFN_APP          0x6
#define IPMI_NETFN_FIRMWARE     0x8
#define IPMI_NETFN_STORAGE      0xa
#define IPMI_NETFN_TRANSPORT    0xc
#define IPMI_NETFN_PICMG        0x2C
#define IPMI_NETFN_DCGRP        0x2C
#define IPMI_NETFN_OEM      0x2E
#define IPMI_NETFN_ISOL         0x34
#define IPMI_NETFN_TSOL         0x30

#define IPMI_BMC_SLAVE_ADDR     0x20
#define IPMI_REMOTE_SWID        0x81

#define GET_SDR_REPO_INFO   0x20
#define GET_SDR_ALLOC_INFO  0x21

#define MAX_NUM_SLOT  128

#define IPM_DEV_DEVICE_ID_SDR_MASK     (0x80)   /* 1 = provides SDRs      */
#define IPM_DEV_DEVICE_ID_REV_MASK     (0x0F)   /* BCD-enoded             */

#define IPM_DEV_FWREV1_AVAIL_MASK      (0x80)   /* 0 = normal operation   */
#define IPM_DEV_FWREV1_MAJOR_MASK      (0x3f)   /* Major rev, BCD-encoded */

#define IPM_DEV_IPMI_VER_MAJOR_MASK    (0x0F)   /* Major rev, BCD-encoded */
#define IPM_DEV_IPMI_VER_MINOR_MASK    (0xF0)   /* Minor rev, BCD-encoded */
#define IPM_DEV_IPMI_VER_MINOR_SHIFT   (4)  /* Minor rev shift        */
#define IPM_DEV_IPMI_VERSION_MAJOR(x) \
	(x & IPM_DEV_IPMI_VER_MAJOR_MASK)
#define IPM_DEV_IPMI_VERSION_MINOR(x) \
	((x & IPM_DEV_IPMI_VER_MINOR_MASK) >> IPM_DEV_IPMI_VER_MINOR_SHIFT)

#define IPM_DEV_MANUFACTURER_ID(x) \
	((uint32_t) ((x[2] & 0x0F) << 16 | x[1] << 8 | x[0]))

//#define ipmi_open_file_read(file)   ipmi_open_file(file, 0)
//#define ipmi_open_file_write(file)  ipmi_open_file(file, 1)

const struct valstr entity_id_vals[] = {
	{ 0x00, "Unspecified" },
	{ 0x01, "Other" },
	{ 0x02, "Unknown" },
	{ 0x03, "Processor" },
	{ 0x04, "Disk or Disk Bay" },
	{ 0x05, "Peripheral Bay" },
	{ 0x06, "System Management Module" },
	{ 0x07, "System Board" },
	{ 0x08, "Memory Module" },
	{ 0x09, "Processor Module" },
	{ 0x0a, "Power Supply" },
	{ 0x0b, "Add-in Card" },
	{ 0x0c, "Front Panel Board" },
	{ 0x0d, "Back Panel Board" },
	{ 0x0e, "Power System Board" },
	{ 0x0f, "Drive Backplane" },
	{ 0x10, "System Internal Expansion Board" }, 
	{ 0x11, "Other System Board" },
	{ 0x12, "Processor Board" },
	{ 0x13, "Power Unit" },
	{ 0x14, "Power Module" },
	{ 0x15, "Power Management" },
	{ 0x16, "Chassis Back Panel Board" },
	{ 0x17, "System Chassis" },
	{ 0x18, "Sub-Chassis" },
	{ 0x19, "Other Chassis Board" },
	{ 0x1a, "Disk Drive Bay" },
	{ 0x1b, "Peripheral Bay" },
	{ 0x1c, "Device Bay" },
	{ 0x1d, "Fan Device" },
	{ 0x1e, "Cooling Unit" },
	{ 0x1f, "Cable/Interconnect" },
	{ 0x20, "Memory Device" },
	{ 0x21, "System Management Software" },
	{ 0x22, "BIOS" },
	{ 0x23, "Operating System" },
	{ 0x24, "System Bus" },
	{ 0x25, "Group" }, 
	{ 0x26, "Remote Management Device" },
	{ 0x27, "External Environment" },
	{ 0x28, "Battery" },
	{ 0x29, "Processing Blade" },
	{ 0x2A, "Connectivity Switch" },
	{ 0x2B, "Processor/Memory Module" },
	{ 0x2C, "I/O Module" },
	{ 0x2D, "Processor/IO Module" },
	{ 0x2E, "Management Controller Firmware" },
	{ 0x2F, "IPMI Channel" },
	{ 0x30, "PCI Bus" },
	{ 0x31, "PCI Express Bus" },
	{ 0x32, "SCSI Bus (parallel)" },
	{ 0x33, "SATA/SAS Bus" },
	{ 0x34, "Processor/Front-Side Bus" },
	{ 0x35, "Real Time Clock(RTC)" },
	{ 0x36, "Reserved" },
	{ 0x37, "Air Inlet" },
	{ 0x38, "Reserved" },
	{ 0x39, "Reserved" },
	{ 0x3A, "Reserved" },
	{ 0x3B, "Reserved" },
	{ 0x3C, "Reserved" },
	{ 0x3D, "Reserved" },
	{ 0x3E, "Reserved" },
	{ 0x3F, "Reserved" },
	{ 0x40, "Air Inlet" },
	{ 0x41, "Processor" },
	{ 0x42, "Baseboard/Main System Board" },
	/* PICMG */
	{ 0xA0, "PICMG Front Board" },
	{ 0xC0, "PICMG Rear Transition Module" },
	{ 0xC1, "PICMG AdvancedMC Module" },
	{ 0xF0, "PICMG Shelf Management Controller" },
	{ 0xF1, "PICMG Filtration Unit" },
	{ 0xF2, "PICMG Shelf FRU Information" },
	{ 0xF3, "PICMG Alarm Panel" },
	{ 0x00, NULL },
};

/* sensor type codes (IPMI v1.5 table 36.3) 
   / Updated to v2.0 Table 42-3, Sensor Type Codes */
#define SENSOR_TYPE_MAX 0x2C
static const char *sensor_type_desc[] __attribute__ ((unused)) = {
	"reserved", 
	"Temperature", "Voltage", "Current", "Fan",
	"Physical Security", "Platform Security", "Processor",
	"Power Supply", "Power Unit", "Cooling Device", "Other",
	"Memory", "Drive Slot / Bay", "POST Memory Resize",
	"System Firmwares", "Event Logging Disabled", "Watchdog1",      
	"System Event", "Critical Interrupt", "Button",
	"Module / Board", "Microcontroller", "Add-in Card",
	"Chassis", "Chip Set", "Other FRU", "Cable / Interconnect",
	"Terminator", "System Boot Initiated", "Boot Error",
	"OS Boot", "OS Critical Stop", "Slot / Connector",
	"System ACPI Power State", "Watchdog2", "Platform Alert",
	"Entity Presence", "Monitor ASIC", "LAN",
	"Management Subsys Health", "Battery", "Session Audit",
	"Version Change", "FRU State" };


#define GET_SDR     0x23
struct sdr_get_rq {
	uint16_t reserve_id;    /* reservation ID */
	uint16_t id;        /* record ID */
	uint8_t offset;     /* offset into SDR */
#define GET_SDR_ENTIRE_RECORD   0xff
	uint8_t length;     /* length to read */
};
struct sdr_get_rs {
	uint16_t next;      /* next record id */
	uint16_t id;        /* record ID */
	uint8_t version;    /* SDR version (51h) */
#define SDR_RECORD_TYPE_FULL_SENSOR     0x01
#define SDR_RECORD_TYPE_COMPACT_SENSOR      0x02
#define SDR_RECORD_TYPE_EVENTONLY_SENSOR    0x03
#define SDR_RECORD_TYPE_ENTITY_ASSOC        0x08
#define SDR_RECORD_TYPE_DEVICE_ENTITY_ASSOC 0x09
#define SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR  0x10
#define SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR  0x11
#define SDR_RECORD_TYPE_MC_DEVICE_LOCATOR   0x12
#define SDR_RECORD_TYPE_MC_CONFIRMATION     0x13
#define SDR_RECORD_TYPE_BMC_MSG_CHANNEL_INFO    0x14
#define SDR_RECORD_TYPE_OEM         0xc0
	uint8_t type;       /* record type */
	uint8_t length;     /* remaining record bytes */
};
struct sdr_record_list {
	uint16_t id; 
	uint8_t version;
	uint8_t type;
	uint8_t length;
	uint8_t *raw;
	struct sdr_record_list *next;
	union {
		struct sdr_record_common_sensor *common;
		struct sdr_record_full_sensor *full;
		struct sdr_record_compact_sensor *compact;
		struct sdr_record_eventonly_sensor *eventonly;
		struct sdr_record_generic_locator *genloc;
		struct sdr_record_fru_locator *fruloc;
		struct sdr_record_mc_locator *mcloc;
		struct sdr_record_entity_assoc *entassoc;
		struct sdr_record_oem *oem;
	}record;
};
struct entity_id {   
	uint8_t id;         /* physical entity id */ 
#if WORDS_BIGENDIAN      
	uint8_t logical     : 1;    /* physical/logical */
	uint8_t instance    : 7;    /* instance number */
#else                
	uint8_t instance    : 7;    /* instance number */
	uint8_t logical     : 1;    /* physical/logical */
#endif               
};

struct ipmi_sdr_iterator {
	uint16_t reservation;
	int total;
	int next;
	int use_built_in;
};


struct sdr_record_oem {
	uint8_t *data;
	int data_len;
};
struct sdrr_queue {
	struct sdr_record_list *head;
	struct sdr_record_list *tail;
}; 

struct get_sdr_repository_info_rsp {
	uint8_t sdr_version;
	uint8_t record_count_lsb;
	uint8_t record_count_msb;
	uint8_t free_space[2];
	uint8_t most_recent_addition_timestamp[4];
	uint8_t most_recent_erase_timestamp[4];
#if WORDS_BIGENDIAN 
	uint8_t overflow_flag:1;
	uint8_t modal_update_support:2;
	uint8_t __reserved1:1;
	uint8_t delete_sdr_supported:1;
	uint8_t partial_add_sdr_supported:1;
	uint8_t reserve_sdr_repository_supported:1;
	uint8_t get_sdr_repository_allo_info_supported:1;
#else   
	uint8_t get_sdr_repository_allo_info_supported:1;
	uint8_t reserve_sdr_repository_supported:1;
	uint8_t partial_add_sdr_supported:1;
	uint8_t delete_sdr_supported:1;
	uint8_t __reserved1:1;
	uint8_t modal_update_support:2;
	uint8_t overflow_flag:1;
#endif
};

/* builtin (device) sdrs support */
struct sdr_device_info_rs {
	unsigned char count;    /* number of records */
	unsigned char flags;    /* flags */
	unsigned char popChangeInd[3];  /* free space in SDR */
};

struct sdr_repo_info_rs {
	uint8_t version;    /* SDR version (51h) */
	uint16_t count;     /* number of records */
	uint16_t free;      /* free space in SDR */
	uint32_t add_stamp; /* last add timestamp */
	uint32_t erase_stamp;   /* last del timestamp */
	uint8_t op_support; /* supported operations */
};
#define GET_SDR_RESERVE_REPO    0x22
struct sdr_reserve_repo_rs {
	uint16_t reserve_id;    /* reservation ID */
} ;

struct sdr_record_eventonly_sensor {
	struct {
		uint8_t owner_id;
#if WORDS_BIGENDIAN
		uint8_t channel:4;  /* channel number */
		uint8_t fru_owner:2;    /* fru device owner lun */
		uint8_t lun:2;  /* sensor owner lun */
#else   
		uint8_t lun:2;  /* sensor owner lun */
		uint8_t fru_owner:2;    /* fru device owner lun */
		uint8_t channel:4;  /* channel number */
#endif 
		uint8_t sensor_num; /* unique sensor number */
	}keys;

	struct entity_id entity; 

	uint8_t sensor_type;    /* sensor type */
	uint8_t event_type; /* event/reading type code */

	struct {
#if WORDS_BIGENDIAN
		uint8_t __reserved:2;
		uint8_t mod_type:2;
		uint8_t count:4;
#else
		uint8_t count:4;
		uint8_t mod_type:2;
		uint8_t __reserved:2;
#endif
#if WORDS_BIGENDIAN
		uint8_t entity_inst:1;
		uint8_t mod_offset:7;
#else
		uint8_t mod_offset:7;     
		uint8_t entity_inst:1;
#endif
	} share;

	uint8_t __reserved;
	uint8_t oem;        /* reserved for OEM use */
	uint8_t id_code;    /* sensor ID string type/length code */
	uint8_t id_string[16];  /* sensor ID string bytes, only if id_code != 0 */

};

struct sdr_record_generic_locator {
	uint8_t dev_access_addr;
	uint8_t dev_slave_addr;
#if WORDS_BIGENDIAN
	uint8_t channel_num:3;
	uint8_t lun:2;
	uint8_t bus:3;
#else
	uint8_t bus:3;
	uint8_t lun:2;
	uint8_t channel_num:3;
#endif
#if WORDS_BIGENDIAN
	uint8_t addr_span:3;
	uint8_t __reserved1:5;
#else
	uint8_t __reserved1:5;
	uint8_t addr_span:3;
#endif
	uint8_t __reserved2;
	uint8_t dev_type;
	uint8_t dev_type_modifier;
	struct entity_id entity;
	uint8_t oem;
	uint8_t id_code;
	uint8_t id_string[16];
};

struct sdr_record_fru_locator {
	uint8_t dev_slave_addr;
	uint8_t device_id;
#if WORDS_BIGENDIAN
	uint8_t logical:1;
	uint8_t __reserved2:2;
	uint8_t lun:2;
	uint8_t bus:3;
#else
	uint8_t bus:3;
	uint8_t lun:2;
	uint8_t __reserved2:2;
	uint8_t logical:1;
#endif
#if WORDS_BIGENDIAN
	uint8_t channel_num:4;
	uint8_t __reserved3:4;
#else
	uint8_t __reserved3:4;
	uint8_t channel_num:4;
#endif
	uint8_t __reserved4;
	uint8_t dev_type;
	uint8_t dev_type_modifier;
	struct entity_id entity;
	uint8_t oem;
	uint8_t id_code;
	uint8_t id_string[16];
};

struct sdr_record_mc_locator {
	uint8_t dev_slave_addr;
#if WORDS_BIGENDIAN
	uint8_t __reserved2:4;
	uint8_t channel_num:4;
#else   
	uint8_t channel_num:4;
	uint8_t __reserved2:4; 
#endif
#if WORDS_BIGENDIAN
	uint8_t pwr_state_notif:3;
	uint8_t __reserved3:1;
	uint8_t global_init:4;
#else              
	uint8_t global_init:4;
	uint8_t __reserved3:1;
	uint8_t pwr_state_notif:3;
#endif             
	uint8_t dev_support;
	uint8_t __reserved4[3];
	struct entity_id entity;
	uint8_t oem;   
	uint8_t id_code;
	uint8_t id_string[16];
};

struct sdr_record_entity_assoc {
	struct entity_id entity;    /* container entity ID and instance */
	struct {
#if WORDS_BIGENDIAN
		uint8_t isrange:1;
		uint8_t islinked:1;
		uint8_t isaccessable:1;
		uint8_t __reserved:5;
#else
		uint8_t __reserved:5;
		uint8_t isaccessable:1;
		uint8_t islinked:1;
		uint8_t isrange:1;
#endif
	} flags;
	uint8_t entity_id_1;    /* entity ID 1    |  range 1 entity */
	uint8_t entity_inst_1;  /* entity inst 1  |  range 1 first instance */
	uint8_t entity_id_2;    /* entity ID 2    |  range 1 entity */
	uint8_t entity_inst_2;  /* entity inst 2  |  range 1 last instance */
	uint8_t entity_id_3;    /* entity ID 3    |  range 2 entity */
	uint8_t entity_inst_3;  /* entity inst 3  |  range 2 first instance */
	uint8_t entity_id_4;    /* entity ID 4    |  range 2 entity */
	uint8_t entity_inst_4;  /* entity inst 4  |  range 2 last instance */
};

int verbose = 0;        
int csv_output = 0; 
static int use_built_in;    /* Uses DeviceSDRs instead of SDRR */
static int sdr_max_read_len = 0;
static int sdr_extended = 0;
static long sdriana = 0;

static struct sdr_record_list *sdr_list_head = NULL;
static struct sdr_record_list *sdr_list_tail = NULL;
static struct ipmi_sdr_iterator *sdr_list_itr = NULL;

void printf_sdr_usage();
int ipmi_sdr_print_sdr(struct ipmi_intf *intf, uint8_t type);
struct ipmi_sdr_iterator * ipmi_sdr_start(struct ipmi_intf *intf, int use_builtin);
int ipmi_sdr_get_reservation(struct ipmi_intf *intf, int use_builtin, uint16_t * reserve_id);
struct sdr_get_rs * ipmi_sdr_get_next_header(struct ipmi_intf *intf, struct ipmi_sdr_iterator *itr);
static struct sdr_get_rs * ipmi_sdr_get_header(struct ipmi_intf *intf, struct ipmi_sdr_iterator *itr);
uint8_t * ipmi_sdr_get_record(struct ipmi_intf * intf, struct sdr_get_rs * header, struct ipmi_sdr_iterator * itr);
int ipmi_sdr_print_listentry(struct ipmi_intf *intf, struct sdr_record_list *entry);
int ipmi_sdr_print_rawentry(struct ipmi_intf *intf, uint8_t type, uint8_t * raw, int len);

int ipmi_sdr_print_sensor_eventonly(struct ipmi_intf *intf, struct sdr_record_eventonly_sensor *sensor);
int ipmi_sdr_print_sensor_generic_locator(struct ipmi_intf *intf, struct sdr_record_generic_locator *dev);
int ipmi_sdr_print_sensor_fru_locator(struct ipmi_intf *intf, struct sdr_record_fru_locator *fru);
int ipmi_sdr_print_sensor_mc_locator(struct ipmi_intf *intf, struct sdr_record_mc_locator *mc);
int ipmi_sdr_print_sensor_entity_assoc(struct ipmi_intf *intf, struct sdr_record_entity_assoc *assoc);
static int ipmi_sdr_print_sensor_oem(struct ipmi_intf *intf, struct sdr_record_oem *oem);
static int ipmi_sdr_print_sensor_oem_intel(struct ipmi_intf *intf, struct sdr_record_oem *oem);

int ipmi_sdr_print_type(struct ipmi_intf *intf, char *type);
int ipmi_sdr_print_entity(struct ipmi_intf *intf, char *entitystr);
int ipmi_sdr_print_info(struct ipmi_intf *intf);
static int ipmi_sdr_print_entry_byid(struct ipmi_intf *intf, int argc, char **argv);
static int ipmi_sdr_dump_bin(struct ipmi_intf *intf, const char *ofile);
int ipmi_sdr_add_from_sensors(struct ipmi_intf *intf, int maxslot);
int ipmi_sdr_add_from_file(struct ipmi_intf *intf, const char *ifile);
int ipmi_sdr_add_from_list(struct ipmi_intf *intf, const char *rangeList);

/* ipmi_sdr_main  -  top-level handler for SDR subsystem
 *
 * @intf:   ipmi interface
 * @argc:   number of arguments
 * @argv:   argument list
 *
 * returns 0 on success
 * returns -1 on error
 */
	int
ipmi_sdr_main(struct ipmi_intf *intf, int argc, char **argv)
{
	int rc = 0;

	/* initialize random numbers used later */
	srand(time(NULL));

	if (argc == 2)
		return ipmi_sdr_print_sdr(intf, 0xfe);
	else if (strncmp(argv[2], "help", 4) == 0) {
		printf_sdr_usage();
	}
	else if (strncmp(argv[2], "list", 4) == 0
			|| strncmp(argv[2], "elist", 5) == 0) {

		if (strncmp(argv[2], "elist", 5) == 0)
			sdr_extended = 1;
		else
			sdr_extended = 0;

		if (argc <= 3)
			rc = ipmi_sdr_print_sdr(intf, 0xfe);
		else if (strncmp(argv[3], "all", 3) == 0)
			rc = ipmi_sdr_print_sdr(intf, 0xff);
		else if (strncmp(argv[3], "full", 4) == 0)
			rc = ipmi_sdr_print_sdr(intf,
					SDR_RECORD_TYPE_FULL_SENSOR);
		else if (strncmp(argv[3], "compact", 7) == 0)
			rc = ipmi_sdr_print_sdr(intf,
					SDR_RECORD_TYPE_COMPACT_SENSOR);
		else if (strncmp(argv[3], "event", 5) == 0)
			rc = ipmi_sdr_print_sdr(intf,
					SDR_RECORD_TYPE_EVENTONLY_SENSOR);
		else if (strncmp(argv[3], "mcloc", 5) == 0)
			rc = ipmi_sdr_print_sdr(intf,
					SDR_RECORD_TYPE_MC_DEVICE_LOCATOR);
		else if (strncmp(argv[3], "fru", 3) == 0)
			rc = ipmi_sdr_print_sdr(intf,
					SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR);
		else if (strncmp(argv[3], "generic", 7) == 0)
			rc = ipmi_sdr_print_sdr(intf,
					SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR);
		else if (strcmp(argv[3], "help") == 0) {
			printf( "usage: sdr %s [all|full|compact|event|mcloc|fru|generic]", argv[2]);
			return 0;
		}
		else {
			printf("Invalid SDR %s command: %s", argv[2], argv[3]);
			printf("usage: sdr %s [all|full|compact|event|mcloc|fru|generic]", argv[2]);
			return (-1);
		}

	} /*else if (strncmp(argv[0], "type", 4) == 0) {
		sdr_extended = 1;
		rc = ipmi_sdr_print_type(intf, argv[1]);
		} else if (strncmp(argv[0], "entity", 6) == 0) {
		sdr_extended = 1;
		rc = ipmi_sdr_print_entity(intf, argv[1]);
		} else if (strncmp(argv[0], "info", 4) == 0) {
		rc = ipmi_sdr_print_info(intf);
		} else if (strncmp(argv[0], "get", 3) == 0) {
		rc = ipmi_sdr_print_entry_byid(intf, argc - 1, &argv[1]);
		} else if (strncmp(argv[0], "dump", 4) == 0) {
		if (argc < 2) {
		printf("Not enough parameters given.");
		printf("usage: sdr dump <file>");
		return (-1);
		}
		rc = ipmi_sdr_dump_bin(intf, argv[1]);
		} else if (strncmp(argv[0], "fill", 4) == 0) {
		if (argc <= 1) {
		printf("Not enough parameters given.");
		printf("usage: sdr fill sensors");
		printf("usage: sdr fill file <file>");
		printf("usage: sdr fill range <range>");
		return (-1);
		} else if (strncmp(argv[1], "sensors", 7) == 0) {
		rc = ipmi_sdr_add_from_sensors(intf, 21);
		} else if (strncmp(argv[1], "nosat", 5) == 0) {
		rc = ipmi_sdr_add_from_sensors(intf, 0);
		} else if (strncmp(argv[1], "file", 4) == 0) {
		if (argc < 3) {
		printf("Not enough parameters given.");
		printf("usage: sdr fill file <file>");
		return (-1);
		}
		rc = ipmi_sdr_add_from_file(intf, argv[2]);
		} else if (strncmp(argv[1], "range", 4) == 0) {
		if (argc < 3) {
		printf("Not enough parameters given.");
		printf("usage: sdr fill range <range>");
		return (-1);
		}
		rc = ipmi_sdr_add_from_list(intf, argv[2]);
		} else {
		printf("Invalid SDR %s command: %s", argv[0], argv[1]);
		printf("usage: sdr %s <sensors|nosat|file|range> [options]", argv[0]);
		return (-1);
		}
		} 
	   */
	else {
		printf("Invalid SDR command: %s", argv[2]);
		rc = -1;
	}

	return rc;
}

/* ipmi_sdr_print_sdr  -  iterate through SDR printing records
 *
 * intf:    ipmi interface
 * type:    record type to print
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_sdr(struct ipmi_intf *intf, uint8_t type)
{
	struct sdr_get_rs *header;
	struct sdr_record_list *e;
	int rc = 0;

	printf("Querying SDR for sensor list\n");

	if (sdr_list_itr == NULL) {
		sdr_list_itr = ipmi_sdr_start(intf, 0);
		if (sdr_list_itr == NULL) {
			printf("Unable to open SDR for reading");
			return -1;
		}
	}

	for (e = sdr_list_head; e != NULL; e = e->next) {
		if (type != e->type && type != 0xff && type != 0xfe)
			continue;
		if (type == 0xfe &&
				e->type != SDR_RECORD_TYPE_FULL_SENSOR &&
				e->type != SDR_RECORD_TYPE_COMPACT_SENSOR)
			continue;
		if (ipmi_sdr_print_listentry(intf, e) < 0)
			rc = -1;
	}

	while ((header = ipmi_sdr_get_next_header(intf, sdr_list_itr)) != NULL) {
		uint8_t *rec;
		struct sdr_record_list *sdrr;

		rec = ipmi_sdr_get_record(intf, header, sdr_list_itr);
		if (rec == NULL) {
			printf("ipmitool: ipmi_sdr_get_record() failed");
			rc = -1;
			continue;
		}

		sdrr = malloc(sizeof (struct sdr_record_list));
		if (sdrr == NULL) {
			printf("ipmitool: malloc failure");
			if (rec != NULL) {
				free(rec);
				rec = NULL;
			}
			break;
		}
		memset(sdrr, 0, sizeof (struct sdr_record_list));
		sdrr->id = header->id;
		sdrr->type = header->type;

		switch (header->type) {
			case SDR_RECORD_TYPE_FULL_SENSOR:
			case SDR_RECORD_TYPE_COMPACT_SENSOR:
				sdrr->record.common =
					(struct sdr_record_common_sensor *) rec;
				break;
			case SDR_RECORD_TYPE_EVENTONLY_SENSOR:
				sdrr->record.eventonly =
					(struct sdr_record_eventonly_sensor *) rec;
				break;
			case SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR:
				sdrr->record.genloc =
					(struct sdr_record_generic_locator *) rec;
				break;
			case SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR:
				sdrr->record.fruloc =
					(struct sdr_record_fru_locator *) rec;
				break;
			case SDR_RECORD_TYPE_MC_DEVICE_LOCATOR:
				sdrr->record.mcloc =
					(struct sdr_record_mc_locator *) rec;
				break;
			case SDR_RECORD_TYPE_ENTITY_ASSOC:
				sdrr->record.entassoc =
					(struct sdr_record_entity_assoc *) rec;
				break;
			default:
				free(rec);
				rec = NULL;
				if (sdrr != NULL) {
					free(sdrr);
					sdrr = NULL;
				}
				continue;
		}

		printf("SDR record ID   : 0x%04x\n", sdrr->id);

		if (type == header->type || type == 0xff ||
				(type == 0xfe &&
				 (header->type == SDR_RECORD_TYPE_FULL_SENSOR ||
				  header->type == SDR_RECORD_TYPE_COMPACT_SENSOR))) {
			if (ipmi_sdr_print_rawentry(intf, header->type,
						rec, header->length) < 0)
				rc = -1;
		}

		/* add to global record liset */
		if (sdr_list_head == NULL)
			sdr_list_head = sdrr;
		else
			sdr_list_tail->next = sdrr;

		sdr_list_tail = sdrr;
	}
	printf("\n");

	return rc;
}

/* ipmi_sdr_start  -  setup sdr iterator
 *
 * @intf:   ipmi interface
 *
 * returns sdr iterator structure pointer
 * returns NULL on error
 */
struct ipmi_sdr_iterator * ipmi_sdr_start(struct ipmi_intf *intf, int use_builtin)
{
	struct ipmi_sdr_iterator *itr;
	struct ipmi_rs *rsp;
	struct ipmi_rq req;

	struct ipm_devid_rsp *devid;

	itr = malloc(sizeof (struct ipmi_sdr_iterator));
	if (itr == NULL) {
		printf("ipmitool: malloc failure");
		return NULL;
	}

	/* check SDRR capability */
	memset(&req, 0, sizeof (req));
	req.msg.netfn = IPMI_NETFN_APP;
	req.msg.cmd = BMC_GET_DEVICE_ID;
	req.msg.data_len = 0;

	rsp = intf->sendrecv(intf, &req);

	if (rsp == NULL) {
		printf("Get Device ID command failed");
		free(itr);
		itr = NULL;
		return NULL;
	}
	if (rsp->ccode > 0) {
		printf("Get Device ID command failed: %#x %s",
				rsp->ccode, val2str(rsp->ccode, completion_code_vals));
		free(itr);
		itr = NULL;
		return NULL;
	}
	devid = (struct ipm_devid_rsp *) rsp->data;

	sdriana =  (long)IPM_DEV_MANUFACTURER_ID(devid->manufacturer_id);

	if (!use_builtin && (devid->device_revision & IPM_DEV_DEVICE_ID_SDR_MASK)) {
		if ((devid->adtl_device_support & 0x02) == 0) {
			if ((devid->adtl_device_support & 0x01)) {
				printf("Using Device SDRs\n");
				use_built_in = 1;
			} else {
				printf("Error obtaining SDR info");
				free(itr);
				itr = NULL;
				return NULL;
			}
		} else {
			printf("Using SDR from Repository \n");
		}
	}
	itr->use_built_in = use_builtin ? 1 : use_built_in;
	/***********************/
	if (itr->use_built_in == 0) {
		struct sdr_repo_info_rs sdr_info;
		/* get sdr repository info */
		memset(&req, 0, sizeof (req));
		req.msg.netfn = IPMI_NETFN_STORAGE;
		req.msg.cmd = GET_SDR_REPO_INFO;

		rsp = intf->sendrecv(intf, &req);
		if (rsp == NULL) {
			printf("Error obtaining SDR info  ");
			free(itr);
			itr = NULL;
			return NULL;
		}
		if (rsp->ccode > 0) {
			printf("Error obtaining SDR info: %s  ",
					val2str(rsp->ccode, completion_code_vals));
			free(itr);
			itr = NULL;
			return NULL;
		}

		memcpy(&sdr_info, rsp->data, sizeof (sdr_info));
		/* IPMIv1.0 == 0x01
		 * IPMIv1.5 == 0x51
		 * IPMIv2.0 == 0x02
		 */
		if ((sdr_info.version != 0x51) &&
				(sdr_info.version != 0x01) &&
				(sdr_info.version != 0x02)) {
			printf("WARNING: Unknown SDR repository\n"
					"version 0x%02x\n", sdr_info.version);
		}

		itr->total = sdr_info.count;
		itr->next = 0;

		printf("SDR free space: %d\n", sdr_info.free);
		printf("SDR records   : %d\n", sdr_info.count);

		/* Build SDRR if there is no record in repository */
		if( sdr_info.count == 0 ) {
			printf("Rebuilding SDRR...");

			if( ipmi_sdr_add_from_sensors( intf, 0 ) != 0 ) {
				printf("Could not build SDRR!");
				free(itr);
				itr = NULL;
				return NULL;
			}
		}
			printf("\n");
	} else {
		struct sdr_device_info_rs sdr_info;
		/* get device sdr info */
		memset(&req, 0, sizeof (req));
		req.msg.netfn = IPMI_NETFN_SE;
		req.msg.cmd = GET_DEVICE_SDR_INFO;

		rsp = intf->sendrecv(intf, &req);
		if (!rsp || !rsp->data_len || rsp->ccode) {
			printf("Err in cmd get sensor sdr info\n");
			free(itr);
			itr = NULL;
			return NULL;
		}
		memcpy(&sdr_info, rsp->data, sizeof (sdr_info));

		itr->total = sdr_info.count;
		itr->next = 0;
		printf("SDR records   : %d\n", sdr_info.count);
	}

	if (ipmi_sdr_get_reservation(intf, itr->use_built_in,
				&(itr->reservation)) < 0) {
		printf("Unable to obtain SDR reservation");
		free(itr);
		itr = NULL;
		return NULL;
	}

	return itr;
}

/* ipmi_sdr_get_reservation  -  Obtain SDR reservation ID
 *
 * @intf:   ipmi interface
 * @reserve_id: pointer to short int for storing the id
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_get_reservation(struct ipmi_intf *intf, int use_builtin, uint16_t * reserve_id)
{
	struct ipmi_rs *rsp;
	struct ipmi_rq req;

	/* obtain reservation ID */
	memset(&req, 0, sizeof (req));

	if (use_builtin == 0) {
		req.msg.netfn = IPMI_NETFN_STORAGE;
	} else {
		req.msg.netfn = IPMI_NETFN_SE;
	}

	req.msg.cmd = GET_SDR_RESERVE_REPO;
	rsp = intf->sendrecv(intf, &req);

	/* be slient for errors, they are handled by calling function */
	if (rsp == NULL)
		return -1;
	if (rsp->ccode > 0)
		return -1;

	*reserve_id = ((struct sdr_reserve_repo_rs *) &(rsp->data))->reserve_id;
	printf("SDR reservation ID %04x \n", *reserve_id);

	return 0;
}


/* ipmi_sdr_get_next_header  -  retreive next SDR header
 *
 * @intf:   ipmi interface
 * @itr:    sdr iterator
 *
 * returns pointer to sensor retrieval struct
 * returns NULL on error
 */
struct sdr_get_rs * ipmi_sdr_get_next_header(struct ipmi_intf *intf, struct ipmi_sdr_iterator *itr)
{
	struct sdr_get_rs *header;

	if (itr->next == 0xffff)
		return NULL;

	header = ipmi_sdr_get_header(intf, itr);
	if (header == NULL)
		return NULL;

	itr->next = header->next;

	return header;
}

/* ipmi_sdr_get_header  -  retreive SDR record header
 *
 * @intf:   ipmi interface
 * @itr:    sdr iterator
 *
 * returns pointer to static sensor retrieval struct
 * returns NULL on error
 */
static struct sdr_get_rs * ipmi_sdr_get_header(struct ipmi_intf *intf, struct ipmi_sdr_iterator *itr)
{
	struct ipmi_rq req;
	struct ipmi_rs *rsp;
	struct sdr_get_rq sdr_rq;
	static struct sdr_get_rs sdr_rs;
	int try = 0;

	memset(&sdr_rq, 0, sizeof (sdr_rq));
	sdr_rq.reserve_id = itr->reservation;
	sdr_rq.id = itr->next;
	sdr_rq.offset = 0;
	sdr_rq.length = 5;  /* only get the header */

	memset(&req, 0, sizeof (req));
	if (itr->use_built_in == 0) {
		req.msg.netfn = IPMI_NETFN_STORAGE;
		req.msg.cmd = GET_SDR;
	} else {
		req.msg.netfn = IPMI_NETFN_SE;
		req.msg.cmd = GET_DEVICE_SDR;
	}
	req.msg.data = (uint8_t *) & sdr_rq;
	req.msg.data_len = sizeof (sdr_rq);

	for (try = 0; try < 5; try++) {
		sdr_rq.reserve_id = itr->reservation;
		rsp = intf->sendrecv(intf, &req);
		if (rsp == NULL) {
			printf("Get SDR %04x command failed",
					itr->next);
			continue;
		} else if (rsp->ccode == 0xc5) {
			/* lost reservation */
			printf("SDR reservation %04x cancelled. "
					"Sleeping a bit and retrying...",
					itr->reservation);

			sleep(rand() & 3);

			if (ipmi_sdr_get_reservation(intf, itr->use_built_in,
						&(itr->reservation)) < 0) {
				printf("Unable to renew SDR reservation");
				return NULL;
			}
		} else if (rsp->ccode > 0) {
			printf("Get SDR %04x command failed: %s",
					itr->next, val2str(rsp->ccode,
						completion_code_vals));
			continue;
		} else {
			break;
		}
	}

	if (try == 5)
		return NULL;

	if (!rsp)
		return NULL;

	printf("SDR record ID   : 0x%04x\n", itr->next);

	memcpy(&sdr_rs, rsp->data, sizeof (sdr_rs));

	if (sdr_rs.length == 0) {
		printf("SDR record id 0x%04x: invalid length %d\n",           itr->next, sdr_rs.length);
		return NULL;
	}

	/* achu (chu11 at llnl dot gov): - Some boards are stupid and
	 * return a record id from the Get SDR Record command
	 * different than the record id passed in.  If we find this
	 * situation, we cheat and put the original record id back in.
	 * Otherwise, a later Get SDR Record command will fail with
	 * completion code CBh = "Requested Sensor, data, or record
	 * not present"
	 */
	if (sdr_rs.id != itr->next) {
		printf("SDR record id mismatch: 0x%04x\n", sdr_rs.id);
		sdr_rs.id = itr->next;
	}

	printf("SDR record type : 0x%02x\n", sdr_rs.type);
	printf("SDR record next : 0x%04x\n", sdr_rs.next);
	printf("SDR record bytes: %d\n", sdr_rs.length);

	return &sdr_rs;
}

/* ipmi_sdr_get_record  -  return RAW SDR record
 *
 * @intf:   ipmi interface
 * @header: SDR header
 * @itr:    SDR iterator
 *
 * returns raw SDR data
 * returns NULL on error
 */
uint8_t * ipmi_sdr_get_record(struct ipmi_intf * intf, struct sdr_get_rs * header, struct ipmi_sdr_iterator * itr)
{
	struct ipmi_rq req;
	struct ipmi_rs *rsp;
	struct sdr_get_rq sdr_rq;
	uint8_t *data;
	int i = 0, len = header->length;

	if (len < 1)
		return NULL;

	data = malloc(len + 1);
	if (data == NULL) {
		printf("ipmitool: malloc failure");
		return NULL;
	}
	memset(data, 0, len + 1);

	memset(&sdr_rq, 0, sizeof (sdr_rq));
	sdr_rq.reserve_id = itr->reservation;
	sdr_rq.id = header->id;
	sdr_rq.offset = 0;

	memset(&req, 0, sizeof (req));
	if (itr->use_built_in == 0) {
		req.msg.netfn = IPMI_NETFN_STORAGE;
		req.msg.cmd = GET_SDR;
	} else {
		req.msg.netfn = IPMI_NETFN_SE;
		req.msg.cmd = GET_DEVICE_SDR;
	}
	req.msg.data = (uint8_t *) & sdr_rq;
	req.msg.data_len = sizeof (sdr_rq);

	/* check if max length is null */
	if ( sdr_max_read_len == 0 ) {
		/* get maximum response size */
		sdr_max_read_len = ipmi_intf_get_max_response_data_size(intf) - 2;

		/* cap the number of bytes to read */
		if (sdr_max_read_len > 0xFE) {
			sdr_max_read_len = 0xFE;
		}
	}

	/* read SDR record with partial reads
	 * because a full read usually exceeds the maximum
	 * transport buffer size.  (completion code 0xca)
	 */
	while (i < len) {
		sdr_rq.length = (len - i < sdr_max_read_len) ?
			len - i : sdr_max_read_len;
		sdr_rq.offset = i + 5;  /* 5 header bytes */

		printf("Getting %d bytes from SDR at offset %d",sdr_rq.length, sdr_rq.offset);

		rsp = intf->sendrecv(intf, &req);
		if (rsp == NULL) {
			sdr_max_read_len = sdr_rq.length - 1;
			if (sdr_max_read_len > 0) {
				/* no response may happen if requests are bridged
				   and too many bytes are requested */
				continue;
			} else {
				free(data);
				data = NULL;
				return NULL;
			}
		}

		switch (rsp->ccode) {
			case 0xca:
				/* read too many bytes at once */
				sdr_max_read_len = sdr_rq.length - 1;
				continue;
			case 0xc5:
				/* lost reservation */
				printf("SDR reservation cancelled. "
						"Sleeping a bit and retrying...");

				sleep(rand() & 3);

				if (ipmi_sdr_get_reservation(intf, itr->use_built_in,
							&(itr->reservation)) < 0) {
					free(data);
					data = NULL;
					return NULL;
				}
				sdr_rq.reserve_id = itr->reservation;
				continue;
		}

		/* special completion codes handled above */
		if (rsp->ccode > 0 || rsp->data_len == 0) {
			free(data);
			data = NULL;
			return NULL;
		}

		memcpy(data + i, rsp->data + 2, sdr_rq.length);
		i += sdr_max_read_len;
	}

	return data;
}
/* ipmi_sdr_print_listentry  -  Print SDR entry from list
 *
 * @intf:   ipmi interface
 * @entry:  sdr record list entry
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_listentry(struct ipmi_intf *intf, struct sdr_record_list *entry)
{
	int rc = 0; 

	switch (entry->type) {
		case SDR_RECORD_TYPE_FULL_SENSOR:
		case SDR_RECORD_TYPE_COMPACT_SENSOR:
			//rc = ipmi_sdr_print_sensor_fc(intf, entry->record.common, entry->type);
			break;
		case SDR_RECORD_TYPE_EVENTONLY_SENSOR:
			rc = ipmi_sdr_print_sensor_eventonly(intf, entry->record.eventonly);
			break;
		case SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR:
			rc = ipmi_sdr_print_sensor_generic_locator(intf, entry->record.genloc);
			break;
		case SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR:
			rc = ipmi_sdr_print_sensor_fru_locator(intf, entry->record.fruloc);
			break;
		case SDR_RECORD_TYPE_MC_DEVICE_LOCATOR:
			rc = ipmi_sdr_print_sensor_mc_locator(intf, entry->record.mcloc);
			break;
		case SDR_RECORD_TYPE_ENTITY_ASSOC:
			rc = ipmi_sdr_print_sensor_entity_assoc(intf, entry->record.entassoc);
			break;
		case SDR_RECORD_TYPE_OEM:
			rc = ipmi_sdr_print_sensor_oem(intf, entry->record.oem);
			break;
		case SDR_RECORD_TYPE_DEVICE_ENTITY_ASSOC:
		case SDR_RECORD_TYPE_MC_CONFIRMATION:
		case SDR_RECORD_TYPE_BMC_MSG_CHANNEL_INFO:
			/* not implemented yet */
			break;
	}    

	return rc;
}

/* ipmi_sdr_print_rawentry  -  Print SDR entry from raw data
 *
 * @intf:   ipmi interface
 * @type:   sensor type
 * @raw:    raw sensor data
 * @len:    length of raw sensor data
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_rawentry(struct ipmi_intf *intf, uint8_t type, uint8_t * raw, int len)
{
	int rc = 0;

	switch (type) {
		case SDR_RECORD_TYPE_FULL_SENSOR:
		case SDR_RECORD_TYPE_COMPACT_SENSOR:
			//			rc = ipmi_sdr_print_sensor_fc(intf,	(struct sdr_record_common_sensor *) raw, type);
			break;
		case SDR_RECORD_TYPE_EVENTONLY_SENSOR:
			rc = ipmi_sdr_print_sensor_eventonly(intf,(struct sdr_record_eventonly_sensor*) raw);
			break;
		case SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR:
			rc = ipmi_sdr_print_sensor_generic_locator(intf,(struct sdr_record_generic_locator*) raw);
			break;
		case SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR:
			rc = ipmi_sdr_print_sensor_fru_locator(intf, (struct sdr_record_fru_locator *) raw);
			break;
		case SDR_RECORD_TYPE_MC_DEVICE_LOCATOR:
			rc = ipmi_sdr_print_sensor_mc_locator(intf, (struct sdr_record_mc_locator *) raw);
			break;
		case SDR_RECORD_TYPE_ENTITY_ASSOC:
			rc = ipmi_sdr_print_sensor_entity_assoc(intf, (struct sdr_record_entity_assoc*) raw);
			break;
		case SDR_RECORD_TYPE_OEM:{
									 struct sdr_record_oem oem;
									 oem.data = raw;
									 oem.data_len = len;
									 rc = ipmi_sdr_print_sensor_oem(intf, (struct sdr_record_oem *) &oem);
									 break;
								 }
		case SDR_RECORD_TYPE_DEVICE_ENTITY_ASSOC:
		case SDR_RECORD_TYPE_MC_CONFIRMATION:
		case SDR_RECORD_TYPE_BMC_MSG_CHANNEL_INFO:
								 /* not implemented */
								 break;
	}

	return rc;
}

/* ipmi_sdr_get_sensor_type_desc  -  Get sensor type descriptor
 *
 * @type:   ipmi sensor type
 *
 * returns
 *   string from sensor_type_desc
 *   or "reserved"
 *   or "OEM reserved"
 */
	const char *
ipmi_sdr_get_sensor_type_desc(const uint8_t type)
{
	static char desc[32];
	memset(desc, 0, 32);
	if (type <= SENSOR_TYPE_MAX)
		return sensor_type_desc[type];
	if (type < 0xc0)
		snprintf(desc, 32, "reserved #%02x", type);
	else
	{
		snprintf(desc, 32, oemval2str(sdriana,type,ipmi_oem_sdr_type_vals),
				type);
	}
	return desc;
}


/* ipmi_sdr_print_sensor_eventonly  -  print SDR event only record
 *
 * @intf:   ipmi interface
 * @sensor: event only sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_sensor_eventonly(struct ipmi_intf *intf, struct sdr_record_eventonly_sensor *sensor)
{
	char desc[17];

	if (sensor == NULL)
		return -1;

	memset(desc, 0, sizeof (desc));
	snprintf(desc, (sensor->id_code & 0x1f) + 1, "%s", sensor->id_string);

	if (verbose) {
		printf("Sensor ID              : %s (0x%x)\n",
				sensor->id_code ? desc : "", sensor->keys.sensor_num);
		printf("Entity ID              : %d.%d (%s)\n",
				sensor->entity.id, sensor->entity.instance,
				val2str(sensor->entity.id, entity_id_vals));
		printf("Sensor Type            : %s (0x%02x)\n",
				ipmi_sdr_get_sensor_type_desc(sensor->sensor_type),
				sensor->sensor_type);
		printf("Event Type Code        : 0x%02x", sensor->event_type);
		printf("\n");
	} else {
		if (csv_output)
			printf("%s,%02Xh,ns,%d.%d,Event-Only\n",
					sensor->id_code ? desc : "",
					sensor->keys.sensor_num,
					sensor->entity.id, sensor->entity.instance);
		else if (sdr_extended)
			printf("%-16s | %02Xh | ns  | %2d.%1d | Event-Only\n",
					sensor->id_code ? desc : "",
					sensor->keys.sensor_num,
					sensor->entity.id, sensor->entity.instance);
		else
			printf("%-16s | Event-Only        | ns\n",
					sensor->id_code ? desc : "");
	}

	return 0;
}

/* ipmi_sdr_print_sensor_generic_locator  -  print generic device locator record
 *
 * @intf:   ipmi interface
 * @gen:    generic device locator sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_sensor_generic_locator(struct ipmi_intf *intf, struct sdr_record_generic_locator *dev)
{
	char desc[17];

	memset(desc, 0, sizeof (desc));
	snprintf(desc, (dev->id_code & 0x1f) + 1, "%s", dev->id_string);

	if (!verbose) {
		if (csv_output)
			printf("%s,00h,ns,%d.%d\n",
					dev->id_code ? desc : "",
					dev->entity.id, dev->entity.instance);
		else if (sdr_extended)
			printf
				("%-16s | 00h | ns  | %2d.%1d | Generic Device @%02Xh:%02Xh.%1d\n",
				 dev->id_code ? desc : "", dev->entity.id,
				 dev->entity.instance, dev->dev_access_addr,
				 dev->dev_slave_addr, dev->oem);
		else
			printf("%-16s | Generic @%02X:%02X.%-2d | ok\n",
					dev->id_code ? desc : "",
					dev->dev_access_addr,
					dev->dev_slave_addr, dev->oem);

		return 0;
	}

	printf("Device ID              : %s\n", dev->id_string);
	printf("Entity ID              : %d.%d (%s)\n",
			dev->entity.id, dev->entity.instance,
			val2str(dev->entity.id, entity_id_vals));

	printf("Device Access Address  : %02Xh\n", dev->dev_access_addr);
	printf("Device Slave Address   : %02Xh\n", dev->dev_slave_addr);
	printf("Address Span           : %02Xh\n", dev->addr_span);
	printf("Channel Number         : %01Xh\n", dev->channel_num);
	printf("LUN.Bus                : %01Xh.%01Xh\n", dev->lun, dev->bus);
	printf("Device Type.Modifier   : %01Xh.%01Xh (%s)\n",
			dev->dev_type, dev->dev_type_modifier,
			val2str(dev->dev_type << 8 | dev->dev_type_modifier,
				entity_device_type_vals));
	printf("OEM                    : %02Xh\n", dev->oem);
	printf("\n");

	return 0;
}

/* ipmi_sdr_print_sensor_fru_locator  -  print FRU locator record
 *
 * @intf:   ipmi interface
 * @fru:    fru locator sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_sensor_fru_locator(struct ipmi_intf *intf, struct sdr_record_fru_locator *fru)
{
	char desc[17];

	memset(desc, 0, sizeof (desc));
	snprintf(desc, (fru->id_code & 0x1f) + 1, "%s", fru->id_string);

	if (!verbose) {
		if (csv_output)
			printf("%s,00h,ns,%d.%d\n",
					fru->id_code ? desc : "",
					fru->entity.id, fru->entity.instance);
		else if (sdr_extended)
			printf("%-16s | 00h | ns  | %2d.%1d | %s FRU @%02Xh\n",
					fru->id_code ? desc : "",
					fru->entity.id, fru->entity.instance,
					(fru->logical) ? "Logical" : "Physical",
					fru->device_id);
		else
			printf("%-16s | %s FRU @%02Xh %02x.%x | ok\n",
					fru->id_code ? desc : "",
					(fru->logical) ? "Log" : "Phy",
					fru->device_id,
					fru->entity.id, fru->entity.instance);

		return 0;
	}

	printf("Device ID              : %s\n", fru->id_string);
	printf("Entity ID              : %d.%d (%s)\n",
			fru->entity.id, fru->entity.instance,
			val2str(fru->entity.id, entity_id_vals));

	printf("Device Access Address  : %02Xh\n", fru->dev_slave_addr);
	printf("%s: %02Xh\n",
			fru->logical ? "Logical FRU Device     " :
			"Slave Address          ", fru->device_id);
	printf("Channel Number         : %01Xh\n", fru->channel_num);
	printf("LUN.Bus                : %01Xh.%01Xh\n", fru->lun, fru->bus);
	printf("Device Type.Modifier   : %01Xh.%01Xh (%s)\n",
			fru->dev_type, fru->dev_type_modifier,
			val2str(fru->dev_type << 8 | fru->dev_type_modifier,
				entity_device_type_vals));
	printf("OEM                    : %02Xh\n", fru->oem);
	printf("\n");

	return 0;
}
/* ipmi_sdr_print_sensor_mc_locator  -  print SDR MC locator record
 *
 * @intf:   ipmi interface
 * @mc:     mc locator sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_sensor_mc_locator(struct ipmi_intf *intf, struct sdr_record_mc_locator *mc)
{
	char desc[17];

	if (mc == NULL)
		return -1;

	memset(desc, 0, sizeof (desc));
	snprintf(desc, (mc->id_code & 0x1f) + 1, "%s", mc->id_string);

	if (verbose == 0) {
		if (csv_output)
			printf("%s,00h,ok,%d.%d\n",
					mc->id_code ? desc : "",
					mc->entity.id, mc->entity.instance);
		else if (sdr_extended) {
			printf("%-16s | 00h | ok  | %2d.%1d | ",
					mc->id_code ? desc : "",
					mc->entity.id, mc->entity.instance);

			printf("%s MC @ %02Xh\n",
					(mc->
					 pwr_state_notif & 0x1) ? "Static" : "Dynamic",
					mc->dev_slave_addr);
		} else {
			printf("%-16s | %s MC @ %02Xh %s | ok\n",
					mc->id_code ? desc : "",
					(mc->
					 pwr_state_notif & 0x1) ? "Static" : "Dynamic",
					mc->dev_slave_addr,
					(mc->pwr_state_notif & 0x1) ? " " : "");
		}

		return 0;   /* done */
	}

	printf("Device ID              : %s\n", mc->id_string);
	printf("Entity ID              : %d.%d (%s)\n",
			mc->entity.id, mc->entity.instance,
			val2str(mc->entity.id, entity_id_vals));

	printf("Device Slave Address   : %02Xh\n", mc->dev_slave_addr);
	printf("Channel Number         : %01Xh\n", mc->channel_num);

	printf("ACPI System P/S Notif  : %sRequired\n",
			(mc->pwr_state_notif & 0x4) ? "" : "Not ");
	printf("ACPI Device P/S Notif  : %sRequired\n",
			(mc->pwr_state_notif & 0x2) ? "" : "Not ");
	printf("Controller Presence    : %s\n",
			(mc->pwr_state_notif & 0x1) ? "Static" : "Dynamic");
	printf("Logs Init Agent Errors : %s\n",
			(mc->global_init & 0x8) ? "Yes" : "No");

	printf("Event Message Gen      : ");
	if (!(mc->global_init & 0x3))
		printf("Enable\n");
	else if ((mc->global_init & 0x3) == 0x1)
		printf("Disable\n");
	else if ((mc->global_init & 0x3) == 0x2)
		printf("Do Not Init Controller\n");
	else
		printf("Reserved\n");

	printf("Device Capabilities\n");
	printf(" Chassis Device        : %s\n",
			(mc->dev_support & 0x80) ? "Yes" : "No");
	printf(" Bridge                : %s\n",
			(mc->dev_support & 0x40) ? "Yes" : "No");
	printf(" IPMB Event Generator  : %s\n",
			(mc->dev_support & 0x20) ? "Yes" : "No");
	printf(" IPMB Event Receiver   : %s\n",
			(mc->dev_support & 0x10) ? "Yes" : "No");
	printf(" FRU Inventory Device  : %s\n",
			(mc->dev_support & 0x08) ? "Yes" : "No");
	printf(" SEL Device            : %s\n",
			(mc->dev_support & 0x04) ? "Yes" : "No");
	printf(" SDR Repository        : %s\n",
			(mc->dev_support & 0x02) ? "Yes" : "No");
	printf(" Sensor Device         : %s\n",
			(mc->dev_support & 0x01) ? "Yes" : "No");

	printf("\n");

	return 0;
}


/* ipmi_sdr_print_sensor_entity_assoc  -  print SDR entity association record
 *
 * @intf:   ipmi interface
 * @mc:     entity association sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_sensor_entity_assoc(struct ipmi_intf *intf, struct sdr_record_entity_assoc *assoc)
{
	return 0;
}

/* ipmi_sdr_print_sensor_oem  -  print OEM sensors
 *
 * This function is generally only filled out by decoding what
 * a particular BMC might stuff into its OEM records.  The
 * records are keyed off manufacturer ID and record subtypes.
 *
 * @intf:   ipmi interface
 * @oem:    oem sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
static int ipmi_sdr_print_sensor_oem(struct ipmi_intf *intf, struct sdr_record_oem *oem)
{
	int rc = 0;

	if (oem == NULL)
		return -1;
	if (oem->data_len == 0 || oem->data == NULL)
		return -1;

	if (verbose > 2)
		printbuf(oem->data, oem->data_len, "OEM Record");

	/* intel manufacturer id */
	if (oem->data[0] == 0x57 &&
			oem->data[1] == 0x01 && oem->data[2] == 0x00) {
		rc = ipmi_sdr_print_sensor_oem_intel(intf, oem);
	}

	return rc;
}
/* ipmi_sdr_print_sensor_oem_intel  -  print Intel OEM sensors
 *
 * @intf:   ipmi interface
 * @oem:    oem sdr record
 *
 * returns 0 on success
 * returns -1 on error
 */
static int ipmi_sdr_print_sensor_oem_intel(struct ipmi_intf *intf, struct sdr_record_oem *oem)
{
	switch (oem->data[3]) { /* record sub-type */
		case 0x02:      /* Power Unit Map */
			if (verbose) {
				printf
					("Sensor ID              : Power Unit Redundancy (0x%x)\n",
					 oem->data[4]);
				printf
					("Sensor Type            : Intel OEM - Power Unit Map\n");
				printf("Redundant Supplies     : %d", oem->data[6]);
				if (oem->data[5])
					printf(" (flags %xh)", oem->data[5]);
				printf("\n");
			}

			switch (oem->data_len) {
				case 7: /* SR1300, non-redundant */
					if (verbose)
						printf("Power Redundancy       : No\n");
					else if (csv_output)
						printf("Power Redundancy,Not Available,nr\n");
					else
						printf
							("Power Redundancy | Not Available     | nr\n");
					break;
				case 8: /* SR2300, redundant, PS1 & PS2 present */
					if (verbose) {
						printf("Power Redundancy       : No\n");
						printf("Power Supply 2 Sensor  : %x\n",
								oem->data[8]);
					} else if (csv_output) {
						printf("Power Redundancy,PS@%02xh,nr\n",
								oem->data[8]);
					} else {
						printf
							("Power Redundancy | PS@%02xh            | nr\n",
							 oem->data[8]);
					}
					break;
				case 9: /* SR2300, non-redundant, PSx present */
					if (verbose) {
						printf("Power Redundancy       : Yes\n");
						printf("Power Supply Sensor    : %x\n",
								oem->data[7]);
						printf("Power Supply Sensor    : %x\n",
								oem->data[8]);
					} else if (csv_output) {
						printf
							("Power Redundancy,PS@%02xh + PS@%02xh,ok\n",
							 oem->data[7], oem->data[8]);
					} else {
						printf
							("Power Redundancy | PS@%02xh + PS@%02xh   | ok\n",
							 oem->data[7], oem->data[8]);
					}
					break;
			}
			if (verbose)
				printf("\n");
			break;
		case 0x03:      /* Fan Speed Control */
			break;
		case 0x06:      /* System Information */
			break;
		case 0x07:      /* Ambient Temperature Fan Speed Control */
			break;
		default:
			printf("Unknown Intel OEM SDR Record type %02x", oem->data[3]);
	}

	return 0;
}

/* ipmi_sdr_print_type  -  print all sensors of specified type
 *
 * @intf:   ipmi interface
 * @type:   sensor type
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_type(struct ipmi_intf *intf, char *type)
{
	struct sdr_record_list *list, *entry;
	int rc = 0;
	int x;
	uint8_t sensor_type = 0;

	if (type == NULL ||
			strncasecmp(type, "help", 4) == 0 ||
			strncasecmp(type, "list", 4) == 0) {
		printf("Sensor Types:\n");
		for (x = 1; x < SENSOR_TYPE_MAX; x += 2) {
			printf("\t%-25s (0x%02x)   %-25s (0x%02x)\n",
					sensor_type_desc[x], x,
					sensor_type_desc[x + 1], x + 1);
		}
		return 0;
	}

	if (strncmp(type, "0x", 2) == 0) {
		/* begins with 0x so let it be entered as raw hex value */
		if (str2uchar(type, &sensor_type) != 0) {
			printf("Given type of sensor \"%s\" is either invalid or out of range.", type);
			return (-1);
		}
	} else {
		for (x = 1; x < SENSOR_TYPE_MAX; x++) {
			if (strncasecmp(sensor_type_desc[x], type,
						__maxlen(type,
							sensor_type_desc[x])) == 0) {
				sensor_type = x;
				break;
			}
		}
		if (sensor_type != x) {
			printf("Sensor Type \"%s\" not found.",	type);
			printf("Sensor Types:\n");
			for (x = 1; x < SENSOR_TYPE_MAX; x += 2) {
				printf("\t%-25s (0x%02x)   %-25s (0x%02x)\n",
						sensor_type_desc[x], x,
						sensor_type_desc[x + 1], x + 1);
			}
			return 0;
		}
	}

	//	list = ipmi_sdr_find_sdr_bysensortype(intf, sensor_type);

	for (entry = list; entry != NULL; entry = entry->next) {
		rc = ipmi_sdr_print_listentry(intf, entry);
	}

	//	__sdr_list_empty(list);

	return rc;
}


/* ipmi_sdr_print_entity  -  print entity's for an id/instance
 *
 * @intf:   ipmi interface
 * @entitystr:  entity id/instance string, i.e. "1.1"
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_sdr_print_entity(struct ipmi_intf *intf, char *entitystr)
{
	struct sdr_record_list *list, *entry;
	struct entity_id entity;
	unsigned id = 0;
	unsigned instance = 0;
	int rc = 0;

	if (entitystr == NULL ||
			strncasecmp(entitystr, "help", 4) == 0 ||
			strncasecmp(entitystr, "list", 4) == 0) {
		print_valstr_2col(entity_id_vals, "Entity IDs", -1);
		return 0;
	}

	if (sscanf(entitystr, "%u.%u", &id, &instance) != 2) {
		/* perhaps no instance was passed
		 * in which case we want all instances for this entity
		 * so set entity.instance = 0x7f to indicate this
		 */
		if (sscanf(entitystr, "%u", &id) != 1) {
			int i, j=0;

			/* now try string input */
			for (i = 0; entity_id_vals[i].str != NULL; i++) {
				if (strncasecmp(entitystr, entity_id_vals[i].str,
							__maxlen(entitystr, entity_id_vals[i].str)) == 0) {
					entity.id = entity_id_vals[i].val;
					entity.instance = 0x7f;
					j=1;
				}
			}
			if (j == 0) {
				printf("Invalid entity: %s", entitystr);
				return -1;
			}
		} else {
			entity.id = id;
			entity.instance = 0x7f;
		}
	} else {
		entity.id = id;
		entity.instance = instance;
	}

	//	list = ipmi_sdr_find_sdr_byentity(intf, &entity);

	for (entry = list; entry != NULL; entry = entry->next) {
		rc = ipmi_sdr_print_listentry(intf, entry);
	}

	//	__sdr_list_empty(list);

	return rc;
}

/*
 * ipmi_sdr_print_info
 *
 * Display the return data of the GET SDR REPOSITORY INFO command
 * See section 33.9 of the IPMI v2 specification for details
 *
 * returns 0 on success
 *         -1 on error
 */
int ipmi_sdr_print_info(struct ipmi_intf *intf)
{
	uint32_t timestamp;
	uint16_t free_space;

	struct get_sdr_repository_info_rsp sdr_repository_info;
	/*
	   if (ipmi_sdr_get_info(intf, &sdr_repository_info) != 0)
	   return -1;
	 */
	printf("SDR Version                         : 0x%x\n",
			sdr_repository_info.sdr_version);
	printf("Record Count                        : %d\n",
			(sdr_repository_info.record_count_msb << 8) |
			sdr_repository_info.record_count_lsb);

	free_space =
		(sdr_repository_info.free_space[1] << 8) |
		sdr_repository_info.free_space[0];

	printf("Free Space                          : ");
	switch (free_space) {
		case 0x0000:
			printf("none (full)\n");
			break;
		case 0xFFFF:
			printf("unspecified\n");
			break;
		case 0xFFFE:
			printf("> 64Kb - 2 bytes\n");
			break;
		default:
			printf("%d bytes\n", free_space);
			break;
	}

	timestamp =
		(sdr_repository_info.most_recent_addition_timestamp[3] << 24) |
		(sdr_repository_info.most_recent_addition_timestamp[2] << 16) |
		(sdr_repository_info.most_recent_addition_timestamp[1] << 8) |
		sdr_repository_info.most_recent_addition_timestamp[0];
	//	printf("Most recent Addition                : %s\n", ipmi_sdr_timestamp(timestamp));

	timestamp =
		(sdr_repository_info.most_recent_erase_timestamp[3] << 24) |
		(sdr_repository_info.most_recent_erase_timestamp[2] << 16) |
		(sdr_repository_info.most_recent_erase_timestamp[1] << 8) |
		sdr_repository_info.most_recent_erase_timestamp[0];
	//	printf("Most recent Erase                   : %s\n", ipmi_sdr_timestamp(timestamp));

	printf("SDR overflow                        : %s\n",
			(sdr_repository_info.overflow_flag ? "yes" : "no"));

	printf("SDR Repository Update Support       : ");
	switch (sdr_repository_info.modal_update_support) {
		case 0:
			printf("unspecified\n");
			break;
		case 1:
			printf("non-modal\n");
			break;
		case 2:
			printf("modal\n");
			break;
		case 3:
			printf("modal and non-modal\n");
			break;
		default:
			printf("error in response\n");
			break;
	}

	printf("Delete SDR supported                : %s\n",
			sdr_repository_info.delete_sdr_supported ? "yes" : "no");
	printf("Partial Add SDR supported           : %s\n",
			sdr_repository_info.partial_add_sdr_supported ? "yes" : "no");
	printf("Reserve SDR repository supported    : %s\n",
			sdr_repository_info.
			reserve_sdr_repository_supported ? "yes" : "no");
	printf("SDR Repository Alloc info supported : %s\n",
			sdr_repository_info.
			get_sdr_repository_allo_info_supported ? "yes" : "no");

	return 0;
}


/* ipmi_sdr_print_entry_byid  -  print sdr entries identified by sensor id
 *
 * @intf:   ipmi interface
 * @argc:   number of entries to print
 * @argv:   list of sensor ids
 *
 * returns 0 on success
 * returns -1 on error
 */
static int ipmi_sdr_print_entry_byid(struct ipmi_intf *intf, int argc, char **argv)
{
	struct sdr_record_list *sdr;
	int rc = 0;
	int v, i;

	if (argc < 1) {
		printf("No Sensor ID supplied");
		return -1;
	}

	v = verbose;
	verbose = 1;

	for (i = 0; i < argc; i++) {
		//		sdr = ipmi_sdr_find_sdr_byid(intf, argv[i]);
		if (sdr == NULL) {
			printf("Unable to find sensor id '%s'", argv[i]);
		} else {
			if (ipmi_sdr_print_listentry(intf, sdr) < 0)
				rc = -1;
		}
	}

	verbose = v;

	return rc;
}

/* ipmi_sdr_dump_bin  -  Write raw SDR to binary file
 *
 * used for post-processing by other utilities
 *
 * @intf:   ipmi interface
 * @ofile:  output filename
 *
 * returns 0 on success
 * returns -1 on error
 */
static int ipmi_sdr_dump_bin(struct ipmi_intf *intf, const char *ofile)
{
	struct sdr_get_rs *header;
	struct ipmi_sdr_iterator *itr;
	struct sdr_record_list *sdrr;
	FILE *fp;
	int rc = 0;

	/* open connection to SDR */
	itr = ipmi_sdr_start(intf, 0);
	if (itr == NULL) {
		printf("Unable to open SDR for reading");
		return -1;
	}

	printf("Dumping Sensor Data Repository to '%s'\n", ofile);

	/* generate list of records */
	while ((header = ipmi_sdr_get_next_header(intf, itr)) != NULL) {
		sdrr = malloc(sizeof(struct sdr_record_list));
		if (sdrr == NULL) {
			printf("ipmitool: malloc failure");
			return -1;
		}
		memset(sdrr, 0, sizeof(struct sdr_record_list));

		printf("Record ID %04x (%d bytes)", header->id, header->length);

		sdrr->id = header->id;
		sdrr->version = header->version;
		sdrr->type = header->type;
		sdrr->length = header->length;
		sdrr->raw = ipmi_sdr_get_record(intf, header, itr);

		if (sdrr->raw == NULL) {
			printf("ipmitool: cannot obtain SDR record %04x", header->id);
			if (sdrr != NULL) {
				free(sdrr);
				sdrr = NULL;
			}
			return -1;
		}

		if (sdr_list_head == NULL)
			sdr_list_head = sdrr;
		else
			sdr_list_tail->next = sdrr;

		sdr_list_tail = sdrr;
	}

	//	ipmi_sdr_end(intf, itr);

	/* now write to file */
	//	fp = ipmi_open_file_write(ofile);
	if (fp == NULL)
		return -1;

	for (sdrr = sdr_list_head; sdrr != NULL; sdrr = sdrr->next) {
		int r;
		uint8_t h[5];

		/* build and write sdr header */
		h[0] = sdrr->id & 0xff;   // LS Byte first
		h[1] = (sdrr->id >> 8) & 0xff;
		h[2] = sdrr->version;
		h[3] = sdrr->type;
		h[4] = sdrr->length;

		r = fwrite(h, 1, 5, fp);
		if (r != 5) {
			printf("Error writing header "
					"to output file %s", ofile);
			rc = -1;
			break;
		}

		/* write sdr entry */
		if (!sdrr->raw) {
			printf("Error: raw data is null (length=%d)", sdrr->length);
			rc = -1;
			break;
		}
		r = fwrite(sdrr->raw, 1, sdrr->length, fp);
		if (r != sdrr->length) {
			printf("Error writing %d record bytes "
					"to output file %s", sdrr->length, ofile);
			rc = -1;
			break;
		}
	}
	fclose(fp);

	return rc;
}

int ipmi_sdr_add_from_sensors(struct ipmi_intf *intf, int maxslot)
{   

	int i;
	int rc = 0;
	int slave_addr;
	int myaddr = intf->target_addr;
	/*
	   if (ipmi_sdr_repo_clear(intf)) {
	   printf("Cannot erase SDRR. Give up."); 
	   return -1; 
	   }     
	 */
	/* First fill the SDRR from local built-in sensors */
	//	rc = sdr_copy_to_sdrr(intf, 1, myaddr, myaddr);

	/* Now fill the SDRR with remote sensors */
	if( maxslot != 0 ) {
		for (i = 0, slave_addr = 0xB0; i < maxslot; i++, slave_addr += 2) {
			/* Hole in the PICMG 2.9 mapping */
			if (slave_addr == 0xC2) slave_addr += 2;
			//			if(sdr_copy_to_sdrr(intf, 0, slave_addr, myaddr) < 0)
			//			{ 
			//				rc = -1;
			//			}       
		}              
	}             
	return rc;        
}               

int ipmi_sdr_add_from_file(struct ipmi_intf *intf, const char *ifile)
{           
	int rc;       
	struct sdrr_queue sdrr_queue;
	struct sdr_record_list *sdrr;
	struct sdr_record_list *sdrr_next;

	/* read the SDR records from file */
	//	rc = ipmi_sdr_read_records(ifile, &sdrr_queue);

	//	if (ipmi_sdr_repo_clear(intf)) {
	//		printf("Cannot erase SDRR. Giving up.");
	/* FIXME: free sdr list */
	//		return -1;
	//	}             

	/* write the SDRs to the SDR Repository */
	for (sdrr = sdrr_queue.head; sdrr != NULL; sdrr = sdrr_next) {
		sdrr_next = sdrr->next;
		//		rc = ipmi_sdr_add_record(intf, sdrr);
		if(rc < 0){
			printf("Cannot add SDR ID 0x%04x to repository...", sdrr->id);
		}
		free(sdrr);
		sdrr = NULL;
	} 
	return rc;
}

int ipmi_sdr_add_from_list(struct ipmi_intf *intf, const char *rangeList)
{       
	int rc = 0;
	int slave_addr;
	int myaddr = intf->target_addr;
	unsigned char listValue[MAX_NUM_SLOT];

	memset( listValue, 0, MAX_NUM_SLOT );

	/* Build list from string */
	//	if(ipmi_parse_range_list(rangeList, listValue) != 0)
	//	{         
	//		printf("Range - List invalid, cannot be parsed.");
	//		return -1;  
	//	}         

	{             
		unsigned char counter = 0;
		printf("List to scan: (Built-in) ");
		while(listValue[counter] != 0)
		{   
			printf("%02x ", listValue[counter]);
			counter++;
		}
		printf("\n");
	} 
	/*
	   printf("Clearing SDR Repository\n");
	   if (ipmi_sdr_repo_clear(intf)) {
	   printf("Cannot erase SDRR. Give up.");
	   return -1;
	   }
	 */
	/* First fill the SDRR from local built-in sensors */
	printf("Sanning built-in sensors..\n");
	//	rc = sdr_copy_to_sdrr(intf, 1, myaddr, myaddr);

	/* Now fill the SDRR with provided sensors list */
	{ 
		unsigned char counter = 0;
		while((rc == 0) && (listValue[counter] != 0))
		{
			slave_addr = listValue[counter];
			printf("Scanning %02Xh..\n", slave_addr);
			//			if(sdr_copy_to_sdrr(intf, 0, slave_addr, myaddr) < 0)
			//			{
			//				rc = -1;
			//			}
			counter++;
		}
	}

	return rc;
}


void printf_sdr_usage()
{
	   printf( 
	   "usage: sdr <command> [options]");
	   printf( 
	   "               list | elist [option]");
	   printf( 
	   "                     all           All SDR Records");
	   printf( 
	   "                     full          Full Sensor Record");
	   printf( 
	   "                     compact       Compact Sensor Record");
	   printf( 
	   "                     event         Event-Only Sensor Record");
	   printf( 
	   "                     mcloc         Management Controller Locator Record");
	   printf( 
	   "                     fru           FRU Locator Record");
	   printf( 
	   "                     generic       Generic Device Locator Record\n");
	   printf( 
	   "               type [option]");
	   printf( 
	   "                     <Sensor_Type> Retrieve the state of specified sensor.");
	   printf( 
	   "                                   Sensor_Type can be specified either as");
	   printf( 
	   "                                   a string or a hex value.");
	   printf( 
	   "                     list          Get a list of available sensor types\n");
	   printf( 
	   "               get <Sensor_ID>");
	   printf( 
	   "                     Retrieve state of the first sensor matched by Sensor_ID\n");
	   printf( 
	   "               info");
	   printf( 
	   "                     Display information about the repository itself\n");
	   printf( 
	   "               entity <Entity_ID>[.<Instance_ID>]");
	   printf( 
	   "                     Display all sensors associated with an entity\n");
	   printf( 
	   "               dump <file>");
	   printf( 
	   "                     Dump raw SDR data to a file\n");
	   printf( 
	   "               fill <option>");
	   printf( 
	   "                     sensors       Creates the SDR repository for the current");
	   printf( 
	   "                                   configuration");
	   printf( 
	   "                     nosat         Creates the SDR repository for the current");
	   printf( 
	   "                                   configuration, without satellite scan");
	   printf( 
	   "                     file <file>   Load SDR repository from a file");
	   printf( 
	   "                     range <range> Load SDR repository from a provided list");
	   printf( 
	   "                                   or range. Use ',' for list or '-' for");
	   printf( 
	   "                                   range, eg. 0x28,0x32,0x40-0x44");
}


#endif //IPMI_IPMI_SDR_H_H


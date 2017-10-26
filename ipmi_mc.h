/*
 * Created by keti on 2017. 10. 17...
 */

#ifndef IPMI_IPMI_MC_H_H
#define IPMI_IPMI_MC_H_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ipmitool.h"
/* ========================= ipmi_mc.c ================================= */

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
#define IPM_DEV_ADTL_SUPPORT_BITS      (8)

const struct valstr ipmi_oem_info[] = {

	{ IPMI_OEM_UNKNOWN,                "Unknown" },
	{ IPMI_OEM_HP,                     "Hewlett-Packard" },
	{ IPMI_OEM_SUN,                    "Sun Microsystems" },
	{ IPMI_OEM_INTEL,                  "Intel Corporation" },
	{ IPMI_OEM_LMC,                    "LMC" },
	{ IPMI_OEM_RADISYS,                "RadiSys Corporation" },
	{ IPMI_OEM_TYAN,                   "Tyan Computer Corporation" },
	{ IPMI_OEM_NEWISYS,                "Newisys" },
	{ IPMI_OEM_SUPERMICRO,             "Supermicro" },
	{ IPMI_OEM_GOOGLE,                 "Google" },
	{ IPMI_OEM_KONTRON,                "Kontron" },
	{ IPMI_OEM_NOKIA,                  "Nokia" },
	{ IPMI_OEM_PICMG,                  "PICMG" },
	{ IPMI_OEM_PEPPERCON,              "Peppercon AG" },
	{ IPMI_OEM_DELL,                   "DELL Inc" },
	{ IPMI_OEM_NEC,                    "NEC" },
	{ IPMI_OEM_MAGNUM,                 "Magnum Technologies" },
	{ IPMI_OEM_FUJITSU_SIEMENS,        "Fujitsu Siemens" },
	{ IPMI_OEM_TATUNG,                 "Tatung" },
	{ IPMI_OEM_AMI,                    "AMI" },
	{ IPMI_OEM_RARITAN,                "Raritan" },
	{ IPMI_OEM_AVOCENT,                "Avocent" },
	{ IPMI_OEM_OSA,                    "OSA" },
	{ IPMI_OEM_TOSHIBA,                "Toshiba" },
	{ IPMI_OEM_HITACHI_116,            "Hitachi" },
	{ IPMI_OEM_HITACHI_399,            "Hitachi" },
	{ IPMI_OEM_NOKIA_SIEMENS_NETWORKS, "Nokia Siemens Networks" },
	{ IPMI_OEM_BULL,                   "Bull Company" },
	{ IPMI_OEM_PPS,                    "Pigeon Point Systems" },
	{ IPMI_OEM_BROADCOM,               "Broadcom Corporation" },
	{ IPMI_OEM_ERICSSON,               "Ericsson AB"},
	{ IPMI_OEM_QUANTA,                 "Quanta" },
	{ IPMI_OEM_ADVANTECH,              "Advantech" },
	{ 0xffff , NULL },
};  

const struct oemvalstr ipmi_oem_product_info[] = {
	/* Keep OEM grouped together */
	/* Intel stuff, thanks to Tim Bell */
	{ IPMI_OEM_INTEL, 0x000C, "TSRLT2" },
	{ IPMI_OEM_INTEL, 0x001B, "TIGPR2U" },
	{ IPMI_OEM_INTEL, 0x0022, "TIGI2U" },
	{ IPMI_OEM_INTEL, 0x0026, "Bridgeport" },
	{ IPMI_OEM_INTEL, 0x0028, "S5000PAL" },
	{ IPMI_OEM_INTEL, 0x0029, "S5000PSL" },
	{ IPMI_OEM_INTEL, 0x0100, "Tiger4" },
	{ IPMI_OEM_INTEL, 0x0103, "McCarran" },
	{ IPMI_OEM_INTEL, 0x0800, "ZT5504" },
	{ IPMI_OEM_INTEL, 0x0808, "MPCBL0001" },
	{ IPMI_OEM_INTEL, 0x0811, "TIGW1U" },
	{ IPMI_OEM_INTEL, 0x4311, "NSI2U" },
	/* Kontron */
	{ IPMI_OEM_KONTRON,4000, "AM4000 AdvancedMC" },
	{ IPMI_OEM_KONTRON,4001, "AM4001 AdvancedMC" },
	{ IPMI_OEM_KONTRON,4002, "AM4002 AdvancedMC" },
	{ IPMI_OEM_KONTRON,4010, "AM4010 AdvancedMC" },
	{ IPMI_OEM_KONTRON,5503, "AM4500/4520 AdvancedMC" },
	{ IPMI_OEM_KONTRON,5504, "AM4300 AdvancedMC" },
	{ IPMI_OEM_KONTRON,5507, "AM4301 AdvancedMC" },
	{ IPMI_OEM_KONTRON,5508, "AM4330 AdvancedMC" },
	{ IPMI_OEM_KONTRON,5520, "KTC5520/EATX" },
	{ IPMI_OEM_KONTRON,5703, "RTM8020" },
	{ IPMI_OEM_KONTRON,5704, "RTM8030" },
	{ IPMI_OEM_KONTRON,5705, "RTM8050" },
	{ IPMI_OEM_KONTRON,6000, "CP6000" },
	{ IPMI_OEM_KONTRON,6006, "DT-64" },
	{ IPMI_OEM_KONTRON,6010, "CP6010" },
	{ IPMI_OEM_KONTRON,6011, "CP6011" },
	{ IPMI_OEM_KONTRON,6012, "CP6012" },
	{ IPMI_OEM_KONTRON,6014, "CP6014" },
	{ IPMI_OEM_KONTRON,5002, "AT8001" },
	{ IPMI_OEM_KONTRON,5003, "AT8010" },
	{ IPMI_OEM_KONTRON,5004, "AT8020" },
	{ IPMI_OEM_KONTRON,5006, "AT8030 IPMC" },
	{ IPMI_OEM_KONTRON,2025, "AT8030 MMC" },
	{ IPMI_OEM_KONTRON,5007, "AT8050" },
	{ IPMI_OEM_KONTRON,5301, "AT8400" },
	{ IPMI_OEM_KONTRON,5303, "AT8901" },
	/* Broadcom */
	{ IPMI_OEM_BROADCOM, 5725, "BCM5725" },
	/* Ericsson */
	{ IPMI_OEM_ERICSSON, 0x0054, "Phantom" },
	/* Advantech */
	/* ATCA Blades */
	{ IPMI_OEM_ADVANTECH, 0x3393, "MIC-3393" },
	{ IPMI_OEM_ADVANTECH, 0x3395, "MIC-3395" },
	{ IPMI_OEM_ADVANTECH, 0x3396, "MIC-3396" },
	{ IPMI_OEM_ADVANTECH, 0x5302, "MIC-5302" },
	{ IPMI_OEM_ADVANTECH, 0x5304, "MIC-5304" },
	{ IPMI_OEM_ADVANTECH, 0x5320, "MIC-5320" },
	{ IPMI_OEM_ADVANTECH, 0x5321, "MIC-5321" },
	{ IPMI_OEM_ADVANTECH, 0x5322, "MIC-5322" },
	{ IPMI_OEM_ADVANTECH, 0x5332, "MIC-5332" },
	{ IPMI_OEM_ADVANTECH, 0x5333, "MIC-5333" },
	{ IPMI_OEM_ADVANTECH, 0x5342, "MIC-5342" },
	{ IPMI_OEM_ADVANTECH, 0x5343, "MIC-5343" },
	{ IPMI_OEM_ADVANTECH, 0x5344, "MIC-5344" },
	{ IPMI_OEM_ADVANTECH, 0x5345, "MIC-5345" },
	{ IPMI_OEM_ADVANTECH, 0x5201, "MIC-5201 Dual 10GE AMC"},
	{ IPMI_OEM_ADVANTECH, 0x5203, "MIC-5203 Quad GbE AMC"},
	{ IPMI_OEM_ADVANTECH, 0x5212, "MIC-5212 Dual 10GE AMC"},
	/* AdvancedMC */
	{ IPMI_OEM_ADVANTECH, 0x5401, "MIC-5401" },
	{ IPMI_OEM_ADVANTECH, 0x5601, "MIC-5601" },
	{ IPMI_OEM_ADVANTECH, 0x5602, "MIC-5602" },
	{ IPMI_OEM_ADVANTECH, 0x5604, "MIC-5604" },
	{ IPMI_OEM_ADVANTECH, 0x5603, "MIC-5603" },
	{ IPMI_OEM_ADVANTECH, 0x6311, "MIC-6311" },
	{ IPMI_OEM_ADVANTECH, 0x6313, "MIC-6313" },
	{ IPMI_OEM_ADVANTECH, 0x8301, "MIC-8301" },
	{ IPMI_OEM_ADVANTECH, 0x8302, "MIC-8302" },
	{ IPMI_OEM_ADVANTECH, 0x8304, "MIC-8304" },
	{ IPMI_OEM_ADVANTECH, 0x5101, "RTM-5101" },
	{ IPMI_OEM_ADVANTECH, 0x5102, "RTM-5102" },
	{ IPMI_OEM_ADVANTECH, 0x5106, "RTM-5106" },
	{ IPMI_OEM_ADVANTECH, 0x5107, "RTM-5107" },
	{ IPMI_OEM_ADVANTECH, 0x5210, "RTM-5210" },
	{ IPMI_OEM_ADVANTECH, 0x5220, "RTM-5220" },
	{ IPMI_OEM_ADVANTECH, 0x5104, "RTM-5104" },
	{ IPMI_OEM_ADVANTECH, 0x5500, "UTCA-5500"},
	{ IPMI_OEM_ADVANTECH, 0x5503, "UTCA-5503"},
	{ IPMI_OEM_ADVANTECH, 0x5504, "UTCA-5504"},
	{ IPMI_OEM_ADVANTECH, 0x5801, "UTCA-5801"},
	{ IPMI_OEM_ADVANTECH, 0x2210, "NCPB-2210"},
	{ IPMI_OEM_ADVANTECH, 0x2305, "NCPB-2305"},
	{ IPMI_OEM_ADVANTECH, 0x2320, "NCPB-2320"},
	{ IPMI_OEM_ADVANTECH, 0x3109, "NCP-3109" },
	{ IPMI_OEM_ADVANTECH, 0x3110, "NCP-3110" },
	{ IPMI_OEM_ADVANTECH, 0x3200, "NCP-3200" },
	{ IPMI_OEM_ADVANTECH, 0x5060, "SMM-5060" },
	{ IPMI_OEM_ADVANTECH, 0x3210, "FWA-3210" },
	{ IPMI_OEM_ADVANTECH, 0x3220, "FWA-3220" },
	{ IPMI_OEM_ADVANTECH, 0x3221, "FWA-3221" },
	{ IPMI_OEM_ADVANTECH, 0x3230, "FWA-3230" },
	{ IPMI_OEM_ADVANTECH, 0x3231, "FWA-3231" },
	{ IPMI_OEM_ADVANTECH, 0x3233, "FWA-3233" },
	{ IPMI_OEM_ADVANTECH, 0x3250, "FWA-3250" },
	{ IPMI_OEM_ADVANTECH, 0x3260, "FWA-3260" },
	{ IPMI_OEM_ADVANTECH, 0x5020, "FWA-5020" },
	{ IPMI_OEM_ADVANTECH, 0x6510, "FWA-6510" },
	{ IPMI_OEM_ADVANTECH, 0x6511, "FWA-6511" },
	{ IPMI_OEM_ADVANTECH, 0x6512, "FWA-6512" },
	{ IPMI_OEM_ADVANTECH, 0x6520, "FWA-6520" },
	{ IPMI_OEM_ADVANTECH, 0x6521, "FWA-6521" },
	{ IPMI_OEM_ADVANTECH, 0x6522, "FWA-6522" },
	{ IPMI_OEM_ADVANTECH, 0x7310, "ATCA-7310"},
	{ IPMI_OEM_ADVANTECH, 0x7330, "ATCA-7330"},
	{ IPMI_OEM_ADVANTECH, 0x7410, "ATCA-7410"},
	{ IPMI_OEM_ADVANTECH, 0x9023, "ATCA-9023"},
	{ IPMI_OEM_ADVANTECH, 0x9112, "ATCA-9112"},
	{ IPMI_OEM_ADVANTECH, 0x4201, "AMC-4201" },
	{ IPMI_OEM_ADVANTECH, 0x4202, "AMC-4202" },
	{ IPMI_OEM_ADVANTECH, 0x3211, "NAMB-3211"},
	{ IPMI_OEM_ADVANTECH, 0x1207, "CPCI-1207"},
	{ IPMI_OEM_ADVANTECH, 0x120E, "CPCI-1207 Test Board"},
	{ IPMI_OEM_ADVANTECH, 0x1304, "CPCI-1304"},
	{ IPMI_OEM_ADVANTECH, 0x7001, "CPCI-7001"},
	{ IPMI_OEM_ADVANTECH, 0x8220, "CPCI-8220"},
	{ IPMI_OEM_ADVANTECH, 0x9001, "ESP-9001" },
	{ IPMI_OEM_ADVANTECH, 0x9002, "ESP-9002" },
	{ IPMI_OEM_ADVANTECH, 0x9012, "ESP-9012" },
	{ IPMI_OEM_ADVANTECH, 0x9212, "ESP-9212" },
	{ IPMI_OEM_ADVANTECH, 0x6000, "CGS-6000" },
	{ IPMI_OEM_ADVANTECH, 0x6010, "CGS-6010" },

	{ 0xffffff        , 0xffff , NULL },
};

/* IPM Device, Get Device ID Command - Additional Device Support */
const char *ipm_dev_adtl_dev_support[8] = {
	"Sensor Device",         /* bit 0 */
	"SDR Repository Device", /* bit 1 */
	"SEL Device",            /* bit 2 */
	"FRU Inventory Device",  /*  ...  */
	"IPMB Event Receiver",
	"IPMB Event Generator",
	"Bridge",
	"Chassis Device"         /* bit 7 */
};

struct bitfield_data {
	const char * name;
	const char * desc;
	uint32_t mask;
} mc_enables_bf[] = {
	{
		.name = "recv_msg_intr",
		.desc = "Receive Message Queue Interrupt",
		.mask = 1<<0,
	},
	{
		.name = "event_msg_intr",
		.desc = "Event Message Buffer Full Interrupt",
		.mask = 1<<1,
	},
	{
		.name = "event_msg",
		.desc = "Event Message Buffer",
		.mask = 1<<2,
	},
	{
		.name = "system_event_log",
		.desc = "System Event Logging",
		.mask = 1<<3,
	},
	{
		.name = "oem0",
		.desc = "OEM 0",
		.mask = 1<<5,
	},
	{
		.name = "oem1",
		.desc = "OEM 1",
		.mask = 1<<6,
	},
	{
		.name = "oem2",
		.desc = "OEM 2",
		.mask = 1<<7,
	},
	{ NULL },
};

struct ipmi_guid {
	uint32_t  time_low; /* timestamp low field */
	uint16_t  time_mid; /* timestamp middle field */
	uint16_t  time_hi_and_version; /* timestamp high field and version number */
	uint8_t   clock_seq_hi_variant;/* clock sequence high field and variant */
	uint8_t   clock_seq_low; /* clock sequence low field */
	uint8_t   node[6];  /* node */
};
//} ATTRIBUTE_PACKING;

int ipmi_mc_main(struct ipmi_intf * intf, int argc, char ** argv);
static void printf_sysinfo_usage(int full_help);
static void printf_mc_usage(void);
static int ipmi_mc_get_deviceid(struct ipmi_intf * intf);
static int ipmi_mc_get_guid(struct ipmi_intf * intf);
uint16_t buf2short(uint8_t * buf);

uint16_t buf2short(uint8_t * buf)
{       
	return (uint16_t)(buf[1] << 8 | buf[0]);
}   

/*
   void lprintf(int level, const char * format, ...)
   {
   static char logmsg[LOG_MSG_LENGTH];
   va_list vptr;

   if (!logpriv)
   log_reinit(); 

   if (logpriv->level < level)
   return;

   va_start(vptr, format);
   vsnprintf(logmsg, LOG_MSG_LENGTH, format, vptr);
   va_end(vptr);

   if (logpriv->daemon)
   syslog(level, "%s", logmsg);
   else
   fprintf(stderr, "%s\n", logmsg);
   return;
   }   
 */

/* ipmi_mc_main  -  top-level handler for MC functions
 *
 * @intf:   ipmi interface
 * @argc:   number of arguments
 * @argv:   argument list
 *
 * returns 0 on success
 * returns -1 on error
 */
int ipmi_mc_main(struct ipmi_intf * intf, int argc, char ** argv)
{
	int rc = 0; 

	if (argc < 1) { 
		//lprintf(LOG_ERR, "Not enough parameters given.");
		printf("Not enough parameters given.\n");
		printf_mc_usage();
		rc = (-1);
	}    
	/*
	   else if (strncmp(argv[2], "help", 4) == 0) { 
	   printf_mc_usage();
	   rc = 0; 
	   }    
	   else if (strncmp(argv[0], "reset", 5) == 0) { 
	   if (argc < 2) { 
	   lprintf(LOG_ERR, "Not enough parameters given.");
	   printf_mc_reset_usage();
	   rc = (-1);
	   }    
	   else if (strncmp(argv[1], "help", 4) == 0) { 
	   printf_mc_reset_usage();
	   rc = 0; 
	   }    
	   else if (strncmp(argv[1], "cold", 4) == 0) { 
	   rc = ipmi_mc_reset(intf, BMC_COLD_RESET);
	   }    
	   else if (strncmp(argv[1], "warm", 4) == 0) { 
	   rc = ipmi_mc_reset(intf, BMC_WARM_RESET);
	   }    
	   else {
	   lprintf(LOG_ERR, "Invalid mc/bmc %s command: %s", argv[0], argv[1]);
	   printf_mc_reset_usage();
	   rc = (-1);
	   }    
	   } 
	 */
	else if (strncmp(argv[2], "info", 4) == 0) { 
		rc = ipmi_mc_get_deviceid(intf);
	}    
	else if (strncmp(argv[2], "guid", 4) == 0) { 
		rc = ipmi_mc_get_guid(intf);
	} 
	/*
	   else if (strncmp(argv[0], "getenables", 10) == 0) { 
	   rc = ipmi_mc_get_enables(intf);
	   }    
	   else if (strncmp(argv[0], "setenables", 10) == 0) { 
	   rc = ipmi_mc_set_enables(intf, argc-1, &(argv[1]));
	   }    
	   else if (!strncmp(argv[0], "selftest", 8)) {
	   rc = ipmi_mc_get_selftest(intf);
	   }
	   else if (!strncmp(argv[0], "watchdog", 8)) {
	   if (argc < 2) {
	   lprintf(LOG_ERR, "Not enough parameters given.");
	   print_watchdog_usage();
	   rc = (-1);
	   }
	   else if (strncmp(argv[1], "help", 4) == 0) {
	   print_watchdog_usage();
	   rc = 0;
	   }
	   else if (strncmp(argv[1], "get", 3) == 0) {
	   rc = ipmi_mc_get_watchdog(intf);
	   }
	   else if(strncmp(argv[1], "off", 3) == 0) {
	   rc = ipmi_mc_shutoff_watchdog(intf);
	   }
	   else if(strncmp(argv[1], "reset", 5) == 0) {
	   rc = ipmi_mc_rst_watchdog(intf);
	   }
	   else {
	   lprintf(LOG_ERR, "Invalid mc/bmc %s command: %s", argv[0], argv[1]);
	   print_watchdog_usage();
	   rc = (-1);
	   }
	   }
	   else if (strncmp(argv[0], "getsysinfo", 10) == 0) {
	   rc = ipmi_sysinfo_main(intf, argc, argv, 0);
	   }
	   else if (strncmp(argv[0], "setsysinfo", 10) == 0) {
	   rc = ipmi_sysinfo_main(intf, argc, argv, 1);
	   }
	   else {
	   lprintf(LOG_ERR, "Invalid mc/bmc command: %s", argv[0]);
	   printf_mc_usage();
	   rc = (-1);
	   }
	 */
	return rc;
}

static void printf_sysinfo_usage(int full_help)
{
	if (full_help != 0)
		printf("usage:\n");

	printf("  getsysinfo <argument>\n");

	if (full_help != 0) {
		printf("    Retrieves system info from BMC for given argument\n");
	}

	printf("  setsysinfo <argument> <string>\n");

	if (full_help != 0) {
		printf("    Stores system info string for given argument to BMC\n");
		printf("\n");
		printf("  Valid arguments are:\n");
	}
	printf("    system_fw_version   System firmware (e.g. BIOS) version\n");
	printf(
			"    primary_os_name     Primary operating system name\n");
	printf("    os_name             Operating system name\n");
	printf("    system_name         System Name of server(vendor dependent)\n");
	printf("    delloem_os_version  Running version of operating system\n");
	printf("    delloem_url         URL of BMC webserver\n");
	printf("\n");
}

static void printf_mc_usage(void)
{
	struct bitfield_data * bf;
	printf("MC Commands:\n");
	printf("  reset <warm|cold>\n");
	printf("  guid\n");
	printf("  info\n");
	printf("  watchdog <get|reset|off>\n");
	printf("  selftest\n");
	printf("  getenables\n");
	printf("  setenables <option=on|off> ...\n");
	for (bf = mc_enables_bf; bf->name != NULL; bf++) {
		printf("    %-20s  %s\n", bf->name, bf->desc);
	}
	printf_sysinfo_usage(0);
}

/* ipmi_mc_get_deviceid  -  print information about this MC
 *
 * @intf:   ipmi interface
 *
 * returns 0 on success
 * returns -1 on error
 */
static int ipmi_mc_get_deviceid(struct ipmi_intf * intf)
{
	struct ipmi_rs * rsp;
	struct ipmi_rq req;
	struct ipm_devid_rsp *devid;
	int i;
	const char *product=NULL;

	memset(&req, 0, sizeof(req));
	req.msg.netfn = IPMI_APP_NETFN;
	req.msg.cmd = BMC_GET_DEVICE_ID;
	req.msg.data_len = 0;

	rsp = intf->sendrecv(intf, &req);
	if (rsp == NULL) {
		printf("Get Device ID command failed\n");
		return -1;
	}
	if (rsp->ccode > 0) {
		printf("Get Device ID command failed: %s\n", val2str(rsp->ccode, completion_code_vals));
		return -1;
	}
	/*
	   devid = (struct ipm_devid_rsp *) rsp->data;
	   printf("Device ID                 : %i\n",
	   devid->device_id);
	   printf("Device Revision           : %i\n",
	   devid->device_revision & IPM_DEV_DEVICE_ID_REV_MASK);
	   printf("Firmware Revision         : %u.%02x\n",
	   devid->fw_rev1 & IPM_DEV_FWREV1_MAJOR_MASK,
	   devid->fw_rev2);
	   printf("IPMI Version              : %x.%x\n",
	   IPM_DEV_IPMI_VERSION_MAJOR(devid->ipmi_version),
	   IPM_DEV_IPMI_VERSION_MINOR(devid->ipmi_version));
	   printf("Manufacturer ID           : %lu\n",
	   (long)IPM_DEV_MANUFACTURER_ID(devid->manufacturer_id));
	   printf("Manufacturer Name         : %s\n",
	   val2str( (long)IPM_DEV_MANUFACTURER_ID(devid->manufacturer_id),
	   ipmi_oem_info) );

	   printf("Product ID                : %u (0x%02x%02x)\n",
	   buf2short((uint8_t *)(devid->product_id)),
	   devid->product_id[1], devid->product_id[0]);

	   product=oemval2str(IPM_DEV_MANUFACTURER_ID(devid->manufacturer_id),
	   (devid->product_id[1]<<8)+devid->product_id[0],
	   ipmi_oem_product_info);

	   if (product!=NULL) {
	   printf("Product Name              : %s\n", product);
	   }

	   printf("Device Available          : %s\n",
	   (devid->fw_rev1 & IPM_DEV_FWREV1_AVAIL_MASK) ?
	   "no" : "yes");
	   printf("Provides Device SDRs      : %s\n",
	   (devid->device_revision & IPM_DEV_DEVICE_ID_SDR_MASK) ?
	   "yes" : "no");
	   printf("Additional Device Support :\n");
	   for (i = 0; i < IPM_DEV_ADTL_SUPPORT_BITS; i++) {
	   if (devid->adtl_device_support & (1 << i)) {
	   printf("    %s\n", ipm_dev_adtl_dev_support[i]);
	   }
	   }
	   if (rsp->data_len == sizeof(*devid)) {
	   printf("Aux Firmware Rev Info     : \n");*/
/* These values could be looked-up by vendor if documented,
 * so we put them on individual lines for better treatment later
 *//*
	  printf("    0x%02x\n    0x%02x\n    0x%02x\n    0x%02x\n",
	  devid->aux_fw_rev[0],
	  devid->aux_fw_rev[1],
	  devid->aux_fw_rev[2],
	  devid->aux_fw_rev[3]);
	  }*/
	return 0;
}

/* ipmi_mc_get_guid  -  print this MC GUID
 *
 * @intf:   ipmi interface
 *
 * returns 0 on success
 * returns -1 on error
 */
static int ipmi_mc_get_guid(struct ipmi_intf * intf)
{
	struct ipmi_rs * rsp;
	struct ipmi_rq req;
	struct ipmi_guid guid;

	memset(&req, 0, sizeof(req));
	req.msg.netfn = IPMI_APP_NETFN;
	req.msg.cmd = BMC_GET_GUID;

	rsp = intf->sendrecv(intf, &req);
	if (rsp == NULL) {
		printf("Get GUID command failed\n");
		return -1;
	}
	if (rsp->ccode > 0) {
		printf("Get GUID command failed: %s\n", val2str(rsp->ccode, completion_code_vals));
		return -1;
	}

	if (rsp->data_len == sizeof(struct ipmi_guid)) {
		char tbuf[40];
		time_t s;
		memset(tbuf, 0, 40);
		memset(&guid, 0, sizeof(struct ipmi_guid));
		memcpy(&guid, rsp->data, rsp->data_len);

		/* Kipp - changed order of last field (node) to follow specification */
		/*printf("System GUID  : %08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x\n",
		  guid.time_low, guid.time_mid, guid.time_hi_and_version,
		  guid.clock_seq_hi_variant << 8 | guid.clock_seq_low,
		  guid.node[0], guid.node[1], guid.node[2],
		  guid.node[3], guid.node[4], guid.node[5]);

		  s = (time_t)guid.time_low;*/ /* Kipp - removed the BSWAP_32, it was not needed here */
		strftime(tbuf, sizeof(tbuf), "%m/%d/%Y %H:%M:%S", localtime(&s));
		//	printf("Timestamp    : %s\n", tbuf);
	}
	else {
		return 0;
		//		printf("Invalid GUID length %d\n", rsp->data_len);
	}

	return 0;
}

#endif //IPMI_IPMI_MC_H_H


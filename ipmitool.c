#include <stdio.h>
#include "ipmitool.h"
#include "ipmi_chassis.h"
#include "ipmi_user.h.h"
#include "ipmi_lanp.h"
#include "ipmi_mc.h"
#include "ipmi_fru.h"

int main(int argc, char *argv[])
{
	int req_len, res_len;
	char *dev_node = "/dev/ast-kcs.2";
	fd = open(dev_node, O_RDWR);

	uint8_t channel = 1;
	struct ipmi_intf *intf = &ipmi_bmc_intf;

	if(strcmp(argv[1],"user") == 0) {
		ipmi_user_main(intf, argc, argv);
	}
	//      ipmi_lan_print(intf, channel);
	else if((strcmp(argv[1], "chassis")==0) || (strcmp(argv[1], "power") == 0)){
		ipmi_power_main(intf, argc, argv);
	}
	else if((strcmp(argv[1], "lan") == 0)){
		ipmi_lanp_main(intf, argc, argv);
	}
	else if(strcmp(argv[1], "mc") == 0){
		ipmi_mc_main(intf, argc, argv);
	}
	///// lika
	else if((strcmp(argv[1], "fru") == 0)){
		ipmi_fru_main(intf, argc, argv);
	}
	return 0;
}



//
// Created by keti on 2017. 10. 12..
//

#ifndef IPMI_IPMI_CHASSIS_H_H
#define IPMI_IPMI_CHASSIS_H_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ipmitool.h"
/* ========================= ipmi_chassis.c ================================= */

const struct valstr ipmi_chassis_power_control_vals[] = {
        { IPMI_CHASSIS_CTL_POWER_DOWN,   "Down/Off" },
        { IPMI_CHASSIS_CTL_POWER_UP,     "Up/On" },
        { IPMI_CHASSIS_CTL_POWER_CYCLE,  "Cycle" },
        { IPMI_CHASSIS_CTL_HARD_RESET,   "Reset" },
        { IPMI_CHASSIS_CTL_PULSE_DIAG,   "Diag" },
        { IPMI_CHASSIS_CTL_ACPI_SOFT,    "Soft" },
        { 0x00, NULL },
};

int ipmi_chassis_power_status(struct ipmi_intf * intf)
{
    struct ipmi_rs * rsp;
    struct ipmi_rq req;

    memset(&req, 0, sizeof(req));
    req.msg.netfn = IPMI_CHASSIS_NETFN;
    req.msg.cmd = IPMI_CHASSIS_POWER_STATUS;
    req.msg.data_len = 0;

    rsp = intf->sendrecv(intf, &req);

    if (rsp == NULL) {
        printf("rsp is null!\n");
        printf("Unable to get Chassis Power Status\n");
        return -1;
    }
    if (rsp->ccode > 0) {
        printf("Get Chassis Power Status Failed\n");
        //lprintf(LOG_ERR, "Get Chassis Power Status failed: %s",       val2str(rsp->ccode, completion_code_vals));
        return -1;
    }

    return rsp->data[0] & 1;
}

int ipmi_chassis_print_power_status(struct ipmi_intf * intf)
{
    int ps = ipmi_chassis_power_status(intf);

    if (ps < 0)
        return -1;

    printf("Chassis Power is %s\n", ps ? "on" : "off");

    return 0;
}

int ipmi_chassis_power_control(struct ipmi_intf * intf, uint8_t ctl)
{
    struct ipmi_rs * rsp;
    struct ipmi_rq req;

    memset(&req, 0, sizeof(req));
    req.msg.netfn = IPMI_CHASSIS_NETFN;
    req.msg.cmd = IPMI_CHASSIS_POWER_CTL;
    req.msg.data = &ctl;
    req.msg.data_len = 1;

    rsp = intf->sendrecv(intf, &req);
    if (rsp == NULL) {
        printf("Unable to set Chassis Poser control\n");
        //lprintf(LOG_ERR, "Unable to set Chassis Power Control to %s", val2str(ctl, ipmi_chassis_power_control_vals));
        return -1;
    }
    if (rsp->ccode > 0) {

        /*
           lprintf(LOG_ERR, "Set Chassis Power Control to %s failed: %s",
           val2str(ctl, ipmi_chassis_power_control_vals),
           val2str(rsp->ccode, completion_code_vals));
         */
        return -1;
    }

    printf("Chassis Power Control: %s\n",
           val2str(ctl, ipmi_chassis_power_control_vals));

#if 0   /* this can cause sessions to hang around after power commands */
    /* sessions often get lost when changing chassis power */
        intf->abort = 1;
#endif

    return 0;
}
int ipmi_power_main(struct ipmi_intf * intf, int argc, char ** argv)
    {
        int rc = 0;
        uint8_t ctl = 0;

        if ((argc < 1) || (strncmp(argv[2], "help", 4) == 0)) {
            //lprintf(LOG_NOTICE, "chassis power Commands: status, on, off, cycle, reset, diag, soft");
            return 0;
        }
        if (strncmp(argv[2], "status", 6) == 0) {

            rc = ipmi_chassis_print_power_status(intf);
            return rc;
        }
	else if(strcmp(argv[2], "power") == 0){
	if (strncmp(argv[3], "status", 6) == 0) {

            rc = ipmi_chassis_print_power_status(intf);
            return rc;
        }
        else if ((strncmp(argv[3], "up", 2) == 0) || (strncmp(argv[3], "on", 2) == 0))
            ctl = IPMI_CHASSIS_CTL_POWER_UP;
        else if ((strncmp(argv[3], "down", 4) == 0) || (strncmp(argv[3], "off", 3) == 0))
            ctl = IPMI_CHASSIS_CTL_POWER_DOWN;
        else if (strncmp(argv[3], "cycle", 5) == 0)
            ctl = IPMI_CHASSIS_CTL_POWER_CYCLE;
        else if (strncmp(argv[3], "reset", 5) == 0)
            ctl = IPMI_CHASSIS_CTL_HARD_RESET;
        else if (strncmp(argv[3], "diag", 4) == 0)
            ctl = IPMI_CHASSIS_CTL_PULSE_DIAG;
	else if ((strncmp(argv[3], "acpi", 4) == 0) || (strncmp(argv[3], "soft", 4) == 0))
            ctl = IPMI_CHASSIS_CTL_ACPI_SOFT;
	}
    else {
        //lprintf(LOG_ERR, "Invalid chassis power command: %s", argv[0]);
        return -1;
    }

    rc = ipmi_chassis_power_control(intf, ctl);
    return rc;
}

#endif //IPMI_IPMI_CHASSIS_H_H


//
// Created by keti on 2017. 10. 15..
//

#ifndef IPMI_IPMI_LANP_H
#define IPMI_IPMI_LANP_H

#include "ipmitool.h"


static int get_lan_param_select(struct ipmi_intf * intf, struct ipmi_rs * response, struct lan_param * p, uint8_t chan, int param, int select)
{
    //struct lan_param * p = NULL;
    struct ipmi_rs * rsp;
    struct ipmi_rq req;
    int i = 0;
    uint8_t msg_data[4];

    for (i = 0; ipmi_lan_params[i].cmd != (-1); i++) {
        if (ipmi_lan_params[i].cmd == param) {
            p = &ipmi_lan_params[i];
            break;
        }
    }

    msg_data[0] = chan;
    msg_data[1] = p->cmd;
    msg_data[2] = select;
    msg_data[3] = 0;

    memset(&req, 0, sizeof(req));
    req.msg.netfn    = IPMI_TRANSPORT_NETFN;
    req.msg.cmd      = IPMI_LAN_GET_CONFIG;
    req.msg.data     = msg_data;
    req.msg.data_len = 4;

    rsp = intf->sendrecv(intf, &req);
    if (rsp == NULL) {
//        printf("rsp is null!\n");
	return -1;
    }

    switch (rsp->ccode)
    {
        case 0x00: /* successful */
	    //printf("ccode is 0x00\n");
            break;

        case 0x80: /* parameter not supported */
        case 0xc9: /* parameter out of range */
        case 0xcc: /* invalid data field in request */

            /* these completion codes usually mean parameter not supported */
            p->data = NULL;
            p->data_len = 0;
            //return p;
            break;
        default:
	    break;
            /* other completion codes are treated as error */

    }

    //printf("rsp data : ");
    if((param == IPMI_LANP_IP_ADDR) || (param == IPMI_LANP_DEF_GATEWAY_IP) || (param == IPMI_LANP_SUBNET_MASK)){
        for(i=0;i<4;i++){
	    response->data[i] = rsp->data[i];
            //printf("%02x", rsp->data[i]);
        }
    }
    else if(param == IPMI_LANP_MAC_ADDR){
	for(i=0;i<8;i++){
	    response->data[i] = rsp->data[i];
	    //printf("%02x ", rsp->data[i]);
	}
    }
    //p->data_len = rsp->data_len;// - 1;

    //return p;
}
static int ipmi_mac_addr(struct ipmi_intf *intf, uint8_t chan){

    struct lan_param p;
    struct ipmi_rs response;

    get_lan_param_select(intf, &response, &p, chan, IPMI_LANP_MAC_ADDR, 0);
    if(response.data == NULL)
	return -1;
    if(response.data != NULL)
	printf("%-24s: %02x:%02x:%02x:%02x:%02x:%02x\n", ipmi_lan_params[IPMI_LANP_MAC_ADDR].desc,
		       response.data[0], response.data[1], response.data[2], response.data[3], response.data[4], response.data[5]);

}
static int ipmi_ip_addr(struct ipmi_intf *intf, uint8_t chan){

    struct lan_param p;
    struct ipmi_rs response;
    get_lan_param_select(intf, &response, &p, chan, IPMI_LANP_IP_ADDR, 0);
    if (response.data == NULL)
    	return -1;
    if (response.data != NULL)
	printf("%-24s: %d.%d.%d.%d\n", ipmi_lan_params[IPMI_LANP_IP_ADDR].desc,
        response.data[0], response.data[1], response.data[2], response.data[3]);

}
static int ipmi_gateway_addr(struct ipmi_intf *intf, uint8_t chan){

    struct lan_param p;
    struct ipmi_rs response;
    get_lan_param_select(intf, &response, &p, chan, IPMI_LANP_DEF_GATEWAY_IP, 0);
    if (response.data == NULL)
        return -1;
    if (response.data != NULL)
        printf("%-24s: %d.%d.%d.%d\n", ipmi_lan_params[IPMI_LANP_DEF_GATEWAY_IP].desc,
        response.data[0], response.data[1], response.data[2], response.data[3]);

}
static int ipmi_subnet_addr(struct ipmi_intf *intf, uint8_t chan){

    struct lan_param p;
    struct ipmi_rs response;
    get_lan_param_select(intf, &response, &p, chan, IPMI_LANP_SUBNET_MASK, 0);
    if (response.data == NULL)
        return -1;
    if (response.data != NULL)
        printf("%-24s: %d.%d.%d.%d\n", ipmi_lan_params[IPMI_LANP_SUBNET_MASK].desc,
        response.data[0], response.data[1], response.data[2], response.data[3]);

}

static int ipmi_lan_print(struct ipmi_intf *intf, uint8_t chan){

    struct lan_param p;


}
int ipmi_lan_set_in(struct ipmi_intf *intf, uint8_t chan){
    struct lan_param p;
    struct ipmi_rs response;

    get_lan_param_select(intf, &response, &p, chan, IPMI_LANP_SET_IN_PROGRESS, 0);
    if (response.data == NULL)
        return -1;
    if (response.data != NULL) {
        printf("%-24s: ", ipmi_lan_params[IPMI_LANP_SET_IN_PROGRESS].desc);
        response.data[0] &= 3;
        switch (response.data[0]) {
            case 0:
                printf("Set Complete\n");
                break;
            case 1:
                printf("Set In Progress\n");
                break;
            case 2:
                printf("Commit Write\n");
                break;
            case 3:
                printf("Reserved\n");
                break;
            default:
                printf("Unknown\n");
        }
    }
}
int ipmi_lan_auth_type(struct ipmi_intf *intf, uint8_t chan)
{
    struct lan_param p;
    struct ipmi_rs response;
    get_lan_param_select(intf, &response, &p, chan, IPMI_LANP_AUTH_TYPE,0);
    
    if (response.data == NULL){
//	printf("p data is null\n");
	return -1;
    }
    if (response.data != NULL) {
	//printf("p data is not null\n");
        printf("%-24s: %s%s%s%s%s\n", ipmi_lan_params[IPMI_LANP_AUTH_TYPE].desc,
               (response.data[0] & 1<<IPMI_SESSION_AUTHTYPE_NONE) ? "NONE " : "",
               (response.data[0] & 1<<IPMI_SESSION_AUTHTYPE_MD2) ? "MD2 " : "",
               (response.data[0] & 1<<IPMI_SESSION_AUTHTYPE_MD5) ? "MD5 " : "",
               (response.data[0] & 1<<IPMI_SESSION_AUTHTYPE_PASSWORD) ? "PASSWORD " : "",
               (response.data[0] & 1<<IPMI_SESSION_AUTHTYPE_OEM) ? "OEM " : "");
    }
}

int ipmi_lanp_main(struct ipmi_intf *intf, int argc, char **argv)
{
    uint8_t chan = 0;
    if(argc > 1) chan = atoi(argv[3]);
    if(strcmp(argv[2], "print") == 0){
	ipmi_lan_set_in(intf, chan);
	ipmi_lan_auth_type(intf, chan);
	ipmi_ip_addr(intf, chan);
	ipmi_subnet_addr(intf, chan);
	ipmi_mac_addr(intf,chan);
	ipmi_gateway_addr(intf, chan);
	printf("\n");
    }
    else if(strcmp(argv[2], "auth") == 0){
        ipmi_lan_auth_type(intf, chan);
    }
    else if(strcmp(argv[2], "setprog") == 0){
        ipmi_lan_set_in(intf, chan);
    }
    else if(strcmp(argv[2], "ipaddr") == 0){
	ipmi_ip_addr(intf, chan);
    }
    else if(strcmp(argv[2], "macaddr") == 0){
	ipmi_mac_addr(intf,chan);
    }
    else if(strcmp(argv[2], "gateway") == 0){
	ipmi_gateway_addr(intf, chan);
    }
    else if(strcmp(argv[2], "subnet") == 0){
	ipmi_subnet_addr(intf, chan);
    }
}

#endif //IPMI_IPMI_LANP_H


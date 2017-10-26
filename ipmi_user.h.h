//
// Created by keti on 2017. 10. 12..
//

#ifndef IPMI_IPMI_USER_H_H
#define IPMI_IPMI_USER_H_H
#include <stdint.h>
#include <syslog.h>
#include "ipmitool.h"

static int ipmi_print_user_list(struct ipmi_intf *intf, uint8_t channel_number);
static int ipmi_print_user_summary(struct ipmi_intf *intf, uint8_t channel_number);
static void dump_user_access(const char *user_name, struct user_access_t *user_access);
int _ipmi_get_user_access(struct ipmi_intf *intf, struct user_access_t *user_access_rsp);
int _ipmi_get_user_name(struct ipmi_intf *intf, struct user_name_t *user_name_ptr);

int
_ipmi_set_user_password(struct ipmi_intf *intf, uint8_t user_id, const char *password, uint8_t is_twenty_byte)
{
	struct ipmi_rq req = {0};
	struct ipmi_rs *rsp;
	uint8_t *data;
	uint8_t data_len = (is_twenty_byte) ? 22 : 18;
	data = malloc(sizeof(uint8_t) * data_len);
	if (data == NULL) {
		return (-4);
	}
	memset(data, 0, data_len);
	data[0] = (is_twenty_byte) ? 0x80 : 0x00;
	data[0] |= (0x0F & user_id);
	if (password != NULL) {
		size_t copy_len = strlen(password);
		if (copy_len > (data_len - 2)) {
			copy_len = data_len - 2;
		} else if (copy_len < 1) {
			copy_len = 0;
		}
		strncpy((char *)(data + 2), password, copy_len);
	}

	req.msg.netfn = IPMI_APP_NETFN;
	req.msg.cmd = IPMI_SET_USER_PASSWORD;
	req.msg.data = data;
	req.msg.data_len = data_len;
	rsp = intf->sendrecv(intf, &req);
	free(data);
	data = NULL;
	if (rsp == NULL) {
		return (-1);
	}
	return rsp->ccode;
}

int
_ipmi_set_user_access(struct ipmi_intf *intf,
		struct user_access_t *user_access_req)
{
	uint8_t data[4];
	struct ipmi_rq req = {0};
	struct ipmi_rs *rsp;
	if (user_access_req == NULL) {
		return (-3);
	}
	printf("priv : %d\n", user_access_req->privilege_limit);
	data[0] = data[0] >> 16;
	data[0] |= user_access_req->callin_callback << 6;
	data[0] |= user_access_req->link_auth << 5;
	data[0] |= user_access_req->ipmi_messaging << 4;
	data[0] |= user_access_req->privilege_limit; 
	//data[0] |= user_access_req->privilege_limit & 0x0F;
	data[1] = user_access_req->user_id & 0x3F;
	data[2] = user_access_req->enable_status;
	req.msg.netfn = IPMI_APP_NETFN;
	req.msg.cmd = IPMI_SET_USER_ACCESS;
	req.msg.data = data;
	req.msg.data_len = 3;
	rsp = intf->sendrecv(intf, &req);
	if (rsp == NULL) {
		return (-1);
	} else {
		return rsp->ccode;
	}
}
static void dump_user_access(const char *user_name,
                             struct user_access_t *user_access)
{
    static int printed_header = 0;
    if (!printed_header) {
        printf("ID  Name	     Callin  Link Auth	IPMI Msg   "
                       "Channel Priv Limit\n");
        printed_header = 1;
    }
    printf("%-4d%-17s%-8s%-11s%-11s%-s\n",
           user_access->user_id,
           user_name,
           user_access->callin_callback? "false": "true ",
           user_access->link_auth? "true ": "false",
           user_access->ipmi_messaging? "true ": "false",
           val2str(user_access->privilege_limit,
                   ipmi_privlvl_vals));
}
static int ipmi_print_user_list(struct ipmi_intf *intf, uint8_t channel_number)
{
    struct user_access_t user_access = {0};
    struct user_name_t user_name = {0};
    int ccode = 0;
    uint8_t current_user_id = 1;
    do {
        memset(&user_access, 0, sizeof(user_access));
        user_access.user_id = current_user_id;
        user_access.channel = channel_number;
        ccode = _ipmi_get_user_access(intf, &user_access);
        /*
           if (eval_ccode(ccode) != 0) {
           return (-1);
           }
         */
        memset(&user_name, 0, sizeof(user_name));
        user_name.user_id = current_user_id;
        ccode = _ipmi_get_user_name(intf, &user_name);
        /*
           if (eval_ccode(ccode) != 0) {
           return (-1);
           }
         */
        if ((current_user_id == 0)
            || user_access.link_auth
            || user_access.ipmi_messaging
            || strcmp("", (char *)user_name.user_name)) {
            dump_user_access((char *)user_name.user_name,
                             &user_access);
        }
        ++current_user_id;
    } while ((current_user_id <= user_access.max_user_ids)
             && (current_user_id <= IPMI_UID_MAX));
    return 0;
}
int _ipmi_get_user_access(struct ipmi_intf *intf, struct user_access_t *user_access_rsp)
{
    struct ipmi_rq req = {0};
    struct ipmi_rs *rsp;
    uint8_t data[2];
    uint8_t callback;
    if (user_access_rsp == NULL) {
        return (-3);
    }
    data[0] = 0x00; //user_access_rsp->channel & 0x0F;
    data[1] = user_access_rsp->user_id & 0x3F;
    req.msg.netfn = IPMI_APP_NETFN;
    req.msg.cmd = IPMI_GET_USER_ACCESS;
    req.msg.data = data;
    req.msg.data_len = 2;
    rsp = intf->sendrecv(intf, &req);
    //printf("response data is %x\n",rsp->data);
    if (rsp == NULL) {
	//printf("response is null\n");
        return (-1);
    } else if (rsp->ccode != 0) {
        //printf("rsp ccode is not 0!\n");
        return rsp->ccode;
    }/* else if (rsp->data_len != 4) {
        return (-2);
    }*/
    user_access_rsp->max_user_ids = rsp->data[0] & 0x3F;
    user_access_rsp->enable_status = rsp->data[1] & 0xC0;
    user_access_rsp->enabled_user_ids = rsp->data[1] & 0x3F;
    user_access_rsp->fixed_user_ids = rsp->data[2] & 0x3F;
    user_access_rsp->callin_callback = rsp->data[3] & 0x40;
    //printf("1jkkjl1jkljkl123jk123123jkl312k1j23kljkl123jkl;123l\n");
    callback = rsp->data[3] & 0x40;
    //printf("callback : %x\n", callback);
    user_access_rsp->link_auth = rsp->data[3] & 0x20;
    user_access_rsp->ipmi_messaging = rsp->data[3] & 0x10;
    user_access_rsp->privilege_limit = rsp->data[3] & 0x0F;
    return rsp->ccode;
}


int _ipmi_get_user_name(struct ipmi_intf *intf, struct user_name_t *user_name_ptr)
{
    struct ipmi_rq req = {0};
    struct ipmi_rs *rsp;
    uint8_t data[1];
    if (user_name_ptr == NULL) {
        return (-3);
    }
    data[0] = user_name_ptr->user_id & 0x3F;
    req.msg.netfn = IPMI_APP_NETFN;
    req.msg.cmd = IPMI_GET_USER_NAME;
    req.msg.data = data;
    req.msg.data_len = 1;
    rsp = intf->sendrecv(intf, &req);
    if (rsp == NULL) {
        return (-1);
    } else if (rsp->ccode > 0) {
        return rsp->ccode;
    } /*else if (rsp->data_len != 16) {
        return (-2);
    }*/
    memset(user_name_ptr->user_name, '\0', 17);
    memcpy(user_name_ptr->user_name, rsp->data, 16);
    return rsp->ccode;
}
static int
ipmi_user_set_username(
		       struct ipmi_intf *intf,
		       uint8_t user_id,
		       const char *name)
{
	struct ipmi_rs	     * rsp;
	struct ipmi_rq	       req;
	uint8_t	       msg_data[17];

	/*
	 * Ensure there is space for the name in the request message buffer
	 */
	if (strlen(name) >= sizeof(msg_data)) {
		return -1;
	}

	memset(&req, 0, sizeof(req));
	req.msg.netfn    = IPMI_APP_NETFN;	     /* 0x06 */
	req.msg.cmd	     = IPMI_SET_USER_NAME;   /* 0x45 */
	req.msg.data     = msg_data;
	req.msg.data_len = sizeof(msg_data);
	memset(msg_data, 0, sizeof(msg_data));

	/* The channel number will remain constant throughout this function */
	msg_data[0] = user_id;
	strncpy((char *)(msg_data + 1), name, strlen(name));
	printf("msg_data : %s\n", msg_data);
	rsp = intf->sendrecv(intf, &req);

	if (rsp == NULL) {
		return -1;
	}
	if (rsp->ccode > 0) {
		return -1;
	}

	return 0;
}
static void print_user_usage(void)
{
/*
    lprintf(LOG_NOTICE,
            "User Commands:");
    lprintf(LOG_NOTICE,
            "               summary      [<channel number>]");
    lprintf(LOG_NOTICE,
            "               list         [<channel number>]");
    lprintf(LOG_NOTICE,
            "               set name     <user id> <username>");
    lprintf(LOG_NOTICE,
            "               set password <user id> [<password> <16|20>]");
    lprintf(LOG_NOTICE,
            "               disable      <user id>");
    lprintf(LOG_NOTICE,
            "               enable       <user id>");
    lprintf(LOG_NOTICE,
            "               priv         <user id> <privilege level> [<channel number>]");
    lprintf(LOG_NOTICE,
            "                     Privilege levels:");
    lprintf(LOG_NOTICE,
            "                      * 0x1 - Callback");
    lprintf(LOG_NOTICE,
            "                      * 0x2 - User");
    lprintf(LOG_NOTICE,
            "                      * 0x3 - Operator");
    lprintf(LOG_NOTICE,
            "                      * 0x4 - Administrator");
    lprintf(LOG_NOTICE,
            "                      * 0x5 - OEM Proprietary");
    lprintf(LOG_NOTICE,
            "                      * 0xF - No Access");
    lprintf(LOG_NOTICE, "");
    lprintf(LOG_NOTICE,
            "               test         <user id> <16|20> [<password]>");
    lprintf(LOG_NOTICE, "");*/
}
int ipmi_user_list(struct ipmi_intf *intf, int argc, char **argv)
{
    /* List */
    uint8_t channel;
    if (argc == 1) {
        channel = 0x0; /* Ask about the current channel */
    } else {
        print_user_usage();
        return (-1);
    }
    return ipmi_print_user_list(intf, channel);
}
int ipmi_user_main(struct ipmi_intf *intf, int argc, char **argv)
{
    struct user_access_t user_access = {0};
    struct user_name_t user_name = {0};
    int ccodes = 0;
    int i = 0;
    if (argc == 0) {
        print_user_usage();
        return (-1);
    }
    if (strncmp(argv[2], "help", 4) == 0) {
        /* Help */
        print_user_usage();
        return 0;
    }
    else if(strcmp(argv[2], "access")==0)
    {
        user_access.user_id = atoi(argv[3]);
        _ipmi_get_user_access(intf, &user_access);
    }
    else if(strcmp(argv[2], "name") == 0){
        user_name.user_id = atoi(argv[3]);
        ccodes = _ipmi_get_user_name(intf, &user_name);
        //printf("user name ccode is %d\n", ccodes);
        //printf("user name ccode is %x\n", ccodes);
        if(ccodes == 0){
            dump_user_access((char *)user_name.user_name,&user_access);
        }
    }
    else if(strcmp(argv[2], "print") == 0){
        user_access.user_id = atoi(argv[3]);
	user_name.user_id = atoi(argv[3]);
        _ipmi_get_user_access(intf, &user_access);
        ccodes = _ipmi_get_user_name(intf, &user_name);
        if(ccodes == 0){
            dump_user_access((char *)user_name.user_name,&user_access);
        }
    }
    else if(strcmp(argv[2], "list") == 0){
        uint8_t chan = atoi(argv[3]);
        ipmi_print_user_list(intf, chan);
    }
    else if(strcmp(argv[2], "set")==0){
	if(strcmp(argv[3], "name")==0){
	    ipmi_user_set_username(intf, atoi(argv[4]), argv[5]);
	}
	else if(strcmp(argv[3], "access") == 0){
		uint8_t in_access[4]={0};
		for(i=0;i<4;i++){
			in_access[i] = atoi(argv[i+4]);
		}	
	}
	else if(strcmp(argv[3], "priv") == 0){
		user_access.user_id = atoi(argv[4]);
		user_access.enable_status = atoi(argv[5]);
		user_access.callin_callback = atoi(argv[6]);
		user_access.link_auth = atoi(argv[7]);
		user_access.ipmi_messaging = atoi(argv[8]);
		user_access.privilege_limit = atoi(argv[9]);
		ccodes = _ipmi_set_user_access(intf, &user_access);
		if(ccodes == 0x0){
			printf("Set User Privilege command Successful\n");
		}
	}
	else if(strcmp(argv[3], "password") == 0){
		user_access.user_id = atoi(argv[4]);
		uint8_t is_twenty, input_twenty,TF = 0;
		is_twenty = strlen(argv[5]);
		input_twenty = atoi(argv[6]);
		if(is_twenty > input_twenty){
			printf("Fail : Input password is bigger than %d\n", input_twenty);
		}
		else{
			TF  = 1;
			ccodes = _ipmi_set_user_password(intf, user_access.user_id, argv[5], TF);
			if(ccodes == 0x0)
				printf("Set User Password Command Successful\n");
		}	
	}
    }	    
    /*
     * else if (strncmp(argv[0], "summary", 7) == 0) {
        return ipmi_user_summary(intf, argc, argv);
    } else if (strncmp(argv[0], "list", 4) == 0) {
        return ipmi_user_list(intf, argc, argv);
    } else if (strncmp(argv[0], "test", 4) == 0) {
        return ipmi_user_test(intf, argc, argv);
    } else if (strncmp(argv[0], "set", 3) == 0) {
         if ((argc >= 3)
            && (strncmp("password", argv[1], 8) == 0)) {
            return ipmi_user_password(intf, argc, argv);
        } else if ((argc >= 2)
                   && (strncmp("name", argv[1], 4) == 0)) {
            return ipmi_user_name(intf, argc, argv);
        } else {
            print_user_usage();
            return (-1);
        }
    } else if (strncmp(argv[0], "priv", 4) == 0) {
        return ipmi_user_priv(intf, argc, argv);
    } else if ((strncmp(argv[0], "disable", 7) == 0)
               || (strncmp(argv[0], "enable",  6) == 0)) {
        return ipmi_user_mod(intf, argc, argv);
    } else {
        //print_user_usage();
        return (-1);
    }
*/
}

#endif //IPMI_IPMI_USER_H_H



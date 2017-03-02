#include "IL_layer_conf.h"

/****************************************
* Configuration variables		*
****************************************/
ONE_BYTE IL_max_var = IL_MAX_VAR;
ONE_BYTE IL_max_node = MAX_NODE;
ONE_BYTE IL_node_num = NODE_NUM;
ONE_BYTE IL_pic_timeout = PIC_TIMEOUT;

/****************************************
* Shared variables			*
****************************************/
struct IL_loc_t IL_loc[IL_MAX_VAR] = 
{	{ 0, 1},
	{ 1, 2},
	{ 1, 2},
	{ 1, 2},
	{ 1, 1},
	{ 2, 2},
	{ 2, 2},
	{ 2, 2}	
};

void* IL_addr[]=
{
#if (NODE_NUM == 0)
    &sent_command,
#else
    NULL,
#endif
#if (NODE_NUM == 1)
    &r_motor_pow,
    &l_motor_pow,
    &motor_diff,
    &rcv_command,
#else
    NULL,
    NULL,
    NULL,
    NULL,
#endif
#if (NODE_NUM == 2)
    &r_motor_pow,
    &l_motor_pow,
    &motor_diff
#else
    NULL,
    NULL,
    NULL
#endif
};

ONE_BYTE IL_flag[]=
{
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL
};

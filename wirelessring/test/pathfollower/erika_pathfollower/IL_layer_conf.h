#ifndef __IL_LAYER_CONF__
#define __IL_LAYER_CONF__

#include "h8/IL_layer_erika.h"

/****************************************
* Configuration variables		*
****************************************/
#define IL_MAX_VAR 	8
#define MAX_NODE	2
#define NODE_NUM 	2
#define PIC_TIMEOUT	((0x40 / (MAX_NODE + 1)) * NODE_NUM)

extern ONE_BYTE IL_max_var;
extern ONE_BYTE IL_max_node;
extern ONE_BYTE IL_node_num;
extern ONE_BYTE IL_pic_timeout;

/****************************************
* Shared variables			*
****************************************/
#if (NODE_NUM == 0)
    extern ONE_BYTE sent_command;
#endif
#if (NODE_NUM == 1)
    extern TWO_BYTES r_motor_pow;
    extern TWO_BYTES l_motor_pow;
    extern TWO_BYTES motor_diff;
    extern ONE_BYTE rcv_command;
#endif
#if (NODE_NUM == 2)
    extern TWO_BYTES r_motor_pow;
    extern TWO_BYTES l_motor_pow;
    extern TWO_BYTES motor_diff;
#endif

#endif

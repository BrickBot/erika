#ifndef __IL_LAYER_H__
#define __IL_LAYER_H__

#ifdef __IL_SUPPORT__	// Erika
    #include "erika.h"
    #include "IL_layer_erika.h"
#else			// Shark
    #include "IL_layer_shark.h"
#endif

/****************************************************************
* Var								*
****************************************************************/

struct IL_func_t{
    ONE_BYTE confirm_flag;
    void (*confirm)(ONE_BYTE msg_id);
    void (*indication)(ONE_BYTE msg_id);
};

extern struct IL_received_t IL_received;

extern ONE_BYTE IL_max_var;
extern ONE_BYTE IL_max_node;
extern ONE_BYTE IL_node_num;

/****************************************************************
* Functions							*
****************************************************************/

int IL_get(ONE_BYTE index, TWO_BYTES freq);

int IL_put(ONE_BYTE node, ONE_BYTE index, TWO_BYTES freq);

void IL_set_confirm(void (*IL_confirm)(ONE_BYTE msg_id), ONE_BYTE flag);

void IL_set_indication(void (*IL_indication)(ONE_BYTE msg_id));

void IL_pic_config();

void IL_init();

#endif

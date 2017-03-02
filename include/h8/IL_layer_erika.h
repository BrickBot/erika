#ifndef __IL_LAYER_ERIKA_H__
#define __IL_LAYER_ERIKA_H__

#define IL_ERIKA

#include "h8/serial.h"

#define ONE_BYTE	ERIKA_UINT8
#define TWO_BYTES	ERIKA_INT16

#define COM_OK		ERIKA_COM_OK
#define COM_ERROR	ERIKA_COM_ERROR
#define COM_BUSY	ERIKA_COM_BUSY
#define COM_NOACK	ERIKA_COM_NOACK
#define COM_TIMEOUT	ERIKA_COM_TIMEOUT

#include "IL_layer_def.h"

extern ERIKA_UINT8 IL_max_var;
extern ERIKA_UINT8 IL_max_node;
extern ERIKA_UINT8 IL_node_num;
extern ERIKA_UINT8 IL_pic_timeout;

void set_request();

void reset_request();

void IL_IR_init();

void ERIKA_IL_timer();

#define send_request L_Data.request

#define IR_set_result ERIKA_IR_result

#define IL_end ERIKA_IR_Close

/********************************
* Export names			*
********************************/
#define	ERIKA_IL_get IL_get
#define ERIKA_IL_put IL_put

#define ERIKA_IL_init IL_init
#define ERIKA_IL_end IL_end
#define RX_enable ERIKA_IR_receive_enable
#define RX_disable ERIKA_IR_receive_disable
#define ERIKA_IL_set_confirm IL_set_confirm
#define ERIKA_IL_set_indication IL_set_indication

#endif
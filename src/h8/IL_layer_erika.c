#include "erikacfg.h"
#include "erika.h"
#include "h8/IL_layer_erika.h"

ERIKA_INT16 time_to_wait = 0;/*decremented in IR_monitor if 0 reached msg is sent */

void ERIKA_IL_timer()
{
    ERIKA_UINT8 msg[5];
    ERIKA_UINT8 len = 4;
    
    if (time_to_wait > 0)
    {
	time_to_wait -= 2;
        if (time_to_wait <= 0)
	{
	    /* prepare the packet */
	    msg[0] = IL_put_data.node;
	    msg[1] = IL_PUT;
	    msg[2] = IL_put_data.index;
    
	    if (IL_loc[IL_put_data.index].dim == 1)
	    {
		msg[3] = *(ERIKA_UINT8*)(IL_addr[IL_put_data.index]) & 0xFF;
		len = 4;
	    }
	    else
	    {
		msg[3] = (*(ERIKA_INT16*)(IL_addr[IL_put_data.index]) >> 8) & 0xFF;
		msg[4] = *(ERIKA_INT16*)(IL_addr[IL_put_data.index]) & 0xFF;
		len = 5;
	    }
    
	    /*send the packet*/
	    L_Data.request( len, msg);
    
	    /*prepare for next wake up, if needed*/
	    time_to_wait = IL_put_data.freq;	/* if 0, only one, no other msg will be sent */
	}
    }	    
}

void set_request()
{
    time_to_wait = 1;	/*msg will be sent next time*/
}

void reset_request()
{
    time_to_wait = 0;
}

void IL_IR_init()
{
    ERIKA_UINT16 CFC_timeout = 500;
    ERIKA_IR_start(&CFC_timeout);
}
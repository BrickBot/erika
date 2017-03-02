#ifdef __IL_SUPPORT__	// Erika
    #include "erikacfg.h"
    #include "erika.h"
    #include "h8/IL_layer.h"
#else			// Shark
    #include <drivers/IL_layer.h>
#endif

/****************************************************************
* Variables							*
****************************************************************/

struct IL_data_t IL_put_data;

struct IL_data_t IL_req_rcv;

struct IL_func_t IL_func = { IL_FILTER_NONE, NULL, NULL};

struct IL_received_t IL_received;

ONE_BYTE request_flag = 0;

/****************************************************************
* Utility functions						*
****************************************************************/

void save_msg(ONE_BYTE len, ONE_BYTE *m)
{
    ONE_BYTE i, j;

    IL_received.index = m[2];

    if (IL_loc[IL_received.index].dim == 1)
    {
	for( i = 3, j = 0; i < len; i++, j++)
	    IL_received.buffer[j] = (TWO_BYTES)(m[i]);
    }
    else
    {
	for ( i = 3, j = 0; i < len; i+=2, j++)
	    IL_received.buffer[j] = ( ((TWO_BYTES)(m[i]) << 8) & 0xFF00 ) | ( (TWO_BYTES)(m[i+1]) & 0x00FF);
    }
}	    


void IL_confirm(ONE_BYTE msg)	/* propaga all'applicazione la conferma, in base alla configurazione */
{
    if (request_flag)	/* to filter low level confirm */
    {
	request_flag = 0;
	if ( IL_func.confirm_flag & (1 << msg) )    
	    IL_func.confirm(msg);
    }

    RX_enable();
}


void IL_indication(ONE_BYTE len, ONE_BYTE *m)
{
    ONE_BYTE msg_id = m[1];		/* message type */
    IL_req_rcv.node = (m[0] >> 4) & 0xFF;
    IL_req_rcv.index = m[2];

    switch(msg_id)
    {
	case IL_GET_ONCE:
	    if (IL_flag[IL_req_rcv.index] & IL_AUTO_GET_ONCE)
		IL_put( IL_req_rcv.node, IL_req_rcv.index, 0);
	    break;

	case IL_GET_PER:
	    IL_req_rcv.freq = ((m[3] & 0xFFFF) << 8) | (m[4] & 0x00FF);
	    if (IL_flag[IL_req_rcv.index] & IL_AUTO_GET_PER)
		IL_put( IL_req_rcv.node, IL_req_rcv.index, IL_req_rcv.freq);
	    break;	

	case IL_PUT:
	    save_msg(len, m);
	    break;
    }
    
    if (IL_flag[IL_req_rcv.index] & msg_id)
	IL_func.indication(msg_id);

    RX_enable();
}

/****************************************************************
* Library functions						*
****************************************************************/

int IL_get(ONE_BYTE index, TWO_BYTES freq)	/* freq is in msec, 0 => only once */
{
    ONE_BYTE msg[5];
    ONE_BYTE len;
    
    if (index >= IL_max_var && index != IL_STOP)
	return ERR_INDEX_RANGE;

    if (IL_loc[index].node == IL_node_num)
	return ERR_NODE;
	
    if (freq < MIN_FREQ && freq > 0)	/* not only once, but period too short */
	return ERR_FREQ;
    
    msg[0] = (IL_node_num << 4 & 0xFF) | IL_loc[index].node;	/* sender-receiver */

    if (freq == 0)
    {	
	msg[1] = IL_GET_ONCE;
        msg[2] = index;
	len = 3;
    }	
    else
    {
	msg[1] = IL_GET_PER;
        msg[2] = index;
	msg[3] = (freq >> 8) & 0xFF;
	msg[4] = freq & 0xFF;
	len = 5;
    }
    
    request_flag = 1;
    send_request(len, msg);
    return 0;
}

int IL_put(ONE_BYTE node, ONE_BYTE index, TWO_BYTES freq)
{
    if (index != IL_STOP && index >= IL_max_var)
	return ERR_INDEX_RANGE;

    if (index != IL_STOP && (IL_loc[index].node != IL_node_num || node == IL_node_num))
	return ERR_NODE;
	
    if (freq < MIN_FREQ && freq > 0)	/* not only once, but period too short */
	return ERR_FREQ;

    if (index == IL_STOP)		/*reset request*/
    {
	IL_put_data.node = IL_STOP;
	IL_put_data.freq = 0;
	reset_request();
    }
    else				/*prepare for new request*/
    {
        IL_put_data.node = ((IL_node_num << 4) & 0xFF) | node;
	IL_put_data.index = index;
	IL_put_data.freq = freq;
	request_flag = 1;
	set_request();
    }

    return 0;
}

void IL_set_confirm(void (*IL_confirm_func)(ONE_BYTE msg_id), ONE_BYTE flag)
{
    IL_func.confirm = IL_confirm_func;
    IL_func.confirm_flag = flag;
}

void IL_set_indication(void (*IL_indication_func)(ONE_BYTE msg_id))
{
    IL_func.indication = IL_indication_func;
}

void IL_pic_config()
{
    ONE_BYTE init[3];
    
    init[0] = IL_node_num;
    init[1] = IL_max_node;
    init[2] = IL_pic_timeout;
    
    send_request(3, init);
}

void IL_init(void)
{
    /* reset req_data*/
    IL_put_data.node = IL_STOP;

    /* initialize IR */
    IL_IR_init();
    IR_set_result(IL_confirm, IL_indication);
    RX_enable();
}

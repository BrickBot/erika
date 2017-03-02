/*
 * Project: S.Ha.R.K.
 *
 * Coordinators: 
 *   Giorgio Buttazzo    <giorgio@sssup.it>
 *   Paolo Gai           <pj@gandalf.sssup.it>
 *
 * Authors     : 
 *   Paolo Gai           <pj@gandalf.sssup.it>
 *   (see the web pages for full authors list)
 *
 * ReTiS Lab (Scuola Superiore S.Anna - Pisa - Italy)
 *
 * http://www.sssup.it
 * http://retis.sssup.it
 * http://shark.sssup.it
 */

/*
 * Copyright (C) 2004 Michele Cirinei
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 IR_layer.c:

 Layer for IR Serial Communication based on IRQ.
*/

#include <kernel/kern.h>
#include <drivers/IR_layer.h>
#include <stdlib.h>

#define Header		0x55
#define FF		0xFF
#define Zero		0x00
#define MSG		0xFA
#define Lock		0x1E
#define Unlock		0xA5
#define ACK		0xF3

/*
We have to distinguish between enable and disable called by
user and by system, so system uses these two macros while
user uses functions IR_RX_enable and IR_RX_disable
*/

#define RX_ENABLE	(com_irq_enable(RBRF_IRQ | LSR_IRQ))
#define RX_DISABLE	(com_irq_disable(RBRF_IRQ | LSR_IRQ))

#define ENABLED		1
#define DISABLED	0

struct timespec act_time;	/* for timeouts */

#define RX_TIMEOUT	8000
int RX_timeout;
/************************************************
* Message-Acknowledgement Protocol = MAP	*
************************************************/
#define ACK_TIMEOUT	100000
BYTE ACK_req;
BYTE __IR_ACK_REQ__;
int ACK_timeout;

/************************************************
* Channel Free Control = CFC			*
************************************************/
int CFC_TIMEOUT;
#define CFC_DELAY	20000
int CFC_timeout;
int CFC_delay;

struct {BYTE len; BYTE buf[MaxData];} CFC_msg;

/************************************************
* Other Data Types and Variables		*
************************************************/
#define Lock_TIMEOUT 	100000
int Lock_timeout;

PID __tower_on_pid__;
BYTE tower_used;
BYTE TX_started = 0;

unsigned tower_port;

int BufferPos;

BYTE TX_Buffer[519];

struct Buffer { BYTE type, len, buf[MaxData], check;};

enum Rstate_t  { LookingForHeader, HeaderReceived, LockReceived, UnlockReceived, FFReceived, ZeroReceived, typeReceived, ntypeReceived,
		 lenReceived, nlenReceived, bufReceived, nbufReceived, checkReceived };

typedef enum Rstate_t Rstate;

volatile Rstate ReceiveState;
struct Buffer ReceiveBuffer;

struct IR_L_Data_type IR_L_Data;

unsigned Error, Port, b;
BYTE RX_user_set;
BYTE TX_lock;

/****************************************
* Function Declaration			*
****************************************/
void CFC_timeout_handler(void *p);

void CFC_delay_handler(void *p);

void ACK_timeout_handler(void *p);

void RX_timeout_handler(void *p);

void Lock_timeout_handler(void *p);

void IR_confirm(BYTE msg);

void IR_RX_handler (BYTE b);

/****************************************
* Functions				*
****************************************/

void IR_RX_enable(void)
{
    com_irq_enable(RBRF_IRQ | LSR_IRQ);
    RX_user_set = ENABLED;

}

void IR_RX_disable(void)
{
    com_irq_disable(RBRF_IRQ | LSR_IRQ);
    RX_user_set = DISABLED;
}

TASK __tower_on__(void)
{
    while (1)
    {
	if (tower_used)
	    tower_used = 0;
	else
	{
	    if (RX_user_set)
	    {
    		com_write(tower_port, THR, 0xF9);
		com_read(tower_port, RBR);
	    }	
	}	
	task_endcycle();
    }
}

void IR_init(unsigned port, unsigned MAP_set, int *CFC_set)
{ 
    HARD_TASK_MODEL __tower_on_task__;	/* mantains tower on */

    BYTE i=0;
  
    hard_task_default_model(__tower_on_task__);
    hard_task_def_wcet( __tower_on_task__, 2000);
    hard_task_def_mit( __tower_on_task__, 3000000);
  
    __tower_on_pid__ = task_create("Tower ON Task", __tower_on__, &__tower_on_task__, NULL);
    if (__tower_on_pid__ == -1)
    {
	perror("Tower ON Task");
	sys_end();
    }
    
    tower_port = port;
    com_init(port, 2400, ODD, 8, 1);
    com_result(IR_confirm, IR_RX_handler);   
    ReceiveState = LookingForHeader;
    Error = 0;
    Port = port;
    __IR_ACK_REQ__ = MAP_set;
  
    /* MAP*/
    ACK_req = 0;
  
    /* CFC */
    if (CFC_set == NULL)
	CFC_TIMEOUT = NIL;		 /* wait until channel becomes free */
    else
	CFC_TIMEOUT = *CFC_set;  /* wait until channel becomes free or timeout reached*/
    for (i = 0; i < MaxData; i++)
	CFC_msg.buf[i] = 0;
    TX_lock = 0;
    TX_started = 0;
    
    CFC_timeout = NIL;
    CFC_delay = NIL;
    ACK_timeout = NIL;
    RX_timeout = NIL;
    
    task_activate(__tower_on_pid__);
    IR_RX_disable();	  	/* Disable RX interrupts */
}

void IR_send_msg(BYTE len, BYTE *m) /* len = 0 => ACK, len > 0 => number of byte to send */
{
    BYTE check;
    int buf_len;

    if (len != 0)
    {
	if (CFC_delay != NIL)
	{
	    kern_event_delete(CFC_delay);
	    CFC_delay = NIL;
	}
        if (tower_used || ACK_req || TX_lock)
        {
	    if (CFC_msg.buf != m)
	    {
	        CFC_msg.len = len;
		memcpy(CFC_msg.buf, m, len);
	    }
	    if (CFC_TIMEOUT != NIL)
	    {
		if (CFC_timeout != NIL)
		{
		    kern_event_delete(CFC_timeout);
		    CFC_timeout = NIL;
		}
		sys_gettime(&act_time);
		ADDUSEC2TIMESPEC(CFC_TIMEOUT, &act_time);
		CFC_timeout = kern_event_post(&act_time, CFC_timeout_handler, NULL);
	    }
	    return;
	}
	else
	{
	    if (CFC_timeout != NIL)
	    {
		kern_event_delete(CFC_timeout);
		CFC_timeout = NIL;
	    }
	}
    }
    else
    {
	if (__IR_ACK_REQ__)
	    ACK_req = 1;
    }

    tower_used = 1;        
    RX_DISABLE;

    /* Control and set message lenght */
    if (len >= MaxData)
	len = MaxData;

    if (len == 0 && !__IR_ACK_REQ__)
    {
	IR_L_Data.confirm(COM_OK);
	return;
    }
    
    /* Enter word into TransmitBuffer */
    
    TX_Buffer[0] = Header;
    TX_Buffer[1] = FF;
    TX_Buffer[2] = Zero;
    
    if (__IR_ACK_REQ__ && len == 0)
    {
	TX_Buffer[3] = ACK;
	TX_Buffer[4] = (~ACK) & 0xff;
	check = ACK;
	BufferPos = 5;
    }
    else
    {
	TX_Buffer[3] = MSG;
	TX_Buffer[4] = (~MSG) & 0xff;
        TX_Buffer[5] = len;
	TX_Buffer[6] = (~len) & 0xff;
	check = (MSG + len) & 0xff;
	BufferPos = 7;
    }
    
    for (buf_len = 0; buf_len < len; buf_len++) 
    {
	TX_Buffer[BufferPos++] = m[buf_len];
	TX_Buffer[BufferPos++] = (~(m[buf_len])) & 0xff;
	check = (check + m[buf_len]) & 0xff;
    }
  
    /* To avoid blocks in trasmitting 0xff as last byte */
    if (check == 0x00) 
	check = 0x0F;
     
    TX_Buffer[BufferPos++] = check;
    TX_Buffer[BufferPos++] = (~check) & 0xff;

    /* Transmit the message */

    TX_started = 1;
    L_Data.request(BufferPos, TX_Buffer);
}

void IR_RX_handler (BYTE b) /* Receive Data Register Full */
{
    switch ( ReceiveState )
    {
	case LookingForHeader: 
	    if ( b == Header )
	    {
		if (CFC_delay != NIL)
		{
		    kern_event_delete(CFC_delay);
		    CFC_delay = NIL;
		}		    
		tower_used = 1;
		ReceiveState = HeaderReceived;
	    }
	    break;

	case HeaderReceived: 
	    switch ( b )
	    {
		case FF:
		    ReceiveState = FFReceived;
		break;
		
		case Lock:
		    if (!TX_lock)
			ReceiveState = LockReceived;
		    else
		    {
			tower_used = 0;
			if (CFC_timeout != NIL)
			{
			    if (CFC_delay != NIL)
			    {
				kern_event_delete(CFC_delay);
				CFC_delay = NIL;
			    }
			    sys_gettime(&act_time);
			    ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
			    CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
			}
			ReceiveState = LookingForHeader;
		    }
		break;
	    
		case Unlock:
		    if (TX_lock)
			ReceiveState = UnlockReceived;
		    else
		    {
			tower_used = 0;
    			if (CFC_timeout != NIL)
			{
			    if (CFC_delay != NIL)
			    {
				kern_event_delete(CFC_delay);
				CFC_delay = NIL;
			    }
			    sys_gettime(&act_time);
			    ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
			    CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
			}
			ReceiveState = LookingForHeader;
		    }
    		break;
	    
		case Header:
		    /* nothing to do */
		break;
	    
		default:
		    tower_used = 0;
	    	    if (CFC_timeout != NIL)
		    {
			if (CFC_delay != NIL)
			{
			    kern_event_delete(CFC_delay);
			    CFC_delay = NIL;
			}
			sys_gettime(&act_time);
			ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
			CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
		    }
		    ReceiveState = LookingForHeader;
		break;
	    }
	break;

	case LockReceived:
	    if ( b == (~Lock & 0xFF))
		TX_lock = 1;
	    if (Lock_timeout != NIL)
	    {
		kern_event_delete(Lock_timeout);
		Lock_timeout = NIL;
	    }
	    sys_gettime(&act_time);
	    ADDUSEC2TIMESPEC(Lock_TIMEOUT, &act_time);
	    Lock_timeout = kern_event_post(&act_time, Lock_timeout_handler, NULL);
	    tower_used = 0;
	    if (CFC_delay != NIL)
	    {
	        kern_event_delete(CFC_delay);
	        CFC_delay = NIL;
	    }
	    ReceiveState = LookingForHeader;
	break;
    
	case UnlockReceived:
	    if ( b == (~Unlock & 0xFF))
		TX_lock = 0;
	    tower_used = 0;
	    if (CFC_timeout != NIL)
	    {
	        if (CFC_delay != NIL)
		{
	    	    kern_event_delete(CFC_delay);
		    CFC_delay = NIL;
		}
		sys_gettime(&act_time);
		ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
		CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
	    }
	    ReceiveState = LookingForHeader;
	break;

	case FFReceived:
	    if ( b == Zero ) 		// 00
		ReceiveState = ZeroReceived;
	    else if (b != Header)		// not 55
	    {
		tower_used = 0;
		if (CFC_timeout != NIL)
		{
		    if (CFC_delay != NIL)
		    {
			kern_event_delete(CFC_delay);
			CFC_delay = NIL;
		    }
		    sys_gettime(&act_time);
		    ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
		    CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
		}
		ReceiveState = LookingForHeader;
	    }
	    else				// 55
		ReceiveState = HeaderReceived;
	break;

	case ZeroReceived:
	    BufferPos = 0;
	    ReceiveBuffer.type = b;
	    ReceiveBuffer.check = (ReceiveBuffer.type & 0xff);
	    ReceiveState = typeReceived;
	break;

	case typeReceived:
	    if (ReceiveBuffer.type == ACK)		/* ACK received */
	    {
		if (ACK_req)
		{
		    ReceiveState = nbufReceived;
		}
		else					/*  waiting for no ACK */
		{
		    tower_used = 0;
		    if (CFC_timeout != NIL)
		    {
			if (CFC_delay != NIL)
			{
			    kern_event_delete(CFC_delay);
			    CFC_delay = NIL;
			}
			sys_gettime(&act_time);
			ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
			CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
		    }
		    ReceiveState = LookingForHeader;
		}
	    }		
	    else if (ReceiveBuffer.type == MSG)		/* MSG received */
	    {
		if (ACK_req)				/* waiting for an ACK */
		{	    
		    if (ACK_timeout != NIL)
		    {
			kern_event_delete(ACK_timeout);	/* stop timeout */
			ACK_timeout = NIL;
		    }
		    ACK_req = 0;
		    IR_L_Data.confirm(COM_NOACK);
		}
	        ReceiveState = ntypeReceived;
	    }	    
	break;

	case ntypeReceived:
    	    ReceiveBuffer.len = b;
	    ReceiveBuffer.check = ((ReceiveBuffer.check + ReceiveBuffer.len) & 0xff);
    	    ReceiveState = lenReceived; 
	break;
    
	case lenReceived:
	    if (ReceiveBuffer.len > 0)
		ReceiveState = nlenReceived;
	    else
		ReceiveState = nbufReceived;
	break;

	case nlenReceived:
	    ReceiveBuffer.buf[BufferPos] = b;
    	    ReceiveBuffer.check = ((ReceiveBuffer.check + ReceiveBuffer.buf[BufferPos++]) & 0xff);
	    ReceiveState = bufReceived;
	break;
    
	case bufReceived:
	    if (BufferPos < ReceiveBuffer.len)
		ReceiveState = nlenReceived;
	    else
		ReceiveState = nbufReceived;
	break;

	case nbufReceived:       
    	    if (ReceiveBuffer.check == 0x00)	/* To avoid blocks in receiving 0xff as last byte (~check)*/
		ReceiveBuffer.check = 0x0f;
	    ReceiveState = checkReceived;
	break;

	case checkReceived:
	    if ( b == ((~ReceiveBuffer.check) & 0xff) )
	    {
		if (__IR_ACK_REQ__)
		{
		    if (ReceiveBuffer.type == ACK)
		    {
			ACK_req = 0;
			if (ACK_timeout != NIL)
			{
			    kern_event_delete(ACK_timeout);	/* stop timeout */
			    ACK_timeout = NIL;
			}
			IR_L_Data.confirm(COM_OK);	/* ACK received */
		    }
		    else
			IR_L_Data.request(0, NULL);	/* request ACK */
		}
		else	
		    IR_L_Data.indication(ReceiveBuffer.len, ReceiveBuffer.buf);
	    }
	    tower_used = 0;
	    if (CFC_timeout != NIL)
	    {
	        if (CFC_delay != NIL)
		{
		    kern_event_delete(CFC_delay);
		    CFC_delay = NIL;
		}
		sys_gettime(&act_time);
		ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
		CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
	    }
	    ReceiveState = LookingForHeader;
	break;
    }    
    
    if (RX_timeout != NIL)
    {
        kern_event_delete(RX_timeout);
	RX_timeout = NIL;
    }

    if (ReceiveState != LookingForHeader)
    {
	sys_gettime(&act_time);
	ADDUSEC2TIMESPEC(RX_TIMEOUT, &act_time);
	RX_timeout = kern_event_post(&act_time, RX_timeout_handler, NULL);
    }
}

void IR_result(void (*confirm)(BYTE msg_status),void (*indication)(BYTE len, BYTE *data))
{
  IR_L_Data.confirm    = confirm;
  IR_L_Data.request    = IR_send_msg;
  IR_L_Data.indication = indication;
}

void CFC_timeout_handler(void *p)
{
    CFC_timeout = NIL;
    IR_L_Data.confirm(COM_BUSY);
}

void CFC_delay_handler(void *p)
{
    CFC_delay = NIL;
    IR_L_Data.request(CFC_msg.len, CFC_msg.buf);
}	

void ACK_timeout_handler(void *p)
{
    ACK_timeout = NIL;
    ReceiveState = LookingForHeader;
    ACK_req = 0;
    IR_L_Data.confirm(COM_TIMEOUT);
}

void RX_timeout_handler(void *p)
{
    RX_timeout = NIL;
    tower_used = 0;
    if (CFC_timeout != NIL)
    {
        if (CFC_delay != NIL)
	{
	    kern_event_delete(CFC_delay);
	    CFC_delay = NIL;
	}
	sys_gettime(&act_time);
	ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
	CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
    }
    ReceiveState = LookingForHeader;
}

void Lock_timeout_handler(void *p)
{
    Lock_timeout = NIL;
    TX_lock = 0;
    if (CFC_timeout != NIL)
    {
        if (CFC_delay != NIL)
	{
	    kern_event_delete(CFC_delay);
	    CFC_delay = NIL;
	}
	sys_gettime(&act_time);
	ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
	CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
    }
}
    
void IR_confirm(BYTE msg)
{
    tower_used = 0;
    TX_started = 0;
    if (msg == COM_OK && __IR_ACK_REQ__)
    {
	if (ACK_req)	/* message sent was an ACK */
    	{
	    if (RX_user_set == ENABLED)
		RX_ENABLE;			/* to reset RX status */
	    if (ACK_timeout != NIL)
	    {
		kern_event_delete(ACK_timeout);	/* stop timeout */
		ACK_timeout = NIL;
	    }
    	    ACK_req = 0;			
	    IR_L_Data.indication(ReceiveBuffer.len, ReceiveBuffer.buf);
	}
	else		/* message sent was a MSG */
	{
	    RX_ENABLE;	    		/* to receive ACK */
	    ACK_req = 1;		/* waiting for an ACK */
	    if (ACK_timeout != NIL)
	    {
		kern_event_delete(ACK_timeout);	/* stop timeout */
		ACK_timeout = NIL;
	    }
	    sys_gettime(&act_time);
	    ADDUSEC2TIMESPEC(ACK_TIMEOUT, &act_time);
	    ACK_timeout = kern_event_post(&act_time, ACK_timeout_handler, NULL);
	}
    }
    else
    {
    	if (CFC_timeout != NIL)
	{
	    if (CFC_delay != NIL)
	    {
		kern_event_delete(CFC_delay);
		CFC_delay  = NIL;
	    }
	    sys_gettime(&act_time);
	    ADDUSEC2TIMESPEC(CFC_DELAY, &act_time);
	    CFC_delay = kern_event_post(&act_time, CFC_delay_handler, NULL);
	}
        IR_L_Data.confirm(msg);
    }
}

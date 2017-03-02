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
 scomirq.c:

 Layer for IRQ Serial Communication.

*/

#include <kernel/kern.h>
#include <drivers/scomirq.h>
#include <stdlib.h>

/****************************
 Variables
****************************/
static unsigned com_irq[] = {COM1_IRQ, COM2_IRQ, COM3_IRQ, COM4_IRQ};

struct L_Data_type L_Data;

unsigned Error, Port, b;

struct TX_data_t{
    int ptr;
    int len;
    BYTE *buf;
};

static struct TX_data_t TX_data;

int controllo=0;

/*****************************
* Functions
*****************************/

void RBRF_handler();

void THRE_handler();

void LSR_handler();

void com_send_msg(BYTE len, BYTE *m);

void com_irq_sel(int no)
{
    BYTE b = DECODE( com_read(Port, IIR));

    switch (b)
    {	
	case LS_CHANGED: 
	    LSR_handler();
	break;
	    
        case RX_FULL:
	    RBRF_handler();
	break;
	
        case TX_EMPTY:
    	    THRE_handler();
        break;
    }
}	

void com_irq_enable(unsigned irq)
{
    com_read(Port, LSR);
    com_read(Port, IIR);
    com_write(Port, IER, com_read(Port, IER) | irq); /* Enable  irq */
}

void com_irq_disable(unsigned irq)
{
    com_read(Port, LSR);
    com_read(Port, IIR);
    com_write(Port, IER, com_read(Port, IER) & ~irq); /* Disable irq */
}

void com_end()
{
    com_write(Port, IER, 0);
    com_read(Port, LSR);
    com_read(Port, IIR);
    com_close(Port);
}

void com_init(unsigned port, DWORD speed, BYTE parity, BYTE len, BYTE stop) 
{ 
    com_open(port, speed, parity, len, stop);    
    Error = 0;
    Port = port;
    cli();
    handler_set(com_irq[Port], com_irq_sel, NIL);
    sti();
    com_irq_disable(ALL_IRQ);	/* Disable interrupts */
}

void com_result(void (*confirm)(BYTE msg_status),void (*indication)(BYTE data))
{
    L_Data.confirm    = confirm;
    L_Data.request    = com_send_msg;
    L_Data.indication = indication;
}

void com_send_msg(BYTE len, BYTE *m)
{
    com_irq_disable(ALL_IRQ);	/* Disable interrupts */

    /* Transmit first byte of message  */
    TX_data.ptr = 0;
    TX_data.len = len;
    TX_data.buf = malloc(len);
    memcpy(TX_data.buf, m, len);
controllo= len;
    
    com_write(Port, THR, TX_data.buf[TX_data.ptr++]);
    com_irq_enable(THRE_IRQ);  /* Enable THRE */
}

void THRE_handler ( void ) /* Transmit Data Register Ready for next byte */
{ 
    if (TX_data.ptr < TX_data.len)
    {
	com_write(Port, THR, TX_data.buf[TX_data.ptr++]);
	controllo = TX_data.len - TX_data.ptr;
    }
    else
    {
	com_irq_disable(THRE_IRQ);
	while( (com_read(Port,LSR) & bit6) == 0); /* Wait until last byte sent */
	L_Data.confirm(COM_OK);
    }
}

void RBRF_handler ( void ) /* Receive Data Register Full */
{
    L_Data.indication( com_read(Port, RBR) );
}

void LSR_handler ()    /* break = 8, frame = 4, parity = 2, RBR overrun = 1*/
{
    Error = (com_read(Port, LSR) & (bit4|bit3|bit2|bit1) ) >> 1;
    cprintf("COM Error %i\n", Error);
    com_irq_disable(ALL_IRQ);  /* Disable all interrupts */
    L_Data.confirm(COM_ERROR);  
}

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

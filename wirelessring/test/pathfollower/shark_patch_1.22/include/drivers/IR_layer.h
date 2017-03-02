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
 CVS :        $Id: IR_layer.h,v 1.1 2004/07/13 13:13:19 cirinei Exp $

 IR_layer.h:

 Layer for IR Serial Communication based on IRQ.

*/
#ifndef __IR_layer__
#define __IR_layer__

#include <drivers/scomirq.h>

#define MaxData 19

/***********************
*  defined in scomirq.h 
************************

#define bit0    (1 << 0)
#define bit1    (1 << 1)
#define bit2    (1 << 2)
#define bit3    (1 << 3)
#define bit4    (1 << 4)
#define bit5    (1 << 5)
#define bit6    (1 << 6)
#define bit7    (1 << 7)

#define RBRF_IRQ bit0
#define LSR_IRQ  bit2

#define COM_OK    	0
#define COM_ERROR 	1

*/

#define COM_BUSY	2
#define COM_NOACK 	3
#define COM_TIMEOUT 	4

#define ACK_NOT_REQ	0
#define ACK_REQ		1

extern enum Rstate_t ReceiveState;


/*****************************
* Variables
*****************************/
struct IR_L_Data_type {
    void (*request)(BYTE len, BYTE *data);
    void (*confirm)(BYTE msg_id);
    void (*indication)(BYTE len, BYTE *data);
}; 

extern struct IR_L_Data_type IR_L_Data;
/*****************************
* Functions
*****************************/
void IR_init(unsigned port, unsigned MAP_set, int *CFC_set);

void IR_result(void (*confirm)(BYTE msg_status),void (*indication)(BYTE len, BYTE *data));

void IR_RX_enable(void);

void IR_RX_disable(void);

/*****************************
* Inline functions
*****************************/

extern __inline__ void IR_end(void)
{
    com_end();
}

#endif

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
 CVS :        $Id: scomirq.h,v 1.1 2004/07/13 13:13:19 cirinei Exp $

 scomirq.h:

 Layer for IRQ Serial Communication.

*/
#ifndef __SCOMIRQ_H__
#define __SCOMIRQ_H__

#include <drivers/scom.h>

#define bit0    (1 << 0)
#define bit1    (1 << 1)
#define bit2    (1 << 2)
#define bit3    (1 << 3)
#define bit4    (1 << 4)
#define bit5    (1 << 5)
#define bit6    (1 << 6)
#define bit7    (1 << 7)

#define RBRF_IRQ bit0
#define THRE_IRQ bit1
#define LSR_IRQ  bit2
#define ALL_IRQ  (RBRF_IRQ | THRE_IRQ | LSR_IRQ)

#define COM_OK    0
#define COM_ERROR 1

extern int controllo;

/****************************
 Variables
****************************/
struct L_Data_type{
    void (*request)(BYTE len, BYTE *data);
    void (*confirm)(BYTE msg_id);
    void (*indication)(BYTE data);
}; 

extern struct L_Data_type L_Data;

/*****************************
* Functions
*****************************/
void com_end();

void com_irq_enable(unsigned irq);

void com_irq_disable(unsigned irq);

void com_init(unsigned port, DWORD speed, BYTE parity, BYTE len, BYTE stop);

void com_result(void (*confirm)(BYTE msg_status),void (*indication)(BYTE data));

#endif

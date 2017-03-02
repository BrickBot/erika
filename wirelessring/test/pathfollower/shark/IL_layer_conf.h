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
 Path Follower test with Wireless Ring Communication.

 Through the graphic interface you can send requests to a path follower or a 
 repeater (both with ERIKA OS), and visualize data about them. You can also
 send commands to the repeater which send them to the path follower.
*/

#ifndef __IL_LAYER_CONF__
#define __IL_LAYER_CONF__

#include <drivers/IL_layer_shark.h>

/****************************************
* Configuration variables		*
****************************************/
#define IL_MAX_VAR 	8
#define MAX_NODE	2
#define NODE_NUM 	0
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



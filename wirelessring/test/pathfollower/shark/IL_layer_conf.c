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
#include "IL_layer_conf.h"

/****************************************
* Configuration variables		*
****************************************/
ONE_BYTE IL_max_var = IL_MAX_VAR;
ONE_BYTE IL_max_node = MAX_NODE;
ONE_BYTE IL_node_num = NODE_NUM;
ONE_BYTE IL_pic_timeout = PIC_TIMEOUT;

/****************************************
* Shared variables			*
****************************************/
struct IL_loc_t IL_loc[IL_MAX_VAR] = 
{	{ 0, 1},
	{ 1, 2},
	{ 1, 2},
	{ 1, 2},
	{ 1, 1},
	{ 2, 2},
	{ 2, 2},
	{ 2, 2}	
};

void* IL_addr[]=
{
#if (NODE_NUM == 0)
    &sent_command,
#else
    NULL,
#endif
#if (NODE_NUM == 1)
    &r_motor_pow,
    &l_motor_pow,
    &motor_diff,
    &rcv_command,
#else
    NULL,
    NULL,
    NULL,
    NULL,
#endif
#if (NODE_NUM == 2)
    &r_motor_pow,
    &l_motor_pow,
    &motor_diff
#else
    NULL,
    NULL,
    NULL
#endif
};

ONE_BYTE IL_flag[]=
{
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL,
    IL_PUT | IL_AUTO_ALL
};

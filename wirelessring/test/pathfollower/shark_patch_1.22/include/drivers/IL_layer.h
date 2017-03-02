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

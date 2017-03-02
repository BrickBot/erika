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

#ifndef __IL_LAYER_SHARK_H__
#define __IL_LAYER_SHARK_H__

#define IL_SHARK

#include <kernel/kern.h>
#include <drivers/IR_layer.h>
#include <stdlib.h>

#define ONE_BYTE	BYTE
#define TWO_BYTES	WORD

#include "IL_layer_def.h"

void set_request();

void reset_request();

void IL_IR_init();

#define send_request	IR_L_Data.request

#define RX_enable() IR_RX_enable()

#define RX_disable() IR_RX_disable()

#define IR_set_result IR_result

#define IL_end IR_end

#endif

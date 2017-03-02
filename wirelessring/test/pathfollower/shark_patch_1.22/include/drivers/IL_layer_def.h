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

#ifndef __IL_LAYER_DEF_H__
#define __IL_LAYER_DEF_H__

/****************************************************************
* Constants							*
****************************************************************/
#define MIN_FREQ		100
#define ERR_INDEX_RANGE		-1
#define ERR_NODE		-2
#define ERR_FREQ		-3

#define IL_FILTER_OK		(1 << COM_OK)
#define IL_FILTER_ERROR		(1 << COM_ERROR)
#define IL_FILTER_BUSY		(1 << COM_BUSY)
#define IL_FILTER_NOACK		(1 << COM_NOACK)
#define IL_FILTER_TIMEOUT	(1 << COM_TIMEOUT)
#define	IL_FILTER_ALL		(IL_FILTER_OK | IL_FILTER_ERROR | IL_FILTER_BUSY | IL_FILTER_NOACK | IL_FILTER_TIMEOUT)
#define IL_FILTER_ALL_ERRORS	(IL_FILTER_ERROR | IL_FILTER_BUSY | IL_FILTER_NOACK | IL_FILTER_TIMEOUT)
#define IL_FILTER_NONE		0

#define IL_STOP			0xff
#define NO_IL 			NODE_NUM

#define IL_GET_ONCE		(1 << 0)
#define IL_GET_PER		(1 << 1)
#define IL_PUT			(1 << 2)
#define IL_AUTO_GET_ONCE	(1 << 3)
#define IL_AUTO_GET_PER		(1 << 4)
#define IL_INDICATION_ALL	(IL_GET_ONCE | IL_GET_PER | IL_PUT)
#define IL_AUTO_ALL		(IL_AUTO_GET_ONCE | IL_AUTO_GET_PER)

/****************************************************************
* Data 								*
****************************************************************/
struct IL_received_t{
    ONE_BYTE index;
    TWO_BYTES buffer[MaxData-3];
};

struct IL_data_t{
    ONE_BYTE node;
    ONE_BYTE index;
    TWO_BYTES freq;
};

struct IL_loc_t{
    ONE_BYTE node;
    ONE_BYTE dim;
};

extern struct IL_data_t IL_put_data;

extern struct IL_loc_t IL_loc[];

extern void* IL_addr[];

extern ONE_BYTE IL_flag[];

extern ONE_BYTE IL_pic_timeout;

#endif

/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2002 Davide Cantini
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: code.h,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erikacfg.h"
#include "erika.h"		
#include "erikahal.h"


typedef void THREAD;
typedef void HANDLER;

THREAD thread0(void);
THREAD thread1(void);

HANDLER on_off_handler(void);



#ifdef __PLAY_BUFFER_SUPPORT__

#define _pause _DO0 
#define _1th	255
#define _2th	_1th >> 1
#define _4th	_1th >> 2
#define _8th	_1th >> 3
#define _16th	_1th >> 4

static struct ERIKA_note_type n_buf[] =
    { 	{_SOL0,_4th,_4th},
	{_SOL0,_4th,_4th},
	{_SOL0,_4th,_4th},
	{_SOL0,_4th,_4th},
	{_LA0, _2th,_2th},
	{_LA0, _4th,_4th},
	{_LA0, _4th,_4th},
	{_SOL0,_2th,_2th},
        {_MI0, _2th,_2th},	
	{_SOL0,_2th,_2th},
	{_MI0, _2th,_2th},
	{_MI0, _4th,_4th},
	{_MI0, _4th,_4th},    
	{_MI0, _4th,_4th},
	{_MI0, _4th,_4th},
	{_FA0, _2th,_2th},
        {_FA0, _4th,_4th},	
	{_FA0, _4th,_4th},
	{_MI0, _2th,_2th},
	{_DO0, _2th,_2th},
	{_MI0, _2th,_2th},    
	{_DO0, _2th,_2th},
	{_DO1, _1th,_4th},
	{_LA0, _4th,_4th},
        {_SI0, _4th,_4th},
	{_DO1, _4th,_4th},
	{_LA0, _4th,_4th},
	{_SOL0,_1th,_1th},			
	{_LA0, _2th,_2th},	
	{_SOL0,_2th,_2th},	
	{_FA0, _2th,_2th},	
	{_MI0, _2th,_2th},	
	{_DO0, _2th,_2th},	
	{_DO0, _2th,_2th},	
	{_DO0, _2th,_2th},		
	{_DO0, _2th,_2th},	
	{_DO0, _1th, 0},
	{_DO0, _2th, 255},
	{_pause,  0, 255},
	{_pause,  0, 255}		
    };


#define kuka_buffer (n_buf)
#define kuka_buffer_len 40  

#endif

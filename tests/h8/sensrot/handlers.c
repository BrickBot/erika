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
 * CVS: $Id: handlers.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "code.h"

ERIKA_UINT8 power = 1;

/* this handler is executed when ON/OFF button falls down */
HANDLER ONOFFBUTTON_handler(void)
{           
    if (power)
    {
	power=0;
	Port6 &= 0xF8;
	MotorDevReg=0x00;
	ERIKA_lcd_clear();
        ERIKA_disable_irq(RUNBUTTON_IRQ);
		
	/* go into software standby mode */
	SYSCR |= 0x80;	
	__asm__ (".align 1");
	__asm__ ("andc	#0x7F, ccr");	
	__asm__ ("sleep"); 
    }
    else 
	ERIKA_sys_reboot();
}

/* this handler is executed when RUN button falls down */
HANDLER RUNBUTTON_handler(void)
{    
  ERIKA_play_note(_SI0,10);
}


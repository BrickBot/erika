/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001-2002 Paolo Gai
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: irq_h8.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"
#include "cabs/cabs.h"

int th1_counter = 0;
int th2_counter = 0;

/* This function is called by timerA_handler every 1 sec. *
 * See erikacfg.h to define TIMER_A_PERIOD                */
void ERIKA_timerA_user_function(void)
{
    static ERIKA_UINT8 count = 10;
    
    count--;
    if (count==0)
    {	//executed every 1.3 s (130ms * 10)

	/* activate thread */
        ERIKA_IRQ_make_ready(0, 1);

	if (th1_counter == 2) {
          ERIKA_IRQ_make_ready(1, 1);
	  th1_counter = 0;
        }
        else
          th1_counter++;

	if (th2_counter == 4) {
          ERIKA_IRQ_make_ready(2, 1);
	  th2_counter = 0;
        }
        else
          th2_counter++;
	  
	count=10;	  
    }
}



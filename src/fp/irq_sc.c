/*
 * Project: E.R.I.K.A. - Embedded Real tIme Kernel Architecture
 *
 * http://erika.sssup.it
 *
 * Copyright (C) 2001-2002 Paolo Gai, Alessandro Colantonio
 *
 * This program is distributed under the GPL license contained into
 * the license.txt file on the E.R.I.K.A. root directory.
 *
 * CVS: $Id: irq_sc.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"


#ifndef __PRIVATE_IRQ_END_INSTANCE__
/* This primitive shall be atomic.
   This primitive shall be inserted as the last function in an IRQ handler.
   If the HAL allow IRQ nesting the C_end_instance should work as follows:
   - it must implement the preemption test only if it is the last IRQ on the stack
   - if there are other interrupts on the stack the IRQ end_instance should do nothing
*/
void ERIKA_fp_IRQ_end_instance(void)
{
    register ERIKA_TID t;

    ERIKA_hal_IRQ_begin_primitive();
    
    t = ERIKA_rq_queryfirst();
    if (t != ERIKA_NIL && ERIKA_sys_ceiling < ERIKA_th_ready_prio[t]) {
#if defined(__MULTI__)
      register int flag;

      flag = ERIKA_th_status[t] & ERIKA_WASSTACKED;
#endif

      ERIKA_sys_ceiling |= ERIKA_th_dispatch_prio[t];

#if defined(__MULTI__) || defined(__WITH_STATUS__)
      ERIKA_th_status[t] = ERIKA_STACKED;
#endif

#if defined(__MULTI__)
      if (flag)
	ERIKA_hal_IRQ_stacked(ERIKA_rq2stk_exchange());
      else
	ERIKA_hal_IRQ_ready(ERIKA_rq2stk_exchange());
#else
      ERIKA_hal_IRQ_ready(ERIKA_rq2stk_exchange());
#endif
    } else {
      ERIKA_hal_IRQ_stacked(ERIKA_stk_queryfirst());
    }

    /*
      NOOOOOOOO!!!!
      ERIKA_hal_IRQ_end_primitive();
    */
}

#endif /* __PRIVATE_IRQ_END_INSTANCE__ */

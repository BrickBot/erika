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
 * CVS: $Id: irq_act.c,v 1.1.1.1 2003/02/21 09:53:07 pj Exp $
 */

#include "erika.h"

#ifndef __PRIVATE_IRQ_MAKE_READY__
/* This primitive shall be atomic and must be called into an IRQ handler. 
   It works as the thread_make_ready */
void ERIKA_fp_IRQ_make_ready (ERIKA_TID t, ERIKA_TYPENACT nact)
{
  ERIKA_hal_IRQ_begin_primitive();

#ifdef __ENABLE_THREAD__
  if (ERIKA_th_enabled[t]) {
#endif
    
      if (ERIKA_th_nact[t] == 0) {
	/* IDLE
	 * no preemption --> the thread goes into the ready queue
	 * The preemption test will be done into sys_scheduler()
	 */ 
#if defined(__MULTI__) || defined(__WITH_STATUS__)
	ERIKA_th_status[t] = ERIKA_READY;
#endif
	ERIKA_rq_insert(t);
      }
      
      ERIKA_th_nact[t] += nact;
      
#ifdef __ENABLE_THREAD__
  }
#endif

  ERIKA_hal_IRQ_end_primitive();
}

#endif

